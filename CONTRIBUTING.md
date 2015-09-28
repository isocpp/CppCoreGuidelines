## Contributing to the C++ Core Guidelines

>"Within C++ is a smaller, simpler, safer language struggling to get out." 
>-- <cite>Bjarne Stroustrup</cite>

The C++ Core Guidelines are a collaborative effort led by Bjarne Stroustrup, much like the C++ language itself. They are the result of many 
person-years of discussion and design across a number of organizations. Their design encourages general applicability and broad adoption but 
they can be freely copied and modified to meet your organization's needs. 

We encourage contributions to the C++ Core Guidelines in a number of ways:
- **Individual feedback** Are you a developer who is passionate about your code? Join the discussion in 
[Issues](https://github.com/isocpp/CppCoreGuidelines/issues). We want to know which rules resonate with you and which don't. Were any rules
inordinately difficult to apply? Does your compiler vendor's Guideline Support Library (e.g., 
[Microsoft's implementation of the GSL](https://github.com/microsoft/gsl)) suit your needs in adopting these guidelines?
- **Organizational adoption** While the guidelines are designed to be broadly adoptable they are also intended to be modified to fit your
organization's particular needs. We encourage your organization to fork this repo and create your own copy of these guidelines with changes 
that reflect your needs. We suggest that you make it clear in the title of your guidelines that these are your organization's fork of the
guidelines and that you provide a link back to the original set of [guidelines](https://github.com/isocpp/CppCoreGuidelines). And if any of
your local changes are appropriate to pull back into the original guidelines, please open an 
[Issue](https://github.com/isocpp/CppCoreGuidelines/issues) which can lead to a pull request.
- **Maintain the Guidelines** The C++ Core Guidelines were created from a wealth of knowledge spread across a number of organizations
worldwide. If you or your organization is passionate about helping to create the guidelines consider becoming an editor or maintainer. If
you're a C++ expert who is serious about participating please 
[email coreguidelines@isocpp.org](mailto:coreguidelines@isocpp.org?subject=Maintain the C++ Code Guidelines).
 
## Contributor License Agreement
By contributing content to the C++ Core Guidelines (i.e., submitting a pull request for inclusion in this repository) you agree with the 
[Standard C++ Foundation](https://isocpp.org/about) [Terms of Use](https://isocpp.org/home/terms-of-use), especially all of the terms specified
regarding Copyright and Patents.   
- You warrant that your material is original, or you have the right to contribute it.
- With respect to the material that you own, you grant a worldwide non-exclusive irrevocable transferable royalty-free license to your contributed 
material to Standard C++ Foundation to display, reproduce, perform, distribute and create derivative works of that material for commercial or 
non-commercial use. With respect to any other material you contribute, such material must be under a license sufficient to allow Standard C++ Foundation
to display, reproduce, perform, distribute and create derivative works of that material for commercial or non-commercial use.
- You agree that, if your contributed material is subsequently reflected in the ISO/IEC C++ standard in any form, it will be subject to all ISO/IEC JTC 
1 policies including [copyrights](http://www.iso.org/iso/home/policies.htm), 
[patents](http://www.iso.org/iso/home/standards_development/governance_of_technical_work/patents.htm), and 
[procedures](http://www.itscj.ipsj.or.jp/sc29/29w7proc.htm); please direct any questions about these policies to the 
[ISO Central Secretariat](http://www.iso.org/iso/home/about.htm).


## Pull requests
We welcome pull requests for scoped changes to the guidelines--bug fixes in examples, clarifying ambiguous text, etc. Significant changes should
first be discussed in the [Issues](https://github.com/isocpp/CppCoreGuidelines/issues) and the Issue number must be included in the pull
request. Also please specify the rule number in your Issue and PR.

Changes should be made in a child commit of a recent commit in the master branch. Also, if you are making many small changes please create
separate PRs to minimize merge issues. 

Lastly, to avoid line ending issues, please set `autocrlf = input` and `whitespace = cr-at-eol` in your git configuration.

