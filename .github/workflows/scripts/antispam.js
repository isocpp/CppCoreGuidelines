let configuration = {
    label_if_suspicious: true,
    comment_if_suspicious: true,
    close_if_suspicious: true,
    suspicious_criteria_tolerated: 0
};

async function make_information_report({ user }) {
    // we might also create a (pre-)report for spam to GH using the following informations:
    return `> [!WARNING] About the author:
> 
> | information | value |
> | ----------- | ----- |
> | email       | ${user.email}    |
> | login       | ${user.login}    |
> | name        | ${user.name}     |
> | location    | ${user.location} |
> | blog        | ${user.blog}     |
> | location    | ${user.location} |
`
}

async function when_suspicious({ github, context, failed_checks }){

    // REFACTO: might wanna use a score of confidence (how suspicious it is), then react on that

    const reasons = failed_checks.map(check => `> - ${check.reason}`).join("\n");
    const commentBody = `> [!WARNING] This issue/PR has been automatically flagged as [suspicious] as it might not meet contribution requirements.
> Please read our contribution guide before submitting.
> 
> Reason(s):
> 
${reasons}
`;

    console.log("Body of the produced comment:\n", commentBody);

    if (context.eventName === 'workflow_dispatch') // so we can test manually
        return;

    const { owner, repo } = context.repo;
    const issueNumber = context.payload.number; // either issue or PR

    if (configuration.comment_if_suspicious) {
        await github.rest.issues.createComment({
            owner,
            repo,
            issue_number: issueNumber,
            body: `${commentBody}`
        });
    }
    if (! configuration.label_if_suspicious) {
        await github.rest.issues.addLabels({
            owner,
            repo,
            issue_number: issueNumber,
            labels: ["suspicious"]
        });
    }
    if (configuration.close_if_suspicious) {
        await github.rest.issues.update({
            owner,
            repo,
            issue_number: issueNumber,
            state: "closed"
        });
    }
}

class Check {
    constructor({ predicate, reason }) {
        this.predicate = predicate;
        this.reason = reason;
    }

    async pass() {
        const result = await this.predicate();
        if (typeof result !== "boolean")
            console.error("Check: invalid argument: not a predicate");

        console.debug("- check: ", (result ? "PASSED" : "FAILED"), " => ", this.reason)

        return result;
    }
}

async function run({ github, context }) {

    const username = context.actor;
    const { data: user } = await github.rest.users.getByUsername({ username: username });

    const isAuthorOnlyContributionOnGH = await (async () => {
        // WARNING: Depending on the time of day, event latency can be anywhere from 30s to 6h. (source: https://octokit.github.io/rest.js/v21/)
        const { data: events } = await github.rest.activity.listEventsForAuthenticatedUser({
            username: username,
            per_page: 1
        });
        return events.length === 0;
    })();
    const WasAuthorRecentlyCreated = (() => {

        const time_point = (() => {
            let value = new Date();
            value.setHours(value.getHours() - 2);
            return value;
        })();
        const create_at = new Date(user.created_at);
        return create_at >= time_point;
    })();
    const isTitleOrBodyTooShort = (() => {

        if (context.eventName === 'workflow_dispatch') // issues or pull_request
            return false;

        const payload = context.payload;
        const title = payload.issue?.title || payload.pull_request?.title || "";
        const body = payload.issue?.body || payload.pull_request?.body || "";

        const threshold = 20;
        return title.length < threshold
            || body.length  < threshold;
    })();

    const checks = [
        new Check({
            predicate: () => ! WasAuthorRecentlyCreated,
            reason: "Author account was recently created"
        }),
        new Check({
            predicate: () => ! isAuthorOnlyContributionOnGH,
            reason: "Author first contribution to any GitHub project"
        }),
        new Check({
            predicate: () => user.followers !== 0 && user.following !== 0,
            reason: "Author has no relationships"
        }),
        new Check({
            predicate: () => user.public_repos !== 0 && user.public_gists !== 0,
            reason: "Author has no public repo/gist"
        }),
        new Check({
            predicate: () => ! isTitleOrBodyTooShort,
            reason: "Issue/PR title or body too short"
        }),
    ];

    // IDEA: mandatory checks -> if any fails, then reject
    //       for other checks
    //          use a weights/factors instead of booleans
    //          compute a confidence score to check against a threshold => if below, then reject
    
    async function failedChecks(checks) {
        const results = await Promise.all(
            checks.map(async (check) => ({
                check,
                passed: await check.pass(),
            }))
        );
        return results
            .filter(({ passed }) => ! passed)
            .map(({ check }) => check);
    }

    failedChecks(checks).then(failed_checks => {

        console.log("Checks: ", {
            passed: checks.length - failed_checks.length,
            failed: failed_checks.length
        })

        if (failed_checks.length <= configuration.suspicious_criteria_tolerated) {
            console.info("Not suspicious");
            return;
        }

        when_suspicious({ github, context, failed_checks });

        make_information_report({ user: user }).then(user_information_as_comment => {
            // do stuffs with user_information_as_comment
            console.log("user_information_as_comment", user_information_as_comment);
        });
    });
};

