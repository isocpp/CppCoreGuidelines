## Contributing to the C++ Core Guidelines

>"Within C++ is a smaller, simpler, safer language struggling to get out." 
>-- <cite>Bjarne Stroustrup</cite>

The C++ Core Guidelines are a collaborative effort led by Bjarne Stroustrup, much like the C++ language itself. They are the result of many 
person-years of discussion and design across a number of organizations. Their design encourages general applicability and broad adoption but 
they can be freely copied and modified to meet your organization's needs. 

We encourage contributions to the C++ Core Guidelines in a number of ways:
- **Individual feedback** Are you a developer who is passionate about your code? Join the discussion in 
[Issues](https://github.com/isocpp/CppCoreGuidelines/issues). We want to know which rules resonate with you and which don't. Were any rules
inordinately difficult to apply? Does your compiler vendor's Guidelines Support Library (e.g., 
[Microsoft's implementation of the GSL](https://github.com/microsoft/gsl)) suit your needs in adopting these guidelines?
- **Organizational adoption** While the guidelines are designed to be broadly adoptable they are also intended to be modified to fit your
organization's particular needs. We encourage your organization to fork this repo and create your own copy of these guidelines with changes 
that reflect your needs. We suggest that you make it clear in the title of your guidelines that these are your organization's fork of the
guidelines and that you provide a link back to the original set of [guidelines](https://github.com/isocpp/CppCoreGuidelines). And if any of
your local changes are appropriate to pull back into the original guidelines, please open an 
[Issue](https://github.com/isocpp/CppCoreGuidelines/issues) which can lead to a pull request.
- **Maintain the Guidelines** The C++ Core Guidelines were created from a wealth of knowledge spread across a number of organizations
worldwide. If you or your organization is passionate about helping to create the guidelines, consider becoming an editor or maintainer. If
you're a C++ expert who is serious about participating, please 
[email coreguidelines@isocpp.org](mailto:coreguidelines@isocpp.org?subject=Maintain%20the%20C++%20Code%20Guidelines).
 
## Contributor License Agreement
By contributing content to the C++ Core Guidelines (i.e., submitting a pull request for inclusion in this repository) you agree with the 
[Standard C++ Foundation](https://isocpp.org/about) [Terms of Use](https://isocpp.org/home/terms-of-use), especially all of the terms specified
regarding Copyright and Patents.   
- You warrant that your material is original, or you have the right to contribute it.
- With respect to the material that you own, you grant a worldwide, non-exclusive, irrevocable, transferable, and royalty-free license to your contributed 
material to Standard C++ Foundation to display, reproduce, perform, distribute, and create derivative works of that material for commercial or 
non-commercial use. With respect to any other material you contribute, such material must be under a license sufficient to allow Standard C++ Foundation
to display, reproduce, perform, distribute, and create derivative works of that material for commercial or non-commercial use.
- You agree that, if your contributed material is subsequently reflected in the ISO/IEC C++ standard in any form, it will be subject to all ISO/IEC JTC 
1 policies including [copyrights](http://www.iso.org/iso/home/policies.htm), 
[patents](http://www.iso.org/iso/home/standards_development/governance_of_technical_work/patents.htm), and 
[procedures](http://www.itscj.ipsj.or.jp/sc29/29w7proc.htm); please direct any questions about these policies to the 
[ISO Central Secretariat](http://www.iso.org/iso/home/about.htm).


## Pull requests

We welcome pull requests for scoped changes to the guidelines--bug fixes in
examples, clarifying ambiguous text, etc.  Significant changes should first be
discussed in the [Issues](https://github.com/isocpp/CppCoreGuidelines/issues)
and the Issue number must be included in the pull request.  For
guideline-related changes, please specify the rule number in your Issue and/or
Pull Request.

Changes should be made in a child commit of a recent commit in the master
branch.  If you are making many small changes, please create separate PRs to
minimize merge issues.

### Document Style Guidelines

Documents in this repository are written in an unspecific flavor of Markdown,
which leaves some ambiguity for formatting text.  We ask that pull requests
maintain the following style guidelines, though we are aware that the document
may not already be consistent.

#### Indentation

Code and nested text should use multiples of 4 spaces of indentation, and no
tab characters, like so:

    void func(const int x)
    {
        std::cout << x << '\n';
    }

#### Code Blocks

Please use 4-space indentation to trigger code parsing, rather than [fenced code blocks](https://help.github.com/articles/github-flavored-markdown/#fenced-code-blocks) or any other style, like so:

    This is some document text, with an example below:

        void func()
        {
            std::cout << "This is code.\n";
        }

#### Document style decisions

We've discussed and made decisions on a number of document style. Please do not open PRs that revisit these stylistic points:

- The CppCoreGuidelines.md file is a single GH-flavored Markdown file. It is not split into separate chapters.
- We do not use syntax highlighting in the Core Guidelines. See PRs #33, #96, #328, and #779. If you want syntax highlighting you
can either view the "pretty" version at http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines or do your own post-processing.
- We're sticking with the ASCII character set. We do not use Unicode em-dashes, Unicode spaces, or pretty quotes. Lots of people edit this file with their various text editors. ASCII is simple and universally understood. 

### Miscellaneous

To avoid line-ending issues, please set `autocrlf = input` and `whitespace = cr-at-eol` in your git configuration.
