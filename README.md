# C++ Core Guidelines

>"Within C++ is a smaller, simpler, safer language struggling to get out." 
>-- <cite>Bjarne Stroustrup</cite>

The C++ Core Guidelines are a collaborative effort led by Bjarne Stroustrup, much like the C++ language itself. They are the result of many 
person-years of discussion and design across a number of organizations. Their design encourages general applicability and broad adoption but 
they can be freely copied and modified to meet your organization's needs. 

The aim of the guidelines is to help people to use modern C++ effectively. By "modern C++" we mean C++11 and C++14 (and soon C++17). In other 
words, what would you like your code to look like in 5 years' time, given that you can start now? In 10 years' time?

The guidelines are focused on relatively higher-level issues, such as interfaces, resource management, memory management, and concurrency. Such 
rules affect application architecture and library design. Following the rules will lead to code that is statically type safe, has no resource 
leaks, and catches many more programming logic errors than is common in code today. And it will run fast - you can afford to do things right.

We are less concerned with low-level issues, such as naming conventions and indentation style. However, no topic that can help a programmer is 
out of bounds.

Our initial set of rules emphasize safety (of various forms) and simplicity. They may very well be too strict. We expect to have to introduce 
more exceptions to better accommodate real-world needs. We also need more rules.

You will find some of the rules contrary to your expectations or even contrary to your experience. If we haven't suggested you change your 
coding style in any way, we have failed! Please try to verify or disprove rules! In particular, we'd really like to have some of our rules 
backed up with measurements or better examples.

You will find some of the rules obvious or even trivial. Please remember that one purpose of a guideline is to help someone who is less 
experienced or coming from a different background or language to get up to speed.

The rules are designed to be supported by an analysis tool. Violations of rules will be flagged with references (or links) to the relevant rule. 
We do not expect you to memorize all the rules before trying to write code.

The rules are meant for gradual introduction into a code base. We plan to build tools for that and hope others will too.

Comments and suggestions for improvements are most welcome. We plan to modify and extend this document as our understanding improves and the 
language and the set of available libraries improve.