class Testing {

    static enabled = true;

    static #parseGitHubUrl({ url }) {
        const match = url.match(/github\.com\/([^\/]+)\/([^\/]+)\/(issues|pull)\/(\d+)/);
        if (!match || match.length !== 5)
            return null;

        return {
            owner: match[1],
            repo: match[2],
            type: match[3], // "issues" or "pull"
            number: parseInt(match[4], 10),
        };
    }

    static async getContext({ url, github }) {

        const parsed_url = Testing.#parseGitHubUrl({ url: url });
        if (!parsed_url) {
            throw new Error(`Invalid GitHub issue/PR URL: [${url}]`);
        }

        const { owner, repo, type, number } = parsed_url;
        let response;

        try {
            response = (type === "issues")
                ? await github.rest.issues.get({ owner, repo, issue_number: number })
                : await github.rest.pulls.get({ owner, repo, pull_number: number })
            ;
        }
        catch (error) {
            throw new Error(`Failed to fetch ${type.slice(0, -1)} #${number}: ${error.message}`);
        }

        // console.log('response.data', response.data)
        // return response.data;

        // make context adapter => could perform it upstream with graphql
        const payload_content = {
            title: response.data.title,
            body: response.data.body,
        };
        return {
            actor: response.data.user.login,
            sender: response.data.user, 
            eventName: type,
            payload: type === 'issues' ? { issue: payload_content } : { pull_request: payload_content }
        }
    }

    static cases = {

        legits: [
            'https://github.com/isocpp/CppCoreGuidelines/pull/2258',
            'https://github.com/isocpp/CppCoreGuidelines/pull/2259'
        ],
        spams: [
            'https://github.com/isocpp/CppCoreGuidelines/pull/2257',
            'https://github.com/isocpp/CppCoreGuidelines/pull/2241',
            'https://github.com/isocpp/CppCoreGuidelines/pull/2254',
            'https://github.com/isocpp/CppCoreGuidelines/pull/2252',
            'https://github.com/isocpp/CppCoreGuidelines/issues/2249',
            'https://github.com/isocpp/CppCoreGuidelines/issues/2238',
            'https://github.com/isocpp/CppCoreGuidelines/issues/2225',
            'https://github.com/isocpp/CppCoreGuidelines/issues/2255',
        ]
    }
}

module.exports = async ({ github, context }) => {

    if (context.eventName === 'workflow_dispatch'){

        console.log('Testing enabled')

        Testing.enabled = true;
        configuration = {
            label_if_suspicious: false,
            comment_if_suspicious: false,
            close_if_suspicious: false,
            suspicious_criteria_tolerated: 0
        }

        // IDEA: run N-by-N to limit memory bloat
        await Promise.all(
            Testing.cases.spams.map((url) => Testing.getContext({ url, github }))
        ).then(async (testing_contexts) => {
            await run({ github, context, core })
        });

        return;
    }

    return await run({ github, context, core });
};

/*
WIP:
A bit more experiments: made some changes so such a CI can run on an arbitrary issue/PR URL,
so it's easier to test against a list of. See https://github.com/GuillaumeDua/CppCoreGuidelines/actions/runs/13616115173/job/38059374996#step:3:1547
*/
