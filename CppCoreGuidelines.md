<a name="main"></a>
# C++ Core Guidelines

September 9, 2015

Editors:

* [Bjarne Stroustrup](http://www.stroustrup.com)
* [Herb Sutter](http://herbsutter.com/)

This document is a very early draft. It is inkorrekt, incompleat, and pÂµÃ¸oorly formatted.
Had it been an open source (code) project, this would have been release 0.6.
Copying, use, modification, and creation of derivative works from this project is licensed under an MIT-style license.
Contributing to this project requires agreeing to a Contributor License. See the accompanying LICENSE file for details.
We make this project available to "friendly users" to use, copy, modify, and derive from, hoping for constructive input.

Comments and suggestions for improvements are most welcome.
We plan to modify and extend this document as our understanding improves and the language and the set of available libraries improve.
When commenting, please note [the introduction](#S-introduction) that outlines our aims and general approach.
The list of contributors is [here](#SS-ack).

Problems:

* The sets of rules have not been thoroughly checked for completeness, consistency, or enforceability.
* Triple question marks (???) mark known missing information
* Update reference sections; many pre-C++11 sources are too old.
* For a more-or-less up-to-date to-do list see: [To-do: Unclassified proto-rules](#S-unclassified)

You can [Read an explanation of the scope and structure of this Guide](#S-abstract) or just jump straight in:

* [P: Philosophy](#S-philosophy)
* [I: Interfaces](#S-interfaces)
* [F: Functions](#S-functions)
* [C: Classes and class hierarchies](#S-class)
* [Enum: Enumerations](#S-enum)
* [ES: Expressions and statements](#S-expr)
* [E: Error handling](#S-errors)
* [R: Resource management](#S-resource)
* [T: Templates and generic programming](#S-templates)
* [CP: Concurrency](#S-concurrency)
* [SL: The Standard library](#S-stdlib)
* [SF: Source files](#S-source)
* [CPL: C-style programming](#S-cpl)
* [PRO: Profiles](#S-profile)
* [GSL: Guideline support library](#S-gsl)

Supporting sections:

* [NL: Naming and layout](#S-naming)
* [PER: Performance](#S-performance)
* [N: Non-Rules and myths](#S-not)
* [RF: References](#S-references)
* [Appendix A: Libraries](#S-libraries)
* [Appendix B: Modernizing code](#S-modernizing)
* [Appendix C: Discussion](#S-discussion)
* [To-do: Unclassified proto-rules](#S-unclassified)

or look at a specific language feature

* [assignment](#S-???)
* [`class`](#S-class)
* [constructor](#SS-ctor)
* [derived `class`](#SS-hier)
* [destructor](#SS-ctor)
* [exception](#S-errors)
* [`for`](#S-???)
* [`inline`](#S-class)
* [initialization](#S-???)
* [lambda expression](#SS-lambdas)
* [operator](#S-???)
* [`public`, `private`, and `protected`](#S-???)
* [`static_assert`](#S-???)
* [`struct`](#S-class)
* [`template`](#S-???)
* [`unsigned`](#S-???)
* [`virtual`](#SS-hier)

Definitions of terms used to express and discuss the rules, that are not language-technical, but refer to design and programming techniques

* error
* exception
* failure
* invariant
* leak
* precondition
* postcondition
* resource
* exception guarantee


<a name="S-abstract"></a>
# Abstract

This document is a set of guidelines for using C++ well.
The aim of this document is to help people to use modern C++ effectively.
By "modern C++" we mean C++11 and C++14 (and soon C++17).
In other words, what would you like your code to look like in 5 years' time, given that you can start now? In 10 years' time?

The guidelines are focused on relatively higher-level issues, such as interfaces, resource management, memory management, and concurrency.
Such rules affect application architecture and library design.
Following the rules will lead to code that is statically type safe,
has no resource leaks, and catches many more programming logic errors than is common in code today.
And it will run fast - you can afford to do things right.

We are less concerned with low-level issues, such as naming conventions and indentation style.
However, no topic that can help a programmer is out of bounds.

Our initial set of rules emphasize safety (of various forms) and simplicity.
They may very well be too strict.
We expect to have to introduce more exceptions to better accommodate real-world needs.
We also need more rules.

You will find some of the rules contrary to your expectations or even contrary to your experience.
If we haven't suggested you change your coding style in any way, we have failed!
Please try to verify or disprove rules!
In particular, we'd really like to have some of our rules backed up with measurements or better examples.

You will find some of the rules obvious or even trivial.
Please remember that one purpose of a guideline is to help someone who is less experienced or coming from a different background or language to get up to speed.

The rules are designed to be supported by an analysis tool.
Violations of rules will be flagged with references (or links) to the relevant rule.
We do not expect you to memorize all the rules before trying to write code.

The rules are meant for gradual introduction into a code base.
We plan to build tools for that and hope others will too.

Comments and suggestions for improvements are most welcome.
We plan to modify and extend this document as our understanding improves and the language and the set of available libraries improve.


<a name ="S-introduction"></a>
# In: Introduction

This is a set of core guidelines for modern C++, C++14, and taking likely future enhancements and taking ISO Technical Specifications (TSs) into account.
The aim is to help C++ programmers to write simpler, more efficient, more maintainable code.

Introduction summary:

* [In.target: Target readership](#SS-readers)
* [In.aims: Aims](#SS-aims)
* [In.not: Non-aims](#SS-non)
* [In.force: Enforcement](#SS-force)
* [In.struct: The structure of this document](#SS-struct)
* [In.sec: Major sections](#SS-sec)


<a name ="SS-readers"></a>
## In.target: Target readership

All C++ programmers. This includes [programmers who might consider C](#S-cpl).


<a name ="SS-aims"></a>
## In.aims: Aims

The purpose of this document is to help developers to adopt modern C++ (C++11, C++14, and soon C++17) and to achieve a more uniform style across code bases.

We do not suffer the delusion that every one of these rules can be effectively applied to every code base.
Upgrading old systems is hard.
However, we do believe that a program that uses a rule is less error-prone and more maintainable than one that does not.
Often, rules also lead to faster/easier initial development.
As far as we can tell, these rules lead to code that performs as well or better than older, more conventional techniques;
they are meant to follow the zero-overhead principle
("what you don't use, you don't pay for" or "When you use an abstraction mechanism appropriately,
you get at least as good performance as if you had handcoded using lower-level language constructs").
Consider these rules ideals for new code, opportunities to exploit when working on older code, and try to approximate these ideas as closely as feasible.
Remember:

<a name="R0"></a>
### In.0: Don't panic!

Take the time to understand the implications of a guideline rule on your program.

These guidelines are designed according to the "subset of a superset" principle ([Stroustrup,2005](#BS2005)).
They do not simply define a subset of C++ to be used (for reliability, safety, performance, or whatever).
Instead, they strongly recommend the use of a few simple "extensions" ([library components](#S-gsl))
that make the use of the most error-prone features of C++ redundant, so that they can be banned (in our set of rules).

The rules emphasize static type safety and resource safety.
For that reason, they emphasize possibilities for range checking, for avoiding dereferencing `nullptr`, for avoiding dangling pointers, and the systematic use of exceptions (via RAII).
Partly to achieve that and partly to minimize obscure code as a source of errors,
the rules also emphasize simplicity and the hiding of necessary complexity behind well-specified interfaces.

Many of the rules are prescriptive.
We are uncomfortable with rules that simply states "don't do that!" without offering an alternative.
One consequence of that is that some rules can be supported only by heuristics, rather than precise and mechanically verifiable checks.
Other rules articulate general principles. For these more general rules, more detailed and specific rules provide partial checking.

These guidelines address a core of C++ and its use.
We expect that most large organizations, specific application areas, and even large projects will need further rules, possibly further restrictions, and further library support.
For example, hard-real time programmers typically can't use free store (dynamic memory) freely and will be restricted in their choice of libraries.
We encourage the development of such more specific rules as addenda to these core guidelines.
Build your ideal small foundation library and use that, rather than lowering you level of programming to glorified assembly code.

The rules are designed to allow [gradual adoption](#S-modernizing).

Some rules aim to increase various forms of safety while others aim to reduce the likelihood of accidents, many do both.
The guidelines aimed at preventing accidents often ban perfectly legal C++.
However, when there are two ways of expressing an idea and one has shown itself a common source of errors and the other has not, we try to guide programmers towards the latter.

<a name="SS-non"></a>
## In.not: Non-aims

The rules are not intended to be minimal or orthogonal.
In particular, general rules can be simple, but unenforceable.
Also, it is often hard to understand the implications of a general rule.
More specialized rules are often easier to understand and to enforce, but without general rules, they would just be a long list of special cases.
We provide rules aimed as helping novices as well as rules supporting expert use.
Some rules can be completely enforced, but others are based on heuristics.

These rules are not meant to be read serially, like a book.
You can browse through them using the links.
However, their main intended use is to be targets for tools.
That is, a tool looks for violations and the tool returns links to violated rules.
The rules then provide reasons, examples of potential consequences of the violation, and suggested remedies.

These guidelines are not intended to be a substitute for a tutorial treatment of C++.
If you need a tutorial for some given level of experience, see [the references](#S-references).

This is not a guide on how to convert old C++ code to more modern code.
It is meant to articulate ideas for new code in a concrete fashion.
However, see [the modernization section](#S-modernizing) for some possible approaches to modernizing/rejuvenating/upgrading.
Importantly, the rules support gradual adoption: It is typically infeasible to convert all of a large code base at once.

These guidelines are not meant to be complete or exact in every language-technical detail.
For the final word on language definition issues, including every exception to general rules and every feature, see the ISO C++ standard.

The rules are not intended to force you to write in an impoverished subset of C++.
They are *emphatically* not meant to define a, say, Java-like subset of C++.
They are not meant to define a single "one true C++" language.
We value expressiveness and uncompromised performance.

The rules are not value-neutral.
They are meant to make code simpler and more correct/safer than most existing C++ code, without loss of performance.
They are meant to inhibit perfectly valid C++ code that correlates with errors, spurious complexity, and poor performance.


<a name="SS-force"></a>
## In.force: Enforcement

Rules with no enforcement are unmanageable for large code bases.
Enforcement of all rules is possible only for a small weak set of rules or for a specific user community.
But we want lots of rules, and we want rules that everybody can use.
But different people have different needs.
But people don't like to read lots of rules.
But people can't remember many rules.
So, we need subsetting to meet a variety of needs.
But arbitrary subsetting leads to chaos: We want guidelines that help a lot of people, make code more uniform, and strongly encourages people to modernize their code.
We want to encourage best practices, rather than leave all to individual choices and management pressures.
The ideal is to use all rules; that gives the greatest benefits.

This adds up to quite a few dilemmas.
We try to resolve those using tools.
Each rule has an **Enforcement** section listing ideas for enforcement.
Enforcement might be by code review, by static analysis, by compiler, or by run-time checks.
Wherever possible, we prefer "mechanical" checking (humans are slow and bore easily) and static checking.
Run-time checks are suggested only rarely where no alternative exists; we do not want to introduce "distributed fat" - if that's what you want, you know where to find it.
Where appropriate, we label a rule (in the **Enforcement** sections) with the name of groups of related rules (called "profiles").
A rule can be part of several profiles, or none.
For a start, we have a few profiles corresponding to common needs (desires, ideals):

* **types**: No type violations (reinterpreting a `T` as a `U` through casts/unions/varargs)
* **bounds**: No bounds violations (accessing beyond the range of an array)
* **lifetime**: No leaks (failing to `delete` or multiple `delete`) and no access to invalid objects (dereferencing `nullptr`, using a dangling reference).

The profiles are intended to be used by tools, but also serve as an aid to the human reader.
We do not limit our comment in the **Enforcement** sections to things we know how to enforce; some comments are mere wishes that might inspire some tool builder.


<a name ="SS-struct"></a>
## In.struct: The structure of this document

Each rule (guideline, suggestion) can have several parts:

* The rule itself - e.g., **no naked `new`**
* A rule reference number - e.g., **C.7** (the 7th rule related to classes).
Since the major sections are not inherently ordered, we use a letter as the first part of a rule reference "number".
We leave gaps in the numbering to minimize "disruption" when we add or remove rules.
* **Reason**s (rationales) - because programmers find it hard to follow rules they don't understand
* **Example**s - because rules are hard to understand in the abstract; can be positive or negative
* **Alternative**s - for "don't do this" rules
* **Exception**s - we prefer simple general rules. However, many rules apply widely, but not universally, so exceptions must be listed
* **Enforcement** - ideas about how the rule might be checked "mechanically"
* **See also**s - references to related rules and/or further discussion (in this document or elsewhere)
* **Note**s (comments) - something that needs saying that doesn't fit the other classifications
* **Discussion** - references to more extensive rationale and/or examples placed outside the main lists of rules

Some rules are hard to check mechanically, but they all meet the minimal criteria that an expert programmer can spot many violations without too much trouble.
We hope that "mechanical" tools will improve with time to approximate what such an expert programmer notices.
Also, we assume that the rules will be refined over time to make them more precise and checkable.

A rule is aimed at being simple, rather than carefully phrased to mention every alternative and special case.
Such information is found in the **Alternative** paragraphs and the [Discussion](#S-discussion) sections.
If you don't understand a rule or disagree with it, please visit its **Discussion**.
If you feel that a discussion is missing or incomplete, send us an email.

This is not a language manual.
It is meant to be helpful, rather than complete, fully accurate on technical details, or a guide to existing code.
Recommended information sources can be found in [the references](#S-references).


<a name ="SS-sec"></a>
## In.sec: Major sections

* [P: Philosophy](#S-philosophy)
* [I: Interfaces](#S-interfaces)
* [F: Functions](#S-functions)
* [C: Classes and class hierarchies](#S-class)
* [Enum: Enumerations](#S-enum)
* [ES: Expressions and statements](#S-expr)
* [E: Error handling](#S-errors)
* [R: Resource management](#S-resource)
* [T: Templates and generic programming](#S-templates)
* [CP: Concurrency](#S-concurrency)
* [SL: The Standard library](#S-stdlib)
* [SF: Source files](#S-source)
* [CPL: C-style programming](#S-cpl)
* [PRO: Profiles](#S-profile)
* [GSL: Guideline support library](#S-gsl)

Supporting sections:

* [NL: Naming and layout](#S-naming)
* [PER: Performance](#S-performance)
* [N: Non-Rules and myths](#S-not)
* [RF: References](#S-references)
* [Appendix A: Libraries](#S-libraries)
* [Appendix B: Modernizing code](#S-modernizing)
* [Appendix C: Discussion](#S-discussion)
* [To-do: Unclassified proto-rules](#S-unclassified)

These sections are not orthogonal.

Each section (e.g., "P" for "Philosophy") and each subsection (e.g., "C.hier" for "Class Hierachies (OOP)") have an abbreviation for ease of searching and reference.
The main section abbreviations are also used in rule numbers (e.g., "C.11" for "Make concrete types regular").


<a name="S-philosophy"></a>
# P: Philosophy

The rules in this section are very general.

Philosophy rules summary:

* [P.1: Express ideas directly in code](#Rp-direct)
* [P.2: Write in ISO Standard C++](#Rp-C++)
* [P.3: Express intent](#Rp-what)
* [P.4: Ideally, a program should be statically type safe](#Rp-typesafe)
* [P.5: Prefer compile-time checking to run-time checking](#Rp-compile-time)
* [P.6: What cannot be checked at compile time should be checkable at run time](#Rp-run-time)
* [P.7: Catch run-time errors early](#Rp-early)
* [P.8: Don't leak any resource](#Rp-leak)
* [P.9: Don't waste time or space](#Rp-waste)

Philosophical rules are generally not mechanically checkable.
However, individual rules reflecting these philosophical themes are.
Without a philosophical basis the more concrete/specific/checkable rules lack rationale.

<a name="Rp-direct"></a>
### P.1: Express ideas directly in code

**Reason**: Compilers don't read comments (or design documents) and neither do many programmers (consistently).
What is expressed in code has a defined semantics and can (in principle) be checked by compilers and other tools.

**Example**:

	class Date {
		// ...
	public:
		Month month() const;	// do
		int month(); 			// don't
		// ...
	};

The first declaration of `month` is explicit about returning a `Month` and about not modifying the state of the `Date` object.
The second version leaves the reader guessing and opens more possibilities for uncaught bugs.

**Example**:

	void do_something(vector<string>& v)
	{
		string val;
		cin>>val;
		// ...
		int index = 0;						// bad
		for(int i=0; i<v.size(); ++i)
			if (v[i]==val) {
				index = i;
				break;
			}
		// ...
	}
	
That loop is a restricted form of `std::find`.
A much clearer expression of intent would be:

	void do_something(vector<string>& v)
	{
		string val;
		cin>>val;
		// ...
		auto p = find(v,val);				// better
		// ...
	}

A well-designed library expresses intent (what is to be done, rather than just how something is being done) far better than direct use of language features.

A C++ programmer should know the basics of the standard library, and use it where appropriate.
Any programmer should know the basics of the foundation libraries of the project being worked on, and use it appropriately.
Any programmer using these guidelines should know the [Guidelines Support Library](#S-gsl), and use it appropriately.

**Example**:

	change_speed(double s);	// bad: what does s signify?
	// ...
	change_speed(2.3);
	
A better approach is to be explicit about the meaning of the double (new speed or delta on old speed?) and the unit used:

	change_speed(Speed s);	// better: the meaning of s is specified
	// ...
	change_speed(2.3);		// error: no unit
	change_speed(23m/10s);	// meters per second

We could have accepted a plain (unit-less) `double` as a delta, but that would have been error-prone.
If we wanted both absolute speed and deltas, we would have defined a `Delta` type.
	
**Enforcement**: very hard in general.

* use `const` consistently (check if member functions modify their object; check if functions modify arguments passed by pointer or reference)
* flag uses of casts (casts neuter the type system)
* detect code that mimics the standard library (hard)


<a name="Rp-C++"></a>
### P.2: Write in ISO Standard C++

**Reason**: This is a set of guidelines for writing ISO Standard C++.

**Note**: There are environments where extensions are necessary, e.g., to access system resources.
In such cases, localize to use of necessary extensions and control their use with non-core Coding Guidelines.

**Note**: There are environments where restrictions on use of standard C++ language or library features are necessary,
e.g., to avoid dynamic memory allocation as required by aircraft control software standards.
In such cases, control their (dis)use with non-core Coding Guidelines.

**Enforcement**: Use an up-to-date C++ compiler (currently C++11 or C++14) with a set of options that do not accept extensions.
	

<a name="Rp-what"></a>
### P.3: Express intent

**Reason**: Unless the intent of some code is stated (e.g., in names or comments), it is impossible to tell whether the code does what it is supposed to do.

**Example**:

	int i = 0;
	while (i<v.size()) {
		// ... do something with v[i] ...
	}

The intent of "just" looping over the elements of `v` is not expressed here. The implementation detail of an index is exposed (so that it might be misused), and `i` outlives the scope of the loop, which may or may not be intended. The reader cannot know from just this section of code.

Better:

	for (auto x : v) { /* do something with x */ }

Now, there is no explicit mention of the iteration mechanism, and the loop operates on a copy of elements so that accidental modification cannot happen. If modification is desired, say so:

	for (auto& x : v) { /* do something with x */ }

Sometimes better still, use a named algorithm:

	for_each(v,[](int x) { /* do something with x */ });
	for_each(parallel.v,[](int x) { /* do something with x */ });

The last variant makes it clear that we are not interested in the order in which the elements of `v` are handled.

A programmer should be familiar with

* [The guideline support library](#S-gsl)
* [The ISO C++ standard library](#S-stdlib)
* Whatever foundation libraries are used for the current project(s)

**Note**: Alternative formulation: Say what should be done, rather than just how it should be done

**Note**: Some language constructs express intent better than others.

**Example**: if two `int`s are meant to be the coordinates of a 2D point, say so:

		drawline(int,int,int,int);	// obscure
		drawline(Point,Point);		// clearer

**Enforcement**: Look for common patterns for which there are better alternatives

* simple `for` loops vs. range-`for` loops
* `f(T*,int)` interfaces vs. `f(array_view<T>)` interfaces
* loop variable in a too large scope
* naked `new` and `delete`
* functions with many arguments of built-in types

There is a huge scope for cleverness and semi-automated program transformation.


<a name="Rp-typesafe"></a>
### P.4: Ideally, a program should be statically type safe

**Reason**: Ideally, a program would be completely statically (compile-time) type safe.
Unfortunately, that is not possible. Problem areas:

* unions
* casts
* array decay
* range errors
* narrowing conversions

**Note**: These areas are sources of serious problems (e.g., crashes and security violations).
We try to provide alternative techniques.

**Enforcement**: We can ban, restrain, or detect the individual problem categories separately, as required and feasible for individual programs.
Always suggest an alternative.
For example:

* unions - use `variant`
* casts - minimize their use; templates can help
* array decay - use `array_view`
* range errors - use `array_view`
* narrowing conversions - minimize their use and use `narrow` or `narrow_cast` where they are necessary


<a name="Rp-compile-time"></a>
### P.5: Prefer compile-time checking to run-time checking

**Reason**: Code clarity and performance. You don't need to write error handlers for errors caught at compile time.

**Example**:

	void initializer(Int x)
		// Int is an alias used for integers
	{
		static_assert(sizeof(Int)>=4);	// do: compile-time check

		int bits = 0;					// don't: avoidable code
		for (Int i = 1; i; i<<=1)
			++bits;
		if (bits<32)
			cerr << "Int too small\n";
			
		// ...
	}

**Example; don't**:

	void read(int* p, int n);		// read max n integers into *p
	
**Example**:

	void read(array_view<int> r);	// read into the range of integers r

**Alternative formulation**: Don't postpone to run time what can be done well at compile time.

**Enforcement**:

* look for pointer arguments
* look for run-time checks for range violations.


<a name="Rp-run-time"></a>
### P.6: What cannot be checked at compile time should be checkable at run time

**Reason**: Leaving hard-to-detect errors in a program is asking for crashes and bad results.

**Note**: Ideally we catch all errors (that are not errors in the programmer's logic) at either compile-time or run-time. It is impossible to catch all errors at compile time and often not affordable to catch all remaining errors at run time. However, we should endeavor to write programs that in principle can be checked, given sufficient resources (analysis programs, run-time checks, machine resources, time).

**Example, bad**:

	extern void f(int* p);	// separately compiled, possibly dynamically loaded
	
	void g(int n)
	{
		f(new int[n]);	// bad: the number of elements is not passed to f()
	}

Here, a crucial bit of information (the number of elements) has been so thoroughly "obscured" that static analysis is probably rendered infeasible and dynamic checking can be very difficult when `f()` is part of an ABI so that we cannot "instrument" that pointer. We could embed helpful information into the free store, but that requires global changes to a system and maybe to the compiler. What we have here is a design that makes error detection very hard.

**Example, bad**: We can of course pass the number of elements along with the pointer:

	extern void f2(int* p, int n);	// separately compiled, possibly dynamically loaded
	
	void g2(int n)
	{
		f2(new int[n],m);	// bad: the wrong number of elements can be passed to f()
	}

Passing the number of elements as an argument is better (and far more common) that just passing the pointer and relying on some (unstated) convention for knowing or discovering the number of elements. However (as shown), a simple typo can introduce a serious error. The connection between the two arguments of `f2()` is conventional, rather than explicit.

Also, it is implicit that `f2()` is supposed to `delete` its argument (or did the caller make a second mistake?).

**Example, bad**: The standard library resource management pointers fail to pass the size when they point to an object:

	extern void f3(unique_ptr<int[]>, int n);	// separately compiled, possibly dynamically loaded

	void g3(int n)
	{
		f3(make_unique<int[]>(n),m);	// bad: pass ownership and size separately
	}

**Example**: We need to pass the pointer and the number of elements as an integral object:

	extern void f4(vector<int>&);		// separately compiled, possibly dynamically loaded
	extern void f4(array_view<int>);	// separately compiled, possibly dynamically loaded

	void g3(int n)
	{
		vector<int> v(n);
		f4(v);					// pass a reference, retain ownership
		f4(array_view<int>{v});	// pass a view, retain ownership
	}

This design carries the number of elements along as an integral part of an object, so that errors are unlikely and dynamic (run-time) checking is always feasible, if not always affordable.

**Example**: How do we transfer both ownership and all information needed for validating use?

	vector<int> f5(int n)	// OK: move
	{
		vector<int> v(n);
		// ... initialize v ...
		return v;
	}

	unique_ptr<int[]> f6(int n)	// bad: loses n
	{
		auto p = make_unique<int[]>(n);
		// ... initialize *p ...
		return p;
	}

	owner<int*> f7(int n)	// bad: loses n and we might forget to delete
	{
		owner<int*> p = new int[n];
		// ... initialize *p ...
		return p;
	}

**Example**:

	show how possible checks are avoided by interfaces that pass polymorphic base classes around, when they actually know what they need?
	Or strings as "free-style" options
	
**Enforcement**:

* Flag (pointer,count) interfaces (this will flag a lot of examples that can't be fixed for compatibility reasons)
* ???

<a name="Rp-early"></a>
### P.7: Catch run-time errors early

**Reason**: Avoid "mysterious" crashes.
Avoid errors leading to (possibly unrecognized) wrong results.

**Example**:

	void increment1(int* p, int n)	// bad: error prone
	{
		for (int i=0; i<n; ++i) ++p[i];
	}

	void use1(int m)
	{
		const int n = 10;
		int a[n] = {};
		// ...
		increment1(a,m);	// maybe typo, maybe m<=n is supposed
							// but assume that m==20
		// ...
	}

Here we made a small error in `use1` that will lead to corrupted data or a crash.
The (pointer,count) interface leaves `increment1()` with no realistic way of  defending itself against out-of-range errors.
Assuming that we could check subscripts for out of range access, the error would not be discovered until `p[10]` was accessed.
We could check earlier and improve the code:

	void increment2(array_view<int> p)
	{
		for (int& x : p) ++x;
	}

	void use2(int m)
	{
		const int n = 10;
		int a[n] = {};
		// ...
		increment2({a,m});	// maybe typo, maybe m<=n is supposed
		// ...
	}

Now, `m<=n` can be checked at the point of call (early) rather than later.
If all we had was a typo so that we meant to use `n` as the bound, the code could be further simplified (eliminating the possibility of an error):

	void use3(int m)
	{
		const int n = 10;
		int a[n] = {};
		// ...
		increment2(a);	// the number of elements of a need not be repeated
		// ...
	}

**Example, bad**: Don't repeatedly check the same value. Don't pass structured data as strings:

	Date read_date(istream& is);	// read date from istream
	
	Date extract_date(const string& s);	// extract date from string
	
	void user1(const string& date)	// manipulate date
	{
		auto d = extract_date(date);
		// ...
	}
		
	void user2()
	{
		Date d = read_date(cin);
		// ...
		user1(d.to_string());
		// ...
	}
	
The date is validated twice (by the `Date` constructor) and passed as an character string (unstructured data).

**Example**: Excess checking can be costly.
There are cases where checking early is dumb because you may not ever need the value,
or may only need part of the value that is more easily checked than the whole.

	class Jet {	// Physics says: e*e < x*x + y*y + z*z
   		float fx, fy, fz, fe;
	public:
   		Jet(float x, float y, float z, float e)
			:fx(x), fy(y), fz(z), fe(e)
		{
			// Should I check the here that the values are physically meaningful?
   		}

   		float m() const
		{
     		// Should I handle the degenerate case here?
     		return sqrt(x*x + y*y + z*z - e*e);
   		}
		   
		???
	};

The physical law for a jet (`e*e < x*x + y*y + z*z`) is not an invariant because the possibility of measurement errors.

???

**Enforcement**:

* Look at pointers and arrays: Do range-checking early
* Look at conversions: eliminate or mark narrowing conversions.
* Look for unchecked values coming from input
* Look for structured data (objects of classes with invariants) being converted into strings
* ???


<a name="Rp-leak"></a>
### P.8: Don't leak any resource

**Reason**: Essential for long-running programs. Efficiency. Ability to recover from errors.

**Example, bad**:

	void f(char* name)
	{
		FILE* input = fopen(name,"r");
		// ...
		if (something) return;		// bad: if something==true, a file handle is leaked
		// ...
		fclose(input);
	}

Prefer [RAII](#Rr-raii):

	void f(char* name)
	{
		ifstream input {name};
		// ...
		if (something) return;		// OK: no leak
		// ...
	}

**See also**: [The resource management section](#S-resource)

**Enforcement**:

* Look at pointers: classify them into non-owners (the default) and owners.
Where feasible, replace owners with standard-library resource handles (as in the example above).
Alternatively, mark an owner as such using `owner` from [the GSL](#S-gsl).
* Look for naked `new` and `delete`
* look for known resource allocating functions returning raw pointers (such as `fopen`, `malloc`, and `strdup`)


<a name="Rp-waste"></a>
### P.9: Don't waste time or space

**Reason**: This is C++.

**Note**: Time and space that you spend well to achieve a goal (e.g., speed of development, resource safety, or simplification of testing) is not wasted.

**Example**: ??? more and better suggestions for gratuitous waste welcome ???

	struct X {
		char ch;
		int i;
		string s;
		char ch2;
		
		X& operator=(const X& a);
		X(const X&);
	};

	X waste(const char* p)
	{
		if (p==nullptr) throw Nullptr_error{};
		int n = strlen(p);
		auto buf = new char[n];
		for (int i = 0; i<n; ++i) buf[i] = p[i];
		if (buf==nullptr) throw Allocation_error{};
		// ... manipulate buffer ...
		X x;
		x.ch = 'a';
		x.s = string(n);	// give x.s space for *ps
		for (int i=0; i<x.s.size(); ++i) x.s[i] = buf[i];	// copy buf into x.s
		delete buf;
		return x;
	}
	
	void driver()
	{
		X x = waste("Typical argument");
		// ...
	}
	
Yes, this is a caricature, but we have seen every individual mistake in production code, and worse.
Note that the layout of `X` guarantees that at least 6 bytes (and most likely more) bytes are wasted.
The spurious definition of copy operations disables move semantics so that the return operation is slow.
The use of `new` and `delete` for `buf` is redundant; if we really needed a local string, we should use a local `string`.
There are several more performance bugs and gratuitous complication.

**Note**: An individual example of waste is rarely significant, and where it is significant, it is typically easily eliminated by an expert.
However, waste spread liberally across a code base can easily be significant and experts are not always as available as we would like.
The aim of this rule (and the more specific rules that supports it) is to eliminate most waste related to the use of C++ before it happens.
After that, we can look at waste related to algorithms and requirements, but that is beyond the scope of these guidelines.
	
**Enforcement**: Many more specific rules aim at the overall goals of simplicity and elimination of gratuitous waste.



<a name="S-interfaces"></a>
# I: Interfaces

An interface is a contract between two parts of a program. Precisely stating what is expected of a supplier of a service and a user of that service is essential.
Having good (easy-to-understand, encouraging efficient use, not error-prone, supporting testing, etc.) interfaces is probably the most important single aspect of code organization.

Interface rule summary:

* [I.1: Make interfaces explicit](#Ri-explicit)
* [I.2: Avoid global variables](#Ri-global)
* [I.3: Avoid singletons](#Ri-singleton)
* [I.4: Make interfaces precisely and strongly typed](#Ri-typed)
* [I.5: State preconditions (if any)](#Ri-pre)
* [I.6: Prefer `Expects()` for expressing preconditions](#Ri-expects)
* [I.7: State postconditions](#Ri-post)
* [I.8: Prefer `Ensures()` for expressing postconditions](#Ri-ensures)
* [I.9: If an interface is a template, document its parameters using concepts](#Ri-concepts)
* [I.10: Use exceptions to signal a failure to perform a required tasks](#Ri-except)
* [I.11: Never transfer ownership by a raw pointer (`T*`)](#Ri-raw)
* [I.12: Declare a pointer that must not be null as `not_null`](#Ri-nullptr)
* [I.13: Do not pass an array as a single pointer](#Ri-array)
* [I.23: Keep the number of function arguments low](#Ri-nargs)
* [I.24: Avoid adjacent unrelated parameters of the same type](#Ri-unrelated)
* [I.25: Prefer abstract classes as interfaces to class hierarchies](#Ri-abstract)
* [I.26: If you want a cross-compiler ABI, use a C-style subset](#Ri-abi)

See also

* [F: Functions](#S-functions)
* [C.concrete: Concrete types](#SS-concrete)
* [C.hier: Class hierarchies](#SS-hier)
* [C.over: Overloading and overloaded operators](#SS-overload)
* [C.con: Containers and other resource handles](#SS-containers)
* [E: Error handling](#S-errors)
* [T: Templates and generic programming](#S-templates)

<a name="Ri-explicit"></a>
### I.1: Make interfaces explicit

**Reason**: Correctness. Assumptions not stated in an interface are easily overlooked and hard to test.

**Example, bad**:
Controlling the behavior of a function through a global (namespace scope) variable (a call mode) is implicit and potentially confusing. For example,

	int rnd(double d)
	{
		return (rnd_up) ? ceil(d) : d;	// don't: "invisible" dependency
	}

It will not be obvious to a caller that the meaning of two calls of `rnd(7.2)` might give different results.

**Exception**: Sometimes we control the details of a set of operations by an environment variable, e.g., normal vs. verbose output or debug vs. optimized.
The use of a non-local control is potentially confusing, but controls only implementation details of an otherwise fixed semantics.

**Example, bad**: Reporting through non-local variables (e.g., `errno`) is easily ignored. For example:

	fprintf(connection,"logging: %d %d %d\n",x,y,s); // don't: no test of printf's return value
	
What if the connection goes down so than no logging output is produced? See Rule I.??.

**Alternative**: Throw an exception. An exception cannot be ignored.
  
**Alternative formulation**: Avoid passing information across an interface through non-local state.
Note that non-`const` member functions pass information to other member functions through their object's state.

**Alternative formulation**: An interface should be a function or a set of functions.
Functions can be template functions and sets of functions can be classes or class  templates.

**Enforcement**:

* (Simple) A function should not make control-flow decisions based on the values of variables declared at namespace scope.
* (Simple) A function should not write to variables declared at namespace scope.

<a name="Ri-global"></a>
### I.2 Avoid global variables

**Reason**: Non-`const` global variables hide dependencies and make the dependencies subject to unpredictable changes.

**Example**:

	struct Data {
		// ... lots of stuff ...
	} data;			// non-const data

	void compute()	// don't
	{
		// ...use data ...
	}

	void output()	// don't
	{
		// ... use data ...
	}

Who else might modify `data`?

**Note**: global constants are useful.

**Note**: The rule against global variables applies to namespace scope variables as well.

**Alternative**: If you use global (more generally namespace scope data) to avoid copying, consider passing the data as an object by const reference.
Another solution is to define the data as the state of some objects and the operations as member functions.

**Warning**: Beware of data races: if one thread can access nonlocal data (or data passed by reference) while another thread execute the callee, we can have a data race.
Every pointer or reference to mutable data is a potential data race.

**Note**: You cannot have a race condition on immutable data.

**Reference**: See the [rules for calling functions](#SS-call).

**Enforcement**: (Simple) Report all non-`const` variables declared at namespace scope.


<a name="Ri-singleton"></a>
### I.3: Avoid singletons

**Reason**: Singletons are basically complicated global objects in disguise.

**Example**:
	
	class Singleton {
		// ... lots of stuff to ensure that only one Singleton object is created, that it is initialized properly, etc.
	};

There are many variants of the singleton idea.
That's part of the problem.
	
**Note**: If you don't want a global object to change, declare it `const` or `constexpr`.

**Exception**: You can use the simplest "singleton" (so simple that it is often not considered a singleton) to get initialization on first use, if any:

	X& myX()
	{
		static X my_x {3};
		return my_x;
	}

This one of the most effective solution to problem related to initialization order.
In a multi-threaded environment the initialization of the static object does not introduce a race condition
(unless you carelessly access a shared objects from within its constructor).

If you, as many do, define a singleton as a class for which only one object is created, functions like `myX` are not singletons,
and this useful technique is not an exception to the no-singleton rule.
	
**Enforcement**: Very hard in general

* Look for classes with name that includes `singleton`
* Look for classes for which only a single object is created (by counting objects or by examining constructors)


<a name="Ri-typed"></a>
### I.4: Make interfaces precisely and strongly typed

Reason: Types are the simplest and best documentation, have well-defined meaning, and are guaranteed to be checked at compile time.
Also, precisely typed code often optimize better.

**Example; don't**: Consider

	void pass(void* data);	// void* is suspicious

Now the callee has to cast the data pointer (back) to a correct type to use it. That is error-prone and often verbose.
Avoid `void*` in interfaces.
Consider using a variant or a pointer to base instead. (Future note: Consider a pointer to concept.)

**Alternative**: Often, a template parameter can eliminate the `void*` turning it into a `T*` or something like that.

**Example; bad**: Consider

	void draw_rect(int,int,int,int);	// great opportunities for mistakes
	
	draw_rect(p.x,p.y,10,20);			// what does 10,20 mean?

An `int` can carry arbitrary forms of information, so we must guess about the meaning of the four `int`s.
Most likely, the first two are an `x`,`y` coordinate pair, but what are the last two?
Comments and parameter names can help, but we could be explicit:

	void draw_rectangle(Point top_left, Point bottom_right);
	void draw_rectangle(Point top_left, Size height_width);
	
	draw_rectangle(p,Point{10,20});	// two corners
	draw_rectangle(p,Size{10,20});	// one corner and a (height,width) pair

Obviously, we cannot catch all errors through the static type system
(e.g., the fact that a first argument is supposed to be a top-left point is left to convention (naming and comments)).
	

**Example**: ??? units: time duration ???

**Enforcement**:

* (Simple) Report the use of void* as a parameter or return type.
* (Hard to do well) Look for member functions with many built-in type arguments


<a name="Ri-pre"></a>
### I.5: State preconditions (if any)

**Reason**: Arguments have meaning that may constrain their proper use in the callee.

**Example**: Consider

	double sqrt(double x);

Here `x` must be nonnegative. The type system cannot (easily and naturally) express that, so we must use other means. For example:

	double sqrt(double x); // x must be nonnegative

Some preconditions can be expressed as assertions. For example:

	double sqrt(double x) { Expects(x>=0); /* ... */ }

Ideally, that `Expects(x>=0)` should be part of the interface of `sqrt()` but that's not easily done. For now, we place it in the definition (function body).

**Reference**: `Expects()` is described in [GSL](#S-gsl).

**Note**: Prefer a formal specification of requirements, such as `Expects(p!=nullptr);` If that is infeasible, use English text in comments, such as
`// the sequence [p:q) is ordered using <`

**Note**: Most member functions have as a precondition that some class invariant holds.
That invariant is established by a constructor and must be reestablished upon exit by every member function called from outside the class.
We don't need to mention it for each member function.

**Enforcement**: (Not enforceable)

**See also**: the rules for passing pointers.


<a name="Ri-expects"></a>
### I.6: Prefer `Expects()` for expressing preconditions

**Reason**: To make it clear that the condition is a precondition and to enable tool use.

**Example**:

	int area(int height, int width)
	{
		Expects(height>0 && width>0);			// good
		if (height<=0 || width<=0) my_error();	// obscure
		// ...
	}

**Note**: Preconditions can be stated in many ways, including comments, `if`-statements, and `assert()`. This can make them hard to distinguish from ordinary code, hard to update, hard to manipulate by tools, and may have the wrong semantics (do you always want to abort in debug mode and check nothing in productions runs?).

**Note**: Preconditions should be part of the interface rather than part of the implementation, but we don't yet have the language facilities to do that.

**Note**: `Expects()` can also be used to check a condition in the middle of an algorithm.

**Enforcement**: (Not enforceable) Finding the variety of ways preconditions can be asserted is not feasible. Warning about those that can be easily identified (assert()) has questionable value in the absence of a language facility.


<a name="Ri-post"></a>
### I.7: State postconditions

**Reason**: To detect misunderstandings about the result and possibly catch erroneous implementations.

**Example; bad**: Consider

	int area(int height, int width) { return height*width; }	// bad

Here, we (incautiously) left out the precondition specification, so it is not explicit that height and width must be positive.
We also left out the postcondition specification, so it is not obvious that the algorithm (`height*width`) is wrong for areas larger than the largest integer.
Overflow can happen.
Consider using:

	int area(int height, int width)
	{
		auto res = height*width;
		Ensures(res>0);
		return res;
	}

**Example, bad**: Consider a famous security bug

	void f()	// problematic
	{
		char buffer[MAX];
		// ...
		memset(buffer,0,MAX);
	}

There was no postcondition stating that the buffer should be cleared and the optimizer eliminated the apparently redundant `memset()` call:

	void f()	// better
	{
		char buffer[MAX];
		// ...
		memset(buffer,0,MAX);
		Ensures(buffer[0]==0);
	}

**Note** postconditions are often informally stated in a comment that states the purpose of a function; `Ensures()` can be used to make this more systematic, visible, and checkable.

**Note**: Postconditions are especially important when they relate to something that is not directly reflected in a returned result, such as a state of a data structure used.

**Example**: Consider a function that manipulates a `Record`, using a `mutex` to avoid race conditions:

	mutex m;

	void manipulate(Record& r)	// don't
	{
		m.lock();
		// ... no m.unlock() ...
	}

Here, we "forgot" to state that the `mutex` should be released, so we don't know if the failure to ensure release of the `mutex` was a bug or a feature. Stating the postcondition would have made it clear:

	void manipulate(Record& r)	// better: hold the mutex m while and only while manipulating r
	{
		m.lock();
		// ... no m.unlock() ...
	}

The bug is now obvious.

Better still, use [RAII](#Rr-raii) to ensure that the postcondition ("the lock must be released") is enforced in code:

	void manipulate(Record& r)	// best
	{
		lock_guard _ {m};
		// ...
	}
	
**Note**: Ideally, postconditions are stated in the interface/declaration so that users can easily see them.
Only postconditions related to the users can be stated in the interface.
Postconditions related only to internal state belongs in the definition/implementation.

**Enforcement**: (Not enforceable) This is a philosophical guideline that is infeasible to check directly.


<a name="Ri-ensures"></a>
### I.8: Prefer `Ensures()` for expressing postconditions

**Reason**: To make it clear that the condition is a postcondition and to enable tool use.

**Example**:

	void f()
	{
		char buffer[MAX];
		// ...
		memset(buffer,0,MAX);
		Ensures(buffer[0]==0);
	}

**Note**: postconditions can be stated in many ways, including comments, `if`-statements, and `assert()`. This can make them hard to distinguish from ordinary code, hard to update, hard to manipulate by tools, and may have the wrong semantics.

**Alternative**: Postconditions of the form "this resource must be released" are best expressed by [RAII](#Rr-raii).

Ideally, that `Ensures` should be part of the interface that's not easily done. For now, we place it in the definition (function body).

**Enforcement**: (Not enforceable) Finding the variety of ways postconditions can be asserted is not feasible. Warning about those that can be easily identified (assert()) has questionable value in the absence of a language facility.


<a name="Ri-concepts"></a>
### I.9: If an interface is a template, document its parameters using concepts

**Reason**: Make the interface precisely specified and compile-time checkable in the (not so distant) future.

**Example**: Use the ISO Concepts TS style of requirements specification. For example:

	template<typename Iter, typename Val>
	//	requires InputIterator<Iter> && EqualityComparable<ValueType<Iter>>,Val>
	Iter find(Iter first, Iter last, Val v)
	{
		// ...
	}

**Note**: Soon (maybe in 2016), most compilers will be able to check `requires` clauses once the `//` is removed.

**See also**: See [generic programming](???) and [???](???)

**Enforcement**: (Not enforceable yet) A language facility is under specification. When the language facility is available, warn if any non-variadic template parameter is not constrained by a concept (in its declaration or mentioned in a `requires` clause.


<a name="Ri-except"></a>
### I.10: Use exceptions to signal a failure to perform a required task

**Reason**: It should not be possible to ignore an error because that could leave the system or a computation in an undefined (or unexpected) state.
This is a major source of errors.

**Example**:

	int printf(const char* ...);	// bad: return negative number if output fails

	template <class F, class ...Args>
	explicit thread(F&& f, Args&&... args);	// good: throw system_error if unable to start the new thread

	
**Note**: What is an error?
An error means that the function cannot achieve its advertised purpose (including establishing postconditions).
Calling code that ignores the error could lead to wrong results or undefined systems state.
For example, not being able to connect to a remote server is not by itself an error:
the server can refuse a connection for all kinds of reasons, so the natural thing is to return a result that the caller always has to check.
However, if failing to make a connection is considered an error, then a failure should throw an exception.

**Exception**: Many traditional interface functions (e.g., UNIX signal handlers) use error codes (e.g., `errno`) to report what are really status codes, rather than errors. You don't have good alternative to using such, so calling these does not violate the rule.

**Alternative**: If you can't use exceptions (e.g. because your code is full of old-style raw-pointer use or because there are hard-real-time constraints),
consider using a style that returns a pair of values:

	int val;
	int error_code;
	tie(val,error_code) = do_something();
	if (error_code==0) {
		// ... handle the error or exit ...
	}
	// ... use val ...

**Note**: We don't consider "performance" a valid reason not to use exceptions.

* Often, explicit error checking and handling consume as much time and space as exception handling.
* Often, cleaner code yields better performance with exceptions (simplifying the tracing of paths through the program and their optimization).
* A good rule for performance critical code is to move checking outside the critical part of the code ([checking](#Rper-checking)).
* In the longer term, more regular code gets better optimized.

**See also**: Rule I.??? and I.??? for reporting precondition and postcondition violations.

**Enforcement**:

* (Not enforceable) This is a philosophical guideline that is infeasible to check directly.
* look for `errno`.


<a name="Ri-raw"></a>
### I.11: Never transfer ownership by a raw pointer (`T*`)

**Reason**: If there is any doubt whether the caller or the callee owns an object, leaks or premature destruction will occur.

**Example**: Consider

	X* compute(args)	// don't
	{
		X* res = new X{};
		// ...
		return res;
	}

Who deletes the returned `X`? The problem would be harder to spot if compute returned a reference.
Consider returning the result by value (use move semantics if the result is large):

	vector<double> compute(args)	// good
	{
		vector<double> res(10000);
		// ...
		return res;
	}

**Alternative**: Pass ownership using a "smart pointer", such as `unique_ptr` (for exclusive ownership) and `shared_ptr` (for shared ownership).
However that is less elegant and less efficient unless reference semantics are needed.

**Alternative**: Sometimes older code can't be modified because of ABI compatibility requirements or lack of resources.
In that case, mark owning pointers using `owner` :

	owner<X*> compute(args)		// It is now clear that ownership is transferred
	{
		owner<X*> res = new X{};
		// ...
		return res;
	}

This tells analysis tools that `res` is an owner.
That is, its value must be `delete`d or transferred to another owner, as is done here by the `return`.

`owner` is used similarly in the implementation of resource handles.

`owner` is defined in the [Guideline Support Library](#S-gsl).

**Note**: Every object passed as a raw pointer (or iterator) is assumed to be owned by the caller, so that its lifetime is handled by the caller.

**See also**: [Argument passing](#Rf-conventional) and [value return](#Rf-T-return).

**Enforcement**:

* (Simple) Warn on `delete` of a raw pointer that is not an `owner`.
* (Simple) Warn on failure to either `reset` or explicitly `delete` an `owner` pointer on every code path.
* (Simple) Warn if the return value of `new` or a function call with return value of pointer type is assigned to a raw pointer.


<a name="Ri-nullptr"></a>
### I.12: Declare a pointer that must not be null as `not_null`

**Reason**: To help avoid dereferencing `nullptr` errors. To improve performance by avoiding redundant checks for `nullptr`.

**Example**:

	int length(const char* p);		// it is not clear whether strlen(nullptr) is valid

	length(nullptr);				// OK?

	int length(not_null<const char*> p);		// better: we can assume that p cannot be nullptr

	int length(const char* p);				// we must assume that p can be nullptr

By stating the intent in source, implementers and tools can provide better diagnostics, such as finding some classes of errors through static analysis, and perform optimizations, such as removing branches and null tests.

**Note**: The assumption that the pointer to `char` pointed to a C-style string (a zero-terminated string of characters) was still implicit, and a potential source of confusion and errors. Use `zstring` in preference to `const char*`.

	int length(not_null<zstring> p);		// we can assume that p cannot be nullptr
									// we can assume that p points to a zero-terminated array of characters

Note: `length()` is, of course, `std::strlen()` in disguise.

**Enforcement**:

* (Simple) ((Foundation)) If a function checks a pointer parameter against `nullptr` before access, on all control-flow paths, then warn it should be declared `not_null`.
* (Complex) If a function with pointer return value ensures it is not `nullptr` on all return paths, then warn the return type should be declared `not_null`.


<a name="Ri-array"></a>
### I.13: Do not pass an array as a single pointer

**Reason**: (pointer,size)-style interfaces are error-prone. Also, a plain pointer (to array) must rely on some convention to allow the callee to determine the size.

**Example**: Consider

	void copy_n(const T* p, T* q, int n); // copy from [p:p+n) to [q:q+n)

What if there are fewer than `n` elements in the array pointed to by `q`? Then, we overwrite some probably unrelated memory.
What if there are fewer than `n` elements in the array pointed to by `p`? Then, we read some probably unrelated memory.
Either is undefined behavior and a potentially very nasty bug.

**Alternative**: Consider using explicit ranges,

	void copy(array_view<const T> r, array_view<T> r2); // copy r to r2

**Example, bad**: Consider

	void draw(Shape* p, int n);	// poor interface; poor code
	Circle arr[10];
	// ...
	draw(arr,10);

Passing `10` as the `n` argument may be a mistake: the most common convention is to assume [`0`:`n`) but that is nowhere stated. Worse is that the call of `draw()` compiled at all: there was an implicit conversion from array to pointer (array decay) and then another implicit conversion from `Circle` to `Shape`. There is no way that `draw()` can safely iterate through that array: it has no way of knowing the size of the elements.

**Alternative**: Use a support class that ensures that the number of elements is correct and prevents dangerous implicit conversions. For example:

	void draw2(array_view<Circle>);
	Circle arr[10];
	// ...
	draw2(array_view<Circle>(arr));	// deduce the number of elements
	draw2(arr);	// deduce the element type and array size

	void draw3(array_view<Shape>);
	draw3(arr);	// error: cannot convert Circle[10] to array_view<Shape>

This `draw2()` passes the same amount of information to `draw()`, but makes the fact that it is supposed to be a range of `Circle`s explicit. See ???.

**Exception**: Use `zstring` and `czstring` to represent a C-style, zero-terminated strings. But see ???.

**Enforcement**:

* (Simple) ((Bounds)) Warn for any expression that would rely on implicit conversion of an array type to a pointer type. Allow exception for zstring/czstring pointer types.
* (Simple) ((Bounds)) Warn for any arithmetic operation on an expression of pointer type that results in a value of pointer type. Allow exception for zstring/czstring pointer types.


<a name="Ri-nargs"></a>
### I.14: Keep the number of function arguments low

**Reason**: Having many arguments opens opportunities for confusion. Passing lots of arguments is often costly compared to alternatives.

**Example**: The standard-library `merge()` is at the limit of what we can comfortably handle

	template<class InputIterator1, class InputIterator2, class OutputIterator, class Compare>
	OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
						InputIterator2 first2, InputIterator2 last2,
						OutputIterator result, Compare comp);

Here, we have four template arguments and six function arguments.
To simplify the most frequent and simplest uses, the comparison argument can be defaulted to `<`:

	template<class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
						InputIterator2 first2, InputIterator2 last2,
						OutputIterator result);

This doesn't reduce the total complexity, but it reduces the surface complexity presented to many users.
To really reduce the number of arguments, we need to bundle the arguments into higher-level abstractions:

	template<class InputRange1, class InputRange2, class OutputIterator>
	OutputIterator merge(InputRange1 r1, InputRange2 r2, OutputIterator result);

Grouping arguments into "bundles" is a general technique to reduce the number of arguments and to increase the opportunities for checking.

**Note**: How many arguments are too many? Four arguments is a lot.
There are functions that are best expressed with four individual arguments, but not many.

**Alternative**: Group arguments into meaningful objects and pass the objects (by value or by reference).

**Alternative**: Use default arguments or overloads to allow the most common forms of calls to be done with fewer arguments.

**Enforcement**:
   - Warn when a functions declares two iterators (including pointers) of the same type instead of a range or a view.
   - (Not enforceable) This is a philosophical guideline that is infeasible to check directly.


<a name="Ri-unrelated"></a>
### I.15: Avoid adjacent unrelated parameters of the same type

**Reason**: Adjacent arguments of the same type are easily swapped by mistake.

**Example; bad**: Consider

		void copy_n(T* p, T* q, int n); // copy from [p:p+n) to [q:q+n)

This is a nasty variant of a K&R C-style interface. It is easy to reverse the "to" and "from" arguments.

Use `const` for the "from" argument:

		void copy_n(const T* p, T* q, int n); // copy from [p:p+n) to [q:q+n)

**Alternative**: Don't pass arrays as pointers, pass an object representing a range (e.g., an `array_view`):

		void copy_n(array_view<const T> p, array_view<T> q); // copy from b to q

**Enforcement**: (Simple) Warn if two consecutive parameters share the same type.


<a name="Ri-abstract"></a>
### I.16: Prefer abstract classes as interfaces to class hierarchies

**Reason**: Abstract classes are more likely to be stable than base classes with state.

**Example; bad**: You just knew that `Shape` would turn up somewhere :-)

	class Shape {	// bad: interface class loaded with data
	public:
		Point center() { return c; }
		virtual void draw();
		virtual void rotate(int);
		// ...
	private:
		Point c;
		vector<Point> outline;
		Color col;
	};

This will force every derived class to compute a center -- even if that's non-trivial and the center is never used. Similarly, not every `Shape` has a `Color`, and many `Shape`s are best represented without an outline defined as a sequence of `Point`s. Abstract classes were invented to discourage users from writing such classes:

	class Shape {	// better: Shape is a pure interface
	public:
		virtual Point center() =0;	// pure virtual function
		virtual void draw() =0;
		virtual void rotate(int) =0;
		// ...
		// ... no data members ...
	};

**Enforcement**: (Simple) Warn if a pointer to a class `C` is assigned to a pointer to a base of `C` and the base class contains data members.


<a name="Ri-abi"></a>
### I.16: If you want a cross-compiler ABI, use a C-style subset

**Reason**: Different compilers implement different binary layouts for classes, exception handling, function names, and other implementation details.

**Exception**: You can carefully craft an interface using a few carefully selected higher-level C++ types. See ???.

**Exception**: Common ABIs are emerging on some platforms freeing you from the more Draconian restrictions.

**Note**: if you use a single compiler, you can use full C++ in interfaces. That may require recompilation after an upgrade to a new compiler version.

**Enforcement**: (Not enforceable) It is difficult to reliably identify where an interface forms part of an ABI.


<a name="S-functions"></a>
# F: Functions

A function specifies an action or a computation that takes the system from one consistent state to the next. It is the fundamental building block of programs.

It should be possible to name a function meaningfully, to specify the requirements of its argument, and clearly state the relationship between the arguments and the result. An implementation is not a specification. Try to think about what a function does as well as about how it does it.
Functions are the most critical part in most interfaces, so see the interface rules.

Function rule summary:

Function definition rules:

* [F.1: "Package" meaningful operations as carefully named functions](#Rf-package)
* [F.2: A function should perform a single logical operation](#Rf-logical)
* [F.3: Keep functions short and simple](#Rf-single)
* [F.4: If a function may have to be evaluated at compile time, declare it `constexpr`](#Rf-constexpr)
* [F.5: If a function is very small and time critical, declare it inline](#Rf-inline)
* [F.6: If your function may not throw, declare it `noexcept`](#Rf-noexcept)
* [F.7: For general use, take `T*` arguments rather than a smart pointers](#Rf-smart)
* [F.8: Prefer pure functions](#Rf-pure)

Argument passing rules:

* [F.15: Prefer simple and conventional ways of passing information](#Rf-conventional)
* [F.16: Use `T*` or `owner<T*>` or a smart pointer to designate a single object](#Rf-ptr)
* [F.17: Use a `not_null<T>` to indicate "null" is not a valid value](#Rf-nullptr)
* [F.18: Use an `array_view<T>` or an `array_view_p<T>` to designate a half-open sequence](#Rf-range)
* [F.19: Use a `zstring` or a `not_null<zstring>` to designate a C-style string](#Rf-string)
* [F.20: Use a `const T&` parameter for a large object](#Rf-const-T-ref)
* [F.21: Use a `T` parameter for a small object](#Rf-T)
* [F.22: Use `T&` for an in-out-parameter](#Rf-T-ref)
* [F.23: Use `T&` for an out-parameter that is expensive to move (only)](#Rf-T-return-out)
* [F.24: Use a `TP&&` parameter when forwarding (only)](#Rf-pass-ref-ref)
* [F.25: Use a `T&&` parameter together with `move` for rare optimization opportunities](#Rf-pass-ref-move)
* [F.26: Use a `unique_ptr<T>` to transfer ownership where a pointer is needed](#Rf-unique_ptr)
* [F.27: Use a `shared_ptr<T>` to share ownership](#Rf-shared_ptr)

Value return rules:

* [F.40: Prefer return values to out-parameters](#Rf-T-return)
* [F.41: Prefer to return tuples to multiple out-parameters](#Rf-T-multi)
* [F.42: Return a `T*` to indicate a position (only)](#Rf-return-ptr)
* [F.43: Never (directly or indirectly) return a pointer to a local object](#Rf-dangle)
* [F.44: Return a `T&` when "returning no object" isn't an option](#Rf-return-ref)
* [F.45: Don't return a `T&&`](#Rf-return-ref-ref)

Other function rules:

* [F.50: Use a lambda when a function won't do (to capture local variables, or to write a local function)](#Rf-capture-vs-overload)
* [F.51: Prefer overloading over default arguments for virtual functions](#Rf-default-args)
* [F.52: Prefer capturing by reference in lambdas that will be used locally, including passed to algorithms](#Rf-reference-capture)
* [F.53: Avoid capturing by reference in lambdas that will be used nonlocally, including returned, stored on the heap, or passed to another thread](#Rf-value-capture)

Functions have strong similarities to lambdas and function objects so see also Section ???.


<a name="SS-fct-def"></a>
## F.def: Function definitions

A function definition is a function declaration that also specifies the function's implementation, the function body.


<a name="Rf-package"></a>
### F.1: "Package" meaningful operations as carefully named functions

**Reason**: Factoring out common code makes code more readable, more likely to be reused, and limit errors from complex code.
If something is a well-specified action, separate it out from its  surrounding code and give it a name.

**Example, don't**:

	void read_and_print(istream& is)	// read and print an int
	{
		int x;
		if (is>>x)
			cout << "the int is " << x << '\n';
		else
			cerr << "no int on input\n";
	}
	
Almost everything is wrong with `read_and_print`.
It reads, it writes (to a fixed `ostream`), it write error messages (to a fixed `ostream`), it handles only `int`s.
There is nothing to reuse, logically separate operations are intermingled and local variables are in scope after the end of their logical use.
For a tiny example, this looks OK, but if the input operation, the output operation, and the error handling had been more complicated the tangled
mess could become hard to understand.

**Note**: If you write a non-trivial lambda that potentially can be used in more than one place,
give it a name by assigning it to a (usually non-local) variable.

**Example**:

	sort(a, b, [](T x, T y) { return x.valid() && y.valid() && x.value()<y.value(); });

Naming that lambda breaks up the expression into its logical parts and provides a strong hint to the meaning of the lambda.

	auto lessT = [](T x, T y) { return x.valid() && y.valid() && x.value()<y.value(); };
	
	sort(a, b, lessT);
	find_if(a,b, lessT);

The shortest code is not always the best for performance or maintainability.

**Exception**: Loop bodies, including lambdas used as loop bodies, rarely needs to be named.
However, large loop bodies (e.g., dozens of lines or dozens of pages) can be a problem.
The rule [Keep functions short](#Rf-single) implies "Keep loop bodies short."
Similarly, lambdas used as callback arguments are sometimes non-trivial, yet unlikely to be re-usable.

**Enforcement**:

* See [Keep functions short](#Rf-single)
* Flag identical and very similar lambdas used in different places.


<a name="Rf-logical"></a>
### F.2: A function should perform a single logical operation

**Reason**: A function that performs a single operation is simpler to understand, test, and reuse.

**Example**: Consider

	void read_and_print()	// bad
	{
		int x;
		cin >> x;
		// check for errors
		cout << x << "\n";
	}

This is a monolith that is tied to a specific input and will never find a another (different) use. Instead, break functions up into suitable logical parts and parameterize:

	int read(istream& is)	// better
	{
		int x;
		is >> x;
		// check for errors
		return x;
	}

	void print(ostream& os, int x)
	{
		os << x << "\n";
	}

These can now be combined where needed:

	void read_and_print()
	{
		auto x = read(cin);
		print(cout, x);
	}

If there was a need, we could further templatize `read()` and `print()` on the data type, the I/O mechanism, etc. Example:

	auto read = [](auto& input, auto& value)	// better
	{
		input >> value;
		// check for errors
	}

	auto print(auto& output, const auto& value)
	{
		output << value << "\n";
	}

**Enforcement**:

* Consider functions with more than one "out" parameter suspicious. Use return values instead, including `tuple` for multiple return values.
* Consider "large" functions that don't fit on one editor screen suspicious. Consider factoring such a function into smaller well-named suboperations.
* Consider functions with 7 or more parameters suspicious.


<a name="Rf-single"></a>
### F.3: Keep functions short and simple

**Reason**: Large functions are hard to read, more likely to contain complex code, and more likely to have variables in larger than minimal scopes.
Functions with complex control structures are more likely to be long and more likely to hide logical errors

**Example**: Consider

	double simpleFunc(double val, int flag1, int flag2)
		// simpleFunc: takes a value and calculates the expected ASIC output, given the two mode flags.
	{
  		double intermediate;
  		if (flag1 > 0) {
    		intermediate = func1(val);
    		if (flag2 % 2)
      			intermediate = sqrt(intermediate);
  		}
  		else if (flag1 == -1) {
    		intermediate = func1(-val);
    		if (flag2 % 2)
      			intermediate = sqrt(-intermediate);
    		flag1 = -flag1;
  		}
  		if (abs(flag2) > 10) {
   			intermediate = func2(intermediate);
  		}
  		switch (flag2 / 10) {
    	case 1: if (flag1 == -1) return finalize(intermediate, 1.171); break;
    	case 2: return finalize(intermediate, 13.1);
    	default: ;
  		}
  		return finalize(intermediate, 0.);
	}

This is too complex (and also pretty long).
How would you know if all possible alternatives have been correctly handled?
Yes, it break other rules also.

We can refactor:

	double func1_muon(double val, int flag)
	{
		// ???
	}
	
	double funct1_tau(double val, int flag1, int flag2)
	{
		// ???
	}
	
	double simpleFunc(double val, int flag1, int flag2)
		// simpleFunc: takes a value and calculates the expected ASIC output, given the two mode flags.
	{
  		if (flag1 > 0)
 			return func1_muon(val, flag2);
		if (flag1 == -1)
    		return func1_tau(-val, flag1, flag2);	// handled by func1_tau: flag1 = -flag1;
  		return 0.;
	}

**Note**: "It doesn't fit on a screen" is often a good practical definition of "far too large."
One-to-five-line functions should be considered normal.

**Note**: Break large functions up into smaller cohesive and named functions.
Small simple functions are easily inlined where the cost of a function call is significant.

**Enforcement**:

* Flag functions that do not "fit on a screen."
How big is a screen? Try 60 lines by 140 characters; that's roughly the maximum that's comfortable for a book page.
* Flag functions that are too complex. How complex is too complex?
You could use cyclomatic complexity. Try "more that 10 logical path through." Count a simple switch as one path.


<a name="Rf-constexpr"></a>
### F.4: If a function may have to be evaluated at compile time, declare it `constexpr`

**Reason**: `constexpr` is needed to tell the compiler to allow compile-time evaluation.

**Example**: The (in)famous factorial:

	constexpr int fac(int n)
	{
		constexpr int max_exp = 17;     // constexpr enables  this to be used in Expects
		Expects(0<=n && n<max_exp);		// prevent silliness and overflow
		int x = 1;
		for (int i=2; i<=n; ++i) x*= n;
		return x;
	}
	
This is C++14. For C++11, use a functional formulation of `fac()`.

**Note**: `constexpr` does not guarantee compile-time evaluation;
it just guarantees that the function can be evaluated at compile time for constant expression arguments if the programmer requires it or the compiler decides to do so to optimize.

	constexpr int min(int x, int y) { return x<y?x:y;}
	
	void test(int v)
	{
		int m1 = min(-1,2);				// probably compile-time evaluation
		constexpr int m2 = min(-1,2);	// compile-time evaluation
		int m3 = min(-1,v);				// run-time evaluation
		constexpr int m4 = min(-1,v);	// error: connot evaluate at compile-time
	}
	
**Note**: `constexpr` functions are pure: they can have no side effects.

	int dcount = 0;
	constexpr int double(int v)
	{
		++dcount;		// error: attempted side effect from constexpr function
		return v+v;
	}
	
This is usually a very good thing.

**Note**: Don't try to make all functions `constexpr`. Most computation is best done at run time.

**Enforcement**: Impossible and unnecessary.
The compiler gives an error if a non-`constexpr` function is called where a constant is required.


<a name="Rf-inline"></a>
### F.5: If a function is very small and time critical, declare it `inline`

**Reason**: Some optimizers are good at inlining without hints from the programmer, but don't rely on it.
Measure! Over the last 40 years or so, we have been promised compilers that can inline better than humans without hints from humans.
We are still waiting.
Specifying `inline` encourages the compiler to do a better job.

**Exception**: Do not put an `inline` function in what is meant to be a stable interface unless you are really sure that it will not change.
An inline function is part of the ABI.

**Note**: `constexpr` implies `inline`.

**Note**: Member functions defined in-class are `inline` by default.

**Exception**: Template functions (incl. template member functions) must be in headers and therefore inline.

**Enforcement**: Flag `inline` functions that are more than three statements and could have been declared out of line (such as class member functions).
To fix: Declare the function out of line. [[NM: Certainly possible, but size-based metrics can be very annoying.]]


<a name="Rf-noexcept"></a>
### F.6: If your function may not throw, declare it `noexcept`

**Reason**: If an exception is not supposed to be thrown, the program cannot be assumed to cope with the error and should be terminated as soon as possible. Declaring a function `noexcept` helps optimizers by reducing the number of alternative execution paths. It also speeds up the exit after failure.

**Example**: Put `noexcept` on every function written completely in C or in any other language without exceptions.
The C++ standard library does that implicitly for all functions in the C standard library.

**Note**: `constexpr` functions cannot throw, so you don't need to use `noexcept` for those.

**Example**: You can use `noexcept` even on functions that can throw:

	vector<string> collect(istream& is) noexcept
	{
		vector<string> res;
		for(string s; is>>s; )
			res.push_back(s);
		return res;
	}

If `collect()` runs out of memory, the program crashes.
Unless the program is crafted to survive memory exhaustion, that may be just the right thing to do;
`terminate()` may generate suitable error log information (but after memory runs out it is hard to do anything clever).

**Note**: In most programs, most functions can throw
(e.g., because they use `new`, call functions that do, or use library functions that reports failure by throwing),
so don't just sprinkle `noexcept` all over the place.
`noexcept` is most useful for frequently used, low-level functions.

**Note**: Destructors, `swap` functions, move operations, and default constructors should never throw.


**Enforcement**:

* Flag functions that are not `noexcept`, yet cannot throw
* Flag throwing `swap`, `move`, destructors, and default constructors.


<a name="Rf-smart"></a>
### F.7: For general use, take `T*` arguments rather than a smart pointers

**Reason**: Passing a smart pointer transfers or shares ownership.
Passing by smart pointer restricts the use of a function to callers that use smart pointers.
Passing a shared smart pointer (e.g., `std::shared_ptr`) implies a run-time cost.

**Example**:

	void f(int*);		// accepts any int*
	void g(unique_ptr<int>);	// can only accept ints for which you want to transfer ownership
	void g(shared_ptr<int>);	// can only accept ints for which you are willing to share ownership

**Note**: We can catch dangling pointers statically, so we don't need to rely on resource management to avoid violations from dangling pointers.

**See also**: Discussion of [smart pointer use](#Rr-summary-smartptrs).

**Enforcement**: Flag smart pointer arguments.


<a name="Rf-pure"></a>
### F.8: Prefer pure functions


**Reason**: Pure functions are easier to reason about, sometimes easier to optimize (and even parallelize), and sometimes can be memoized.

**Example**:

    template<class T>
	auto square(T t) { return t*t; }
	
**Note**: `constexpr` functions are pure.
	
**Enforcement**: not possible.


<a name="SS-call"></a>
## F.call: Argument passing

There are a variety of ways to pass arguments to a function and to return values.


<a name="Rf-conventional"></a>
### Rule F.15: Prefer simple and conventional ways of passing information

**Reason**: Using "unusual and clever" techniques causes surprises, slows understanding by other programmers, and encourages bugs.
If you really feel the need for an optimization beyond the common techniques, measure to ensure that it really is an improvement,
and document/comment because the improvement may not be portable.

![Normal parameter passing table](./param-passing-normal.png "Normal parameter passing")

**For an "output-only" value:** Prefer return values to output parameters.
This includes large objects like standard containers that use implicit move operations for performance and to avoid explicit memory management.
If you have multiple values to return, [use a tuple](#Rf-T-multi) or similar multi-member type.

**Example**:

	vector<const int*> find_all(const vector<int>&, int x);	// return pointers to elements with the value x
	
**Example, bad**:

	void find_all(const vector<int>&, vector<const int*>& out, int x);	// place pointers to elements with value x in out

**Exceptions**:

* For non-value types, such as types in an inheritance hierarchy, return the object by `unique_ptr` or `shared_ptr`.
* If a type is expensive to move (e.g., `array<BigPOD>`), consider allocating it on the free store and return a handle (e.g., `unique_ptr`), or passing it in a non-`const` reference to a target object to fill (to be used as an out-parameter).
* In the special case of allowing a caller to reuse an object that carries capacity (e.g., `std::string`, `std::vector`) across multiple calls to the function in an inner loop, treat it as an in/out parameter instead and pass by `&`. This one use of the more generally named "caller-allocated out" pattern.

**For an "in-out" parameter:** Pass by non-`const` reference. This makes it clear to callers that the object is assumed to be modified.

**For an "input-only" value:** If the object is cheap to copy, pass by value.
Otherwise, pass by `const&`. It is useful to know that a function does not mutate an argument, and both allow initialization by rvalues.
What is "cheap to copy" depends on the machine architecture, but two or three words (doubles, pointers, references) are usually best passed by value.
In particular, an object passed by value does not require an extra reference to access from the function.

![Advanced parameter passing table](./param-passing-advanced.png "Advanced parameter passing")

For advanced uses (only), where you really need to optimize for rvalues passed to "input-only" parameters:

* If the function is going to unconditionally move from the argument, take it by `&&`.
* If the function is going to keep a copy of the argument, in addition to passing by `const&` add an overload that passes the parameter by `&&` and in the body `std::move`s it to its destination. (See [F.25](#Rf-pass-ref-move).)
* In special cases, such as multiple "input + copy" parameters, consider using perfect forwarding. (See [F.24](#Rf-pass-ref-ref).)

**Example**:

		int multiply(int, int); // just input ints, pass by value
 
		string& concatenate(string&, const string& suffix); // suffix is input-only but not as cheap as an int, pass by const&
		
		void sink(unique_ptr<widget>);	// input only, and consumes the widget
 
Avoid "esoteric techniques" such as:

* Passing arguments as `T&&` "for efficiency". Most rumors about performance advantages from passing by `&&` are false or brittle (but see [F.25](#Rf-pass-ref-move).)
* Returning `const T&` from assignments and similar operations.

**Example**: Assuming that `Matrix` has move operations (possibly by keeping its elements in a `std::vector`.

	Matrix operator+(const Matrix& a, const Matrix& b)
	{
		Matrix res;
		// ... fill res with the sum ...
		return res;
	}

	Matrix x = m1+m2;	// move constructor

	y = m3+m3;			// move assignment

**Note**: The (optional) return value optimization doesn't handle the assignment case.

**See also**: [implicit arguments](#Ri-explicit).

**Enforcement**: This is a philosophical guideline that is infeasible to check directly and completely.
However, many of the the detailed rules (F.16-F.45) can be checked,
such as passing a `const int&`, returning an `array<BigPOD>` by value, and returning a pointer to fre store alloced by the function.


<a name="Rf-ptr"></a>
### F.16: Use `T*` or `owner<T*>` to designate a single object

**Reason**: In traditional C and C++ code, "Plain `T*` is used for many weakly-related purposes, such as

* Identify a (single) object (not to be deleted by this function)
* Point to an object allocated on the free store (and delete it later)
* Hold the `nullptr`
* Identify a C-style string (zero-terminated array of characters)
* Identify an array with a length specified separately
* Identify a location in an array

Confusion about what meaning a `T*` is the source of many serious errors, so using separate names for pointers of these separate uses makes code clearer.
For debugging, `owner<T*>` and `not_null<T>` can be instrumented to check.
For example, `not_null<T*>` makes it obvious to a reader (human or machine) that a test for `nullptr` is not necessary before dereference.

**Example**: Consider

	int length(Record* p);

When I call `length(r)` should I test for `r==nullptr` first? Should the implementation of `length()` test for `p==nullptr`?

	int length(not_null<Record*> p);	// it is the caller's job to make sure p!=nullptr

	int length(Record* p);	// the implementor of length() must assume that p==nullptr is possible

**Note**: A `not_null<T>` is assumed not to be the `nullptr`; a `T*` may be the `nullptr`; both can be represented in memory as a `T*` (so no run-time overhead is implied).

**Note**: `owner<T*>` represents ownership.

**Also**: Assume that a `T*` obtained from a smart pointer to `T` (e.g., unique_ptr<`T`>) points to a single element.

**See also**: [Support library](#S-gsl).

**Enforcement**:

* (Simple) ((Bounds)) Warn for any arithmetic operation on an expression of pointer type that results in a value of pointer type.


<a name="Rf-nullptr"></a>
### F.17: Use a `not_null<T>` to indicate that "null" is not a valid value

**Reason**: Clarity. Making it clear that a test for null isn't needed.

**Example**:

	not_null<T*> check(T* p) { if (p) return not_null<T*>{p}; throw Unexpected_nullptr{}; }

	void computer(not_null<array_view<int>> p)
	{
		if (0<p.size()) {	// bad: redundant test
			// ...
		}
	}

**Note**: `not_null` is not just for built-in pointers. It works for `array_view`, `string_view`, `unique_ptr`, `shared_ptr`, and other pointer-like types.

**Enforcement**:

* (Simple) Warn if a raw pointer is dereferenced without being tested against `nullptr` (or equivalent) within a function, suggest it is declared `not_null` instead.
* (Simple) Error if a raw pointer is sometimes dereferenced after first being tested against `nullptr` (or equivalent) within the function and sometimes is not.
* (Simple) Warn if a `not_null` pointer is tested against `nullptr` within a function.


<a name="Rf-range"></a>
### F.18: Use an `array_view<T>` or an `array_view_p<T>` to designate a half-open sequence

**Reason**: Informal/non-explicit ranges are a source of errors

**Example**:

	X* find(array_view<X> r, const X& v)	// find v in r

	vector<X> vec;
	// ...
	auto p = find({vec.begin(),vec.end()},X{});	// find X{} in vec

**Note**: Ranges are extremely common in C++ code. Typically, they are implicit and their correct use is very hard to ensure. In particular, given a pair of arguments `(p,n)` designating an array [`p`:`p+n`), it is in general impossible to know if there really are n elements to access following `*p`. `array_view<T>` and `array_view_p<T>` are simple helper classes designating a [p:q) range and a range starting with p and ending with the first element for which a predicate is true, respectively.

**Note**: an `array_view<T>` object does not own its elements and is so small that it can be passed by value.

**Note**: Passing an `array_view` object as an argument is exactly as efficient as passing a pair of pointer arguments or passing a pointer and an integer count.

**See also**: [Support library](#S-gsl).

**Enforcement**: (Complex) Warn where accesses to pointer parameters are bounded by other parameters that are integral types and suggest they could use `array_view` instead.


<a name="Rf-string"></a>
### F.19: Use a `zstring` or a `not_null<zstring>` to designate a C-style string

**Reason**: 
C-style strings are ubiquitous. They are defined by convention: zero-terminated arrays of characters. 
We must distinguish C-style strings from a pointer to a single character or an old-fashioned pointer to an array of characters.


**Example**: Consider

	int length(const char* p);

When I call `length(s)` should I test for `s==nullptr` first? Should the implementation of `length()` test for `p==nullptr`?

	int length(zstring p);	// the implementor of length() must assume that p==nullptr is possible

	int length(not_null<zstring> p);	// it is the caller's job to make sure p!=nullptr

**Note**: `zstring` do not represent ownership.

**See also**: [Support library](#S-gsl).


<a name="Rf-const-T-ref"></a>
### F.20: Use a `const T&` parameter for a large object

**Reason**: Copying large objects can be expensive. A `const T&` is always cheap and protects the caller from unintended modification.

**Example**:

	void fct(const string& s);	// OK: pass by const reference; always cheap

	void fct2(string s);		// bad: potentially expensive

**Exception**: Sinks (that is, a function that eventually destroys an object or passes it along to another sink), may benefit ???

**Note**: A reference may be assumed to refer to a valid object (language rule).
There in no (legitimate) "null reference."
If you need the notion of an optional value, use a pointer, `std::optional`, or a special value used to denote "no value."

**Enforcement**:

* (Simple) ((Foundation)) Warn when a parameter being passed by value has a size greater than `4*sizeof(int)`.
Suggest using a `const` reference instead.


<a name="Rf-T"></a>
### F.21: Use a `T` parameter for a small object

**Reason**: Nothing beats the simplicity and safety of copying.
For small objects (up to two or three words) it is also faster than alternatives.

**Example**:

	void fct(int x);		// OK: Unbeatable

	void fct2(const int& x);	// bad: overhead on access in fct2()

	void fct(int& x);		// OK, but means something else; use only for an "out parameter"

**Enforcement**:

* (Simple) ((Foundation)) Warn when a `const` parameter being passed by reference has a size less than `3*sizeof(int)`. Suggest passing by value instead.


<a name="Rf-T-ref"></a>
### F.22: Use a `T&` for an in-out-parameter

**Reason**: A called function can write to a non-`const` reference argument, so assume that it does.

**Example**:

	void update(Record& r);	// assume that update writes to r
	
**Note**: A `T&` argument can pass information into a function as well as well as out of it.
Thus `T&` could be and in-out-parameter. That can in itself be a problem and a source of errors:

	void f(string& s)
	{
		s = "New York";	// non-obvious error
	}
	
	string g()
	{
		string buffer = ".................................";
		f(buffer);
		// ...
	}
	
Here, the writer of `g()` is supplying a buffer for `f()` to fill,
but `f()` simply replaces it (at a somewhat higher cost than a simple copy of the characters).
If the writer of `g()` makes an assumption about the size of `buffer` a bad logic error can happen.

**Enforcement**:

* (Moderate) ((Foundation)) Warn about functions with non-`const` reference arguments that do *not* write to them.
* Flag functions that take a `T&` and replace the `T` referred to, rather what the contents of that `T`


<a name="Rf-T-return-out"></a>
### F.23: Use `T&` for an out-parameter that is expensive to move (only)

**Reason**: A return value is harder to miss and harder to misuse than a `T&` (an in-out parameter); [see also](#Rf-T-return); [see also](#Rf-T-multi).

**Example**:

	struct Package {
		char header[16];
		char load[2024-16];
	};
	
	Package fill();			// Bad: large return value
	void fill(Package&);	// OK
	
	int val();				// OK
	val(int&);				// Bad: Is val reading its argument

**Enforcement**: Hard to choose a cutover value for the size of the value returned.


<a name="Rf-pass-ref-ref"></a>
### F.24: Use a `TP&&` parameter when forwarding (only)

**Reason**: When `TP` is a template type parameter, `TP&&` is a forwarding reference -- it both *ignores* and *preserves* const-ness and rvalue-ness. Therefore any code that uses a `T&&` is implicitly declaring that it itself doesn't care about the variable's const-ness and rvalue-ness (because it is ignored), but that intends to pass the value onward to other code that does care about const-ness and rvalue-ness (because it is preserved). When used as a parameter `TP&&` is safe because any temporary objects passed from the caller will live for the duration of the function call. A parameter of type `TP&&` should essentially always be passed onward via `std::forward` in the body of the function.

**Example**:

	template <class F, class... Args>
    inline auto invoke(F&& f, Args&&... args) {
        return forward<F>(f)(forward<Args>(args)...);
    }

**Enforcement**: Flag a function that takes a `TP&&` parameter (where `TP` is a template type parameter name) and uses it without `std::forward`.


<a name ="Rf-pass-ref-move"></a>
### F.25: Use a `T&&` parameter together with `move` for rare optimization opportunities

**Reason**: Moving from an object leaves an object in its moved-from state behind.
In general, moved-from objects are dangerous. The only guaranteed operation is destruction (more generally, member functions without preconditions).
The standard library additionally requires that a moved-from object can be assigned to.
If you have performance justification to optimize for rvalues, overload on `&&` and then `move` from the parameter ([example of such overloading](#)).

**Example**:

	void somefct(string&&);
	
	void user()
	{
		string s = "this is going to be fun!";
		// ...
		somefct(std::move(s));		// we don't need s any more, give it to somefct()
		//
		cout << s << '\n';			// Oops! What happens here?
	}

**Enforcement**:

* Flag all `X&&` parameters (where `X` is not a template type parameter name) and uses it without `std::move`.
* Flag access to moved-from objects


<a name="Rf-unique_ptr"></a>
### F.26: Use a `unique_ptr<T>` to transfer ownership where a pointer is needed

**Reason**: Using `unique_ptr` is the cheapest way to pass a pointer safely.

**Example**:

	unique_ptr<Shape> get_shape(istream& is)	// assemble shape from input stream
	{
		auto kind = read_header(is); // read header and identify the next shape on input
		switch (kind) {
		case kCicle:
			return make_unique<Circle>(is);
		case kTriangle:
			return make_unique<Triangle>(is);
		// ...
	}

**Note**: You need to pass a pointer rather than an object if what you are transferring is an object from a class hierarchy that is to be used through an interface (base class).

**Enforcement**: (Simple) Warn if a function returns a locally-allocated raw pointer. Suggest using either `unique_ptr` or `shared_ptr` instead.


<a name="Rf-shared_ptr"></a>
### F.27: Use a `shared_ptr<T>` to share ownership

**Reason**: Using `std::shared_ptr` is the standard way to represent shared ownership. That is, the last owner deletes the object.

**Example**:

	shared_ptr<Image> im { read_image(somewhere); };
	
	std::thread t0 {shade,args0,top_left,im};
	std::thread t1 {shade,args1,top_right,im};
	std::thread t2 {shade,args2,bottom_left,im};
	std::thread t3 {shade,args3,bottom_right,im};
	
	// detach treads
	// last thread to finish deletes the image


**Note**: Prefer a `unique_ptr` over a `shared_ptr` if there is never more than one owner at a time.
`shared_ptr` is for shared ownership.

**Alternative**: Have a single object own the shared object (e.g. a scoped object) and destroy that (preferably implicitly) when all users have completd.

**Enforcement**: (Not enforceable) This is a too complex pattern to reliably detect.


<a name="Rf-T-return"></a>
### F.40: Prefer return values to out-parameters

**Reason**: It's self-documenting. A `&` parameter could be either in/out or out-only.

**Example**:

	void incr(int&);
	int incr(int);
	
	int i = 0;
	incr(i);
	i = incr(i);

**Enforcement**: Flag non-const reference parameters that are not read before being written to and are a type that could be cheaply returned.


<a name="Rf-T-multi"></a>
### F.41: Prefer to return tuples to multiple out-parameters

**Reason**: A return value is self-documenting as an "output-only" value.
And yes, C++ does have multiple return values, by convention of using a `tuple`, with the extra convenience of `tie` at the call site.

**Example**:

    int f( const string& input, /*output only*/ string& output_data ) { // BAD: output-only parameter documented in a comment
	    // ...
		output_data = something();
		return status;
	}

    tuple<int,string> f( const string& input ) { // GOOD: self-documenting
	    // ...
		return make_tuple(something(), status);
    }

In fact, C++98's standard library already used this convenient feature, because a `pair` is like a two-element `tuple`.
For example, given a `set<string> myset`, consider:

    // C++98
	result = myset.insert( "Hello" );
	if (result.second) do_something_with( result.first );    // workaround
	
With C++11 we can write this, putting the results directly in existing local variables:

	Sometype iter;                                          // default initialize if we haven't already
	Someothertype success;                                  // used these variables for some other purpose

	tie( iter, success ) = myset.insert( "Hello" );         // normal return value
	if (success) do_something_with( iter );

**Exception**: For types like `string` and `vector` that carry additional capacity, it can sometimes be useful to treat it as in/out instead by using the "caller-allocated out" pattern, which is to pass an output-only object by reference to non-`const` so that when the callee writes to it the object can reuse any capacity or other resources that it already contains. This technique can dramatically reduce the number of allocations in a loop that repeatedly calls other functions to get string values, by using a single string object for the entire loop.

**Note**: In some cases it may be useful to return a specific, user-defined `Value_or_error` type along the lines of `variant<T,error_code>`,
rather than using the generic `tuple`.

**Enforcement**:

    * Output parameters should be replaced by return values.
	An output parameter is one that the function writes to, invokes a non-`const` member function, or passes on as a non-`const`.


<a name="Rf-return-ptr"></a>
### F.42: Return a `T*` to indicate a position (only)

**Reason**: That's what pointers are good for.
Returning a `T*` to transfer ownership is a misuse.

**Note**: Do not return a pointer to something that is not in the caller's scope.

**Example**:

		Node* find(Node* t, const string& s)	// find s in a binary tree of Nodes
		{
			if (t == nullptr || t->name == s) return t;
			if (auto p = find(t->left,s)) return p;
			if (auto p = find(t->right,s)) return p;
			return nullptr;
		}

If it isn't the `nullptr`, the pointer returned by `find` indicates a `Node` holding `s`.
Importantly, that does not imply a transfer of ownership of the pointed-to object to the caller.

**Note**: Positions can also be transferred by iterators, indices, and references.

**Example, bad**:

	int* f()
	{
		int x = 7;
		// ...
		return &x;		// Bad: returns pointer to object that is about to be destroyed
	}

This applies to references as well:

	int& f()
	{
		int x = 7;
		// ...
		return x;	// Bad: returns reference to object that is about to be destroyed
	}

**See also**: [discussion of dangling pointer prevention](#???).

**Enforcement**: A slightly different variant of the problem is placing pointers in a container that outlives the objects pointed to.

* Compilers tend to catch return of reference to locals and could in many cases catch return of pointers to locals.
* Static analysis can catch most (all?) common patterns of the use of pointers indicating positions (thus eliminating dangling pointers)


<a name="Rf-dangle"></a>
### F.43: Never (directly or indirectly) return a pointer to a local object

**Reason**: To avoid the crashes and data corruption that can result from the use of such a dangling pointer.

**Example**, bad: After the return from a function its local objects no longer exist:

	int* f()
	{
		int fx = 9;
		return &fx;	// BAD
	}
	
	void g(int* p)	// looks innocent enough
	{
		int gx;
		cout << "*p == " << *p << '\n';
		*p = 999;
		cout << "gx == " << gx << '\n';
	}
		
	void h()
	{
		int* p = f();
		int z = *p;		// read from abandoned stack frame (bad)
		g(p);			// pass pointer to abandoned stack frame to function (bad)
		
	}
	
Here on one popular implementation I got the output

	*p == 9
	cx == 999
	
I expected that because the call of `g()` reuses the stack space abandoned by the call of `f()` so `*p` refers to the space now occupied by `gx`.

Imagine what would happen if `fx` and `gx` were of different types.
Imagine what would happen if `fx` or `gx` was a type with an invariant.
Imagine what would happen if more that dangling pointer was passed around among a larger set of functions.
Imagine what a cracker could do with that dangling pointer.

Fortunately, most (all?) modern compilers catch and warn against this simple case.

**Note**: you can construct similar examples using references.

**Note**: This applies only to non-`static` local variables.
All `static` variables are (as their name indicates) statically allocated, so that pointers to them cannot dangle.

**Example**, bad: Not all examples of leaking a pointer to a local variable are that obvious:

	int* glob;		// global variables are bad in so many ways

	template<class T>
	void steal(T x)
	{
		glob = x();	// BAD
	}

	void f()
	{
		int i = 99;
		steal([&] { return &i; });
	}

	int main()
	{
		f();
		cout << *glob << '\n';
	}
	
Here I managed to read the location abandoned by the call of `f`.
The pointer stored in `glob` could be used much later and cause trouble in unpredictable ways.

**Note**: The address of a local variable can be "returned"/leaked by a return statement,
by a `T&` out-parameter, as a member of a returned object, as an element of a returned array, and more.

**Note**: Similar examples can be constructed "leaking" a pointer from an inner scope to an outer one;
such examples are handled equivalently to leaks of pointers out of a function.

**See also**: Another way of getting dangling pointers is [pointer invalidation](#???).
It can be detected/prevented with similar techniques.

**Enforcement**: Preventable through static analysis.


<a name="Rf-return-ref"></a>
### F.44: Return a `T&` when "returning no object" isn't an option

**Reason**: The language guarantees that a `T&` refers to an object, so that testing for `nullptr` isn't necessary.

**See also**: The return of a reference must not imply transfer of ownership:
[discussion of dangling pointer prevention](#???) and [discussion of ownership](#???).

**Example**:

	???

**Enforcement**: ???


<a name="Rf-return-ref-ref"></a>
### F.45: Don't return a `T&&`

**Reason**: It's asking to return a reference to a destroyed temporary object. A `&&` is a magnet for temporary objects. This is fine when the reference to the temporary is being passed "downward" to a callee, because the temporary is guaranteed to outlive the function call. (See [F.24](#Rf-pass-ref-ref) and [F.25](#Rf-pass-ref-move).) However, it's not fine when passing such a reference "upward" to a larger caller scope. See also [F54](#Rf-local-ref-ref).

For passthrough functions that pass in parameters (by ordinary reference or by perfect forwarding) and want to return values, use simple `auto` return type deduction (not `auto&&`).

**Example; bad**: If `F` returns by value, this function returns a reference to a temporary.

	template<class F>
	auto&& wrapper(F f) {
		log_call(typeid(f)); // or whatever instrumentation
	    return f();
	}

**Example; good**: Better:
	
	template<class F>
	auto wrapper(F f) {
		log_call(typeid(f)); // or whatever instrumentation
	    return f();
	}

**Exception**: `std::move` and `std::forward` do return `&&`, but they are just casts -- used by convention only in expression contexts where a reference to a temporary object is passed along within the same expression before the temporary is destroyed. We don't know of any other good examples of returning `&&`.

**Enforcement**: Flag any use of `&&` as a return type, except in `std::move` and `std::forward`.


<a name="Rf-capture-vs-overload"></a>
### F.50: Use a lambda when a function won't do (to capture local variables, or to write a local function)

**Reason**: Functions can't capture local variables or be declared at local scope; if you need those things, prefer a lambda where possible, and a handwritten function object where not. On the other hand, lambdas and function objects don't overload; if you need to overload, prefer a function (the workarounds to make lambdas overload are ornate). If either will work, prefer writing a function; use the simplest tool necessary.

**Example**:

	// writing a function that should only take an int or a string -- overloading is natural
	void f(int);
	void f(const string&);
	
	// writing a function object that needs to capture local state and appear
	// at statement or expression scope -- a lambda is natural
	vector<work> v = lots_of_work();
	for(int tasknum = 0; tasknum < max; ++tasknum) {
	    pool.run([=, &v]{
			/*
			...
			... process 1/max-th of v, the tasknum-th chunk
			...
			*/
		});
	}
	pool.join();

**Exception**: Generic lambdas offer a concise way to write function templates and so can be useful even when a normal function template would do equally well with a little more syntax. This advantage will probably disappear in the future once all functions gain the ability to have Concept parameters.

**Enforcement**:

    * Warn on use of a named non-generic lambda (e.g., `auto x = [](int i){ /*...*/; };`) that captures nothing and appears at global scope. Write an ordinary function instead.



<a name="Rf-default-args"></a>
### F.51: Prefer overloading over default arguments for virtual functions
??? possibly other situations?

**Reason**: Virtual function overrides do not inherit default arguments, leading to surprises.

**Example; bad**:

	class base {
	public:
		virtual int multiply(int value, int factor = 2) = 0;
	};

	class derived : public base {
	public:
		override int multiply(int value, int factor = 10);
	};
	
	derived d;
	base& b = d;
	
	b.multiply(10);	// these two calls will call the same function but
	d.multiply(10); // with different arguments and so different results

**Enforcement**: Flag all uses of default arguments in virtual functions.


<a name="Rf-reference-capture"></a>
### F.52: Prefer capturing by reference in lambdas that will be used locally, including passed to algorithms

**Reason**: For efficiency and correctness, you nearly always want to capture by reference when using the lambda locally. This includes when writing or calling parallel algorithms that are local because they join before returning.

**Example**: This is a simple three-stage parallel pipeline. Each `stage` object encapsulates a worker thread and a queue, has a `process` function to enqueue work, and in its destructor automatically blocks waiting for the queue to empty before ending the thread.

	void send_packets( buffers& bufs ) {
	    stage encryptor  ([] (buffer& b){ encrypt(b); });
	    stage compressor ([&](buffer& b){ compress(b); encryptor.process(b); });
	    stage decorator  ([&](buffer& b){ decorate(b); compressor.process(b); });
	    for (auto& b : bufs) { decorator.process(b); }
	} // automatically blocks waiting for pipeline to finish

**Enforcement**: ???


<a name="Rf-value-capture"></a>
### F.53: Avoid capturing by reference in lambdas that will be used nonlocally, including returned, stored on the heap, or passed to another thread

**Reason**: Pointers and references to locals shouldn't outlive their scope. Lambdas that capture by reference are just another place to store a reference to a local object, and shouldn't do so if they (or a copy) outlive the scope.

**Example**:

	{
		// ...
		
		// a, b, c are local variables
		background_thread.queue_work([=]{ process(a,b,c); });	// want copies of a, b, and c
	}

**Enforcement**: ???



<a name="S-class"></a>
# C: Classes and Class Hierarchies

A class is a user-defined type, for which a programmer can define the representation, operations, and  interfaces.
Class hierarchies are used to organize related classes into hierarchical structures.

Class rule summary:

* [C.1: Organize related data into structures (`struct`s or `class`es)](#Rc-org)
* [C.2: Use `class` if the class has an invariant; use `struct` if the data members can vary independently](#Rc-struct)
* [C.3: Represent the distinction between an interface and an implementation using a class](#Rc-interface)
* [C.4: Make a function a member only if it needs direct access to the representation of a class](#Rc-member)
* [C.5: Place helper functions in the same namespace as the class they support](#Rc-helper)
* [C.6: Declare a member function that does not modify the state of its object `const`](#Rc-const)

Subsections:

* [C.concrete: Concrete types](#SS-concrete)
* [C.ctor: Constructors, assignments, and destructors](#SS-ctor)
* [C.con: Containers and other resource handles](#SS-containers)
* [C.lambdas: Function objects and lambdas](#SS-lambdas)
* [C.hier: Class hierarchies (OOP)](#SS-hier)
* [C.over: Overloading and overloaded operators](#SS-overload)
* [C.union: Unions](#SS-union)


<a name="Rc-org"></a>
### C.1: Organize related data into structures (`struct`s or `class`es)

**Reason**: Ease of comprehension. If data is related (for fundamental reasons), that fact should be reflected in code.

**Example**:

	void draw(int x, int y, int x2, int y2);	// BAD: unnecessary implicit relationships
	void draw(Point from, Point to)				// better

**Note**: A simple class without virtual functions implies no space or time overhead.

**Note**: From a language perspective `class` and `struct` differ only in the default visibility of their members.

**Enforcement**: Probably impossible. Maybe a heuristic looking for date items used together is possible.


<a name="Rc-struct"></a>
### C.2: Use `class` if the class has an invariant; use `struct` if the data members can vary independently

**Reason**: Ease of comprehension. The use of `class` alerts the programmer to the need for an invariant

**Note**: An invariant is logical condition for the members of an object that a constructor must establish for the public member functions to assume. After the invariant is established (typically by a constructor) every member function can be called for the object. An invariant can be stated informally (e.g., in a comment) or more formally using `Expects`.

**Example**:

	struct Pair {	// the members can vary independently
		string name;
		int volume;
	};

but

	class Date {
	private:
		int y;
		Month m;
		char d;		// day
	public:
		Date(int yy, Month mm, char dd);		// validate that {yy,mm,dd} is a valid date and initialize
		// ...
	};

**Enforcement**: Look for `struct`s with all data private and `class`es with public members.


<a name="Rc-interface"></a>
### C.3: Represent the distinction between an interface and an implementation using a class

**Reason**: an explicit distinction between interface and implementation improves readability and simplifies maintenance.

**Example**:

	class Date {
		// ... some representation ...
	public:
		Date();
		Date(int yy, Month mm, char dd);		// validate that {yy,mm,dd} is a valid date and initialize

		int day() const;
		Month month() const;
		// ...
	};

For example, we can now change the representation of a `Date` without affecting its users (recompilation is likely, though).

**Note**: Using a class in this way to represent the distinction between interface and implementation is of course not the only way.
For example, we can use a set of declarations of freestanding functions in a namespace,
an abstract base class,
or a template function with concepts to represent an interface.
The most important issue is to explicitly distinguish between an interface and its implementation "details."
Ideally, and typically, an interface is far more stable than its implementation(s).

**Enforcement**: ???
		

<a name="Rc-member"></a>
### C.4: Make a function a member only if it needs direct access to the representation of a class

**Reason**: Less coupling than with member functions, fewer functions that can cause trouble by modifying object state, reduces the number of functions that needs to be modified after a change in representation.

**Example**:

	class Date {
		// ... relatively small interface ...
	};

	// helper functions:
	Date next_weekday(Date);
	bool operator==(Date, Date);

The "helper functions" have no need for direct access to the representation of a `Date`.

**Note**: This rule becomes even better if C++17 gets "uniform function call." ???

**Enforcement**: Look for member function that do not touch data members directly.
The snag is that many member functions that do not need to touch data members directly do.


<a name="Rc-helper"></a>
### C.5: Place helper functions in the same namespace as the class they support

**Reason**: A helper function is a function (usually supplied by the writer of a class) that does not need direct access to the representation of the class,
yet is seen as part of the useful interface to the class.
Placing them in the same namespace as the class makes their relationship to the class obvious and allows them to be found by argument dependent lookup.

**Example**:

	namespace Chrono { // here we keep time-related services
	
		class Time { /* ... */ };
		class Date { /* ... */ };
		
		// helper functions:
		bool operator==(Date,Date);
		Date next_weekday(Date);
		// ...
	}
	
**Enforcement**:

* Flag global functions taking argument types from a single namespace.

		
<a name="Rc-const"></a>
### C.6: Declare a member function that does not modify the state of its object `const`

**Reason**: More precise statement of design intent, better readability, more errors caught by the compiler, more optimization opportunities.

**Example**:

	int Date::day() const { return d; }

**Note**: [Do not cast away `const`](#Res-casts-const).

**Enforcement**: Flag non-`const` member functions that do not write to their objects


<a name="SS-concrete"></a>
## C.concrete: Concrete types

One ideal for a class is to be a regular type.
That means roughly "behaves like an `int`." A concrete type is the simplest kind of class.
A value of regular type can be copied and the result of a copy is an independent object with the same value as the original.
If a concrete type has both `=` and `==`, `a=b` should result in `a==b` being `true`.
Concrete classes without assignment and equality can be defined, but they are (and should be) rare.
The C++ built-in types are regular, and so are standard-library classes, such as `string`, `vector`, and `map`.
Concrete types are also often referred to as value types to distinguish them from types uses as part of a hierarchy.

Concrete type rule summary:

* [C.10: Prefer a concrete type over more complicated classes](#Rc-concrete)
* [C.11: Make a concrete types regular](#Rc-regular)


<a name="Rc-concrete"></a>
### C.10 Prefer a concrete type over more complicated classes

**Reason**: A concrete type is fundamentally simpler than a hierarchy:
easier to design, easier to implement, easier to use, easier to reason about, smaller, and faster.
You need a reason (use cases) for using a hierarchy.

**Example**

	class Point1 {
		int x, y;
		//  ... operations ...
		// .. no virtual functions ...
	};

	class Point2 {
		int x, y;
		// ... operations, some virtual ...
		virtual ~Point2();
	};

	void use()
	{
		Point1 p11 { 1,2};	// make an object on the stack
		Point1 p12 {p11};	// a copy

		auto p21 = make_unique<Point2>(1,2);	// make an object on the free store
		auto p22 = p21.clone();					// make a copy

		// ...
	}

If a class can be part of a hierarchy, we (in real code if not necessarily in small examples) must manipulate its objects through pointers or references.
That implies more memory overhead, more allocations and deallocations, and more run-time overhead to perform the resulting indirections.

**Note**: Concrete types can be stack allocated and be members of other classes.

**Note**: The use of indirection is fundamental for run-time polymorphic interfaces.
The allocation/deallocation overhead is not (that's just the most common case).
We can use a base class as the interface of a scoped object of a derived class.
This is done where dynamic allocation is prohibited (e.g. hard real-time) and to provide a stable interface to some kinds of plug-ins.

**Enforcement**: ???


<a name="Rc-regular"></a>
### C.11: Make a concrete types regular

**Reason**: Regular types are easier to understand and reason about than types that are not regular (irregularities requires extra effort to understand and use).

**Example**:

	struct Bundle {
		string name;
		vector<Record> vr;
	};

	bool operator==(const Bundle& a, const Bundle& b) { return a.name==b.name && a.vr==b.vr; }

	Bundle b1 { "my bundle", {r1,r2,r3}};
	Bundle b2 = b1;
	if (!(b1==b2)) error("impossible!");
	b2.name = "the other bundle";
	if (b1==b2) error("No!");

In particular, if a concrete type has an assignment also give it an equals operator so that `a=b` implies `a==b`.

**Enforcement**: ???


<a name="SS-ctor"></a>
## C.ctor: Constructors, assignments, and destructors

These functions control the lifecycle of objects: creation, copy, move, and destruction.
Define constructors to guarantee and simplify initialization of classes.

These are *default operations*:

* a default constructor: `X()`
* a copy constructor: `X(const X&)`
* a copy assignment: `operator=(const X&)`
* a move constructor: `X(X&&)`
* a move assignment: `operator=(X&&)`
* a destructor: `~X()`

By default, the compiler defines each of these operations if it is used, but the default can be suppressed.

The default operations are a set of related operations that together implement the lifecycle semantics of an object.
By default, C++ treats classes as value-like types, but not all types are value-like.

Set of default operations rules:

* [C.20: If you can avoid defining any default operations, do](#Rc-zero)
* [C.21: If you define or `=delete` any default operation, define or `=delete` them all](#Rc-five)
* [C.22: Make default operations consistent](#Rc-matched)

Destructor rules:

* [C.30: Define a destructor if a class needs an explicit action at object destruction](#Rc-dtor)
* [C.31: All resources acquired by a class must be released by the class's destructor](#Rc-dtor-release)
* [C.32: If a class has a raw  pointer (`T*`) or reference (`T&`), consider whether it might be owning](#Rc-dtor-ptr)
* [C.33: If a class has an owning pointer member, define or `=delete` a destructor](#Rc-dtor-ptr2)
* [C.34: If a class has an owning reference member, define or `=delete` a destructor](#Rc-dtor-ref)
* [C.35: A base class with a virtual function needs a virtual destructor](#Rc-dtor-virtual)
* [C.36: A destructor may not fail](#Rc-dtor-fail)
* [C.37: Make destructors `noexcept`](#Rc-dtor-noexcept)

Constructor rules:

* [C.40: Define a constructor if a class has an invariant](#Rc-ctor)
* [C.41: A constructor should create a fully initialized object](#Rc-complete)
* [C.42: If a constructor cannot construct a valid object, throw an exception](#Rc-throw)
* [C.43: Give a class a default constructor](#Rc-default0)
* [C.44: Prefer default constructors to be simple and non-throwing](#Rc-default00)
* [C.45: Don't define a default constructor that only initializes data members; use member initializers instead](#Rc-default)
* [C.46: By default, declare single-argument constructors `explicit`](#Rc-explicit)
* [C.47: Define and initialize member variables in the order of member declaration](#Rc-order)
* [C.48: Prefer in-class initializers to member initializers in constructors for constant initializers](#Rc-in-class-initializer)
* [C.49: Prefer initialization to assignment in constructors](#Rc-initialize)
* [C.50: Use a factory function if you need "virtual behavior" during initialization](#Rc-factory)
* [C.51: Use delegating constructors to represent common actions for all constructors of a class](#Rc-delegating)
* [C.52: Use inheriting constructors to import constructors into a derived class that does not need further explicit initialization](#Rc-inheriting)

Copy and move rules:

* [C.60: Make copy assignment non-`virtual`, take the parameter by `const&`, and return by non-`const&`](#Rc-copy-assignment)
* [C.61: A copy operation should copy](#Rc-copy-semantic)
* [C.62: Make copy assignment safe for self-assignment](#Rc-move-self)
* [C.63: Make move assignment non-`virtual`, take the parameter by `&&`, and return by non-`const&`](#Rc-move-assignment)
* [C.64: A move operation should move and leave its source in a valid state](#Rc-move-semantic)
* [C.65: Make move assignment safe for self-assignment](#Rc-copy-self)
* [C.66: Make move operations `noexcept`](#Rc-move-noexcept)
* [C.67: A base class should suppress copying, and provide a virtual `clone` instead if "copying" is desired](#Rc-copy-virtual)

Other default operations rules:

* [C.80: Use `=default` if you have to be explicit about using the default semantics](#Rc-=default)
* [C.81: Use `=delete` when you want to disable default behavior (without wanting an alternative)](#Rc-=delete)
* [C.82: Don't call virtual functions in constructors and destructors](#Rc-ctor-virtual)
* [C.83: For value-like types, consider providing a `noexcept` swap function](#Rc-swap)
* [C.84: A `swap` may not fail](#Rc-swap-fail)
* [C.85: Make `swap` `noexcept`](#Rc-swap-noexcept)
* [C.86: Make `==` symmetric with respect of operand types and `noexcept`](#Rc-eq)
* [C.87: Beware of `==` on base classes](#Rc-eq-base)
* [C.88: Make `<` symmetric with respect of operand types and `noexcept`](#Rc-lt)
* [C.89: Make a `hash` `noexcept`](#Rc-hash)


<a name="SS-defop"></a>
## C.defop: Default Operations

By default, the language supply the default operations with their default semantics.
However, a programmer can disalble or replace these defaults.


<a name="Rc-zero"></a>
### C.20: If you can avoid defining default operations, do

**Reason**: It's the simplest and gives the cleanest semantics.

**Example**:

	struct Named_map {
	public:
		// ... no default operations declared ...
	private:
		string name;
		map<int,int> rep;
	};

	Named_map nm;		// default construct
	Named_map nm2 {nm};	// copy construct

Since `std::map` and `string` have all the special functions, not further work is needed.

**Note**: This is known as "the rule of zero".

**Enforcement**: (Not enforceable) While not enforceable, a good static analyzer can detect patterns that indicate a possible improvement to meet this rule.
 For example, a class with a (pointer,size) pair of member and a destructor that `delete`s the pointer could probably be converted to a `vector`.


<a name="Rc-five"></a>
### C.21: If you define or `=delete` any default operation, define or `=delete` them all

**Reason**: The semantics of the special functions are closely related, so it one needs to be non-default, the odds are that other need modification.

**Example, bad**:

	struct M2 {		// bad: incomplete set of default operations
	public:
		// ...
		// ... no copy or move operations ...
		~M2() { delete[] rep; }
	private:
		pair<int,int>* rep;  // zero-terminated set of pairs
	};

	void use()
	{
		M2 x;
		M2 y;
		// ...
		x = y;	// the default assignment
		// ...
	}

Given that "special attention" was needed for the destructor (here, to deallocate), the likelihood that copy and move assignment (both will implicitly destroy an object) are correct is low (here, we would get double deletion).

**Note**: This is known as "the rule of five" or "the rule of six", depending on whether you count the default constructor.

**Note**: If you want a default implementation of a default operation (while defining another), write `=default` to show you're doing so intentionally for that function.
If you don't want a default operation, suppress it with `=delete`.

**Note:** Compilers enforce much of this rule and ideally warn about any violation.

**Note**: Relying on an implicitly generated copy operation in a class with a destructor is deprecated.

**Enforcement**: (Simple) A class should have a declaration (even a `=delete` one) for either all or none of the special functions.


<a name="Rc-matched"></a>
### C.22: Make default operations consistent

**Reason**: The default operations are conceptually a matched set. Their semantics is interrelated.
Users will be surprised if copy/move construction and copy/move assignment do logically different things. Users will be surprised if constructors and destructors do not provide a consistent view of resource management. Users will be surprised if copy and move doesn't reflect the way constructors and destructors work.

**Example; bad**:

	class Silly { 		// BAD: Inconsistent copy operations
		class Impl {
			// ...
		};
        shared_ptr<Impl> p;
	public:
		Silly(const Silly& a) : p{a.p} { *p = *a.p; }   // deep copy
		Silly& operator=(const Silly& a) { p = a.p; }   // shallow copy
		// ...
	};

These operations disagree about copy semantics. This will lead to confusion and bugs.

**Enforcement**:

* (Complex) A copy/move constructor and the corresponding copy/move assignment operator should write to the same member variables at the same level of dereference.
* (Complex) Any member variables written in a copy/move constructor should also be initialized by all other constructors.
* (Complex) If a copy/move constructor performs a deep copy of a member variable, then the destructor should modify the member variable.
* (Complex) If a destructor is modifying a member variable, that member variable should be written in any copy/move constructors or assignment operators.



<a name="SS-dtor"></a>
## C.dtor: Destructors

Does this class need a destructor is a surprisingly powerful design question.
For most classes the answer is "no" either because the class holds no resources or because destruction is handled by [the rule of zero](#Rc-zero);
that is, its members can take care of themselves as concerns destruction.
If the answer is "yes", much of the design of the class follows (see [the rule of five](#Rc-five).


<a name="Rc-dtor"></a>
### C.30: Define a destructor if a class needs an explicit action at object destruction

**Reason**: A destructor is implicitly invoked at the end of an objects lifetime.
If the default destructor is sufficient, use it.
Only if you need code that is not simply destructors of members executed, define a non-default destructor.

**Example**:

	template<typename A>
	struct Final_action {	// slightly simplified
		A act;
		Final_action(F a) :act{a} {}
		~Final_action() { act(); }
	};

	template<typename A>
	Final_action<A> finally(A act)	// deduce action type
	{
		return Final_action<A>{a};
	}

	void test()
	{
		auto act = finally([]{ cout<<"Exit test\n"; });	// establish exit action
		// ...
		if (something) return;	// act done here
		// ...
	} // act done here

The whole purpose of `Final_action` is to get a piece of code (usually a lambda) executed upon destruction.

**Note**: There are two general categories of classes that need a user-defined destructor:

* A class with a resource that is not already represented as a class with a destructor, e.g., a `vector` or a transaction class.
* A class that exists primarily to execute an action upon destruction, such as a tracer or `Final_action`.

**Example, bad**:

	class Foo {		// bad; use the default destructor
	public:
		// ...
		~Foo() { s=""; i=0; vi.clear(); }  // clean up
	private:
		string s;
		int i;
		vector<int> vi;
	}

The default destructor does it better, more efficiently, and can't get it wrong.

**Note**: If the default destructor is needed, but its generation has been suppressed (e.g., by defining a move constructor), use `=default`.

**Enforcement**: Look for likely "implicit resources", such as pointers and references. Look for classes with destructors even though all their data members have destructors.


<a name="Rc-dtor-release"></a>
### C.31: All resources acquired by a class must be released by the class's destructor

**Reason**: Prevention of resource leaks, especially in error cases.

**Note**: For resources represented as classes with a complete set of default operations, this happens automatically.

**Example**:

	class X {
		ifstream f;	// may own a file
		// ... no default operations defined or =deleted ...
	};

`X`'s `ifstream` implicitly closes any file it may have open upon destruction of its `X`.

**Example; bad**:

	class X2 {	// bad
		FILE* f;	// may own a file
		// ... no default operations defined or =deleted ...
	};

`X2` may leak a file handle.

**Note**: What about a sockets that won't close? A destructor, close, or cleanup operation [should never fail](#Rc-dtor-fail).
If it does nevertheless, we have a problem that has no really good solution.
For starters, the writer of a destructor does not know why the destructor is called and cannot "refuse to act" by throwing an exception.
See [discussion](#Sd-never-fail).
To make the problem worse, many "close/release" operations are not retryable.
Many have tried to solve this problem, but no general solution is known.
If at all possible, consider failure to close/cleanup a fundamental design error and terminate.

**Note**: A class can hold pointers and references to objects that it does not own.
Obviously, such objects should not be `delete`d by the class's destructor.
For example:

	Preprocessor pp { /* ... */ };
	Parser p { pp, /* ... */ };
	Type_checker tc { p, /* ... */ };

Here `p` refers to `pp` but does not own it.

**Enforcement**:

* (Simple) If a class has pointer or reference member variables that are owners
(e.g., deemed owners by using `GSL::owner`), then they should be referenced in its destructor.
* (Hard) Determine if pointer or reference member variables are owners when there is no explicit statement of ownership
(e.g., look into the constructors).


<a name="Rc-dtor-ptr"></a>
### C.32: If a class has a raw  pointer (`T*`) or reference (`T&`), consider whether it might be owning

**Reason**: There is a lot of code that is non-specific about ownership.

**Example**:

	???

**Note**: If the `T*` or `T&` is owning, mark it `owning`. If the `T*` is not owning, consider marking it `ptr`.
This will aide documentation and analysis.

**Enforcement**: Look at the initialization of raw member pointers and member references and see if an allocation is used.


<a name="Rc-dtor-ptr2"></a>
### C.33: If a class has an owning pointer member, define a destructor

**Reason**: An owned object must be `deleted` upon destruction of the object that owns it.

**Example**: A pointer member may represent a resource.
[A `T*` should not do so](#Rr-ptr), but in older code, that's common.
Consider a `T*` a possible owner and therefore suspect.

	template<typename T>
	class Smart_ptr {
		T* p;	// BAD: vague about ownership of *p
		// ...
	public:
		// ... no user-defined default operations ...
	};

	void use(Smart_ptr<int> p1)
	{
		auto p2 = p1;	// error: p2.p leaked (if not nullptr and not owned by some other code)
	}

Note that if you define a destructor, you must define or delete [all default operations](#Rc-five):

	template<typename T>
	class Smart_ptr2 {
		T* p;	// BAD: vague about ownership of *p
		// ...
	public:
		// ... no user-defined copy operations ...
		~Smart_ptr2() { delete p; }		// p is an owner!
	};

	void use(Smart_ptr<int> p1)
	{
		auto p2 = p1;	// error: double deletion
	}

The default copy operation will just copy the `p1.p` into `p2.p` leading to a double destruction of `p1.p`. Be explicit about ownership:

	template<typename T>
	class Smart_ptr3 {
		owner<T>* p;	// OK: explicit about ownership of *p
		// ...
	public:
		// ...
		// ... copy and move operations ...
		~Smart_ptr3() { delete p; }
	};

	void use(Smart_ptr3<int> p1)
	{
		auto p2 = p1;	// error: double deletion
	}


 **Note**: Often the simplest way to get a destructor is to replace the pointer with a smart pointer (e.g., `std::unique_ptr`)
 and let the compiler arrange for proper destruction to be done implicitly.
 
**Note**: Why not just require all owning pointers to be "smart pointers"?
 That would sometimes require non-trivial code changes and may affect ABIs.

**Enforcement**:

* A class with a pointer data member is suspect.
* A class with an `owner<T>` should define its default operations.


<a name="Rc-dtor-ref"></a>
### C.34: If a class has an owning reference member, define a destructor

**Reason**: A reference member may represent a resource.
It should not do so, but in older code, that's common.
See [pointer members and destructors](#Rc-dtor-ptr).
Also, copying may lead to slicing.

**Example, bad**:

	class Handle {		// Very suspect
		Shape& s;	// use reference rather than pointer to prevent rebinding
					// BAD: vague about ownership of *p
		// ...
	public:
		Handle(Shape& ss) : s{ss} { /* ... */ }
		// ...
	};

The problem of whether `Handle` is responsible for the destruction of its `Shape` is the same as for <a ref="#Rc-dtor-ptr">the pointer case</a>:
If the `Handle` owns the object referred to by `s` it must have a destructor.

**Example**:

	class Handle {		// OK
		owner<Shape&> s;	// use reference rather than pointer to prevent rebinding
		// ...
	public:
		Handle(Shape& ss) : s{ss} { /* ... */ }
		~Handle() { delete &s; }
		// ...
	};

Independently of whether `Handle` owns its `Shape`, we must consider the default copy operations suspect:

	Handle x {*new Circle{p1,17}};	// the Handle had better own the Circle or we have a leak
	Handle y {*new Triangle{p1,p2,p3}};
	x = y;		// the default assignment will try *x.s=*y.s

That `x=y` is highly suspect.
Assigning a `Triangle` to a `Circle`?
Unless `Shape` has its [copy assignment `=deleted`](#Rc-copy-virtual), only the `Shape` part of `Triangle` is copied into the `Circle`.


**Note**: Why not just require all owning references to be replaced by "smart pointers"?
 Changing from references to smart pointers implies code changes.
 We don't (yet) have smart references.
 Also, that may affect ABIs.

**Enforcement**:

* A class with a reference data member is suspect.
* A class with an `owner<T>` reference should define its default operations.


<a name="Rc-dtor-virtual"></a>
### C.35: A base class with a virtual function needs a virtual destructor

**Reason**: To prevent undefined behavior.
If an application attempts to delete a derived class object through a base class pointer, the result is undefined if the base class's destructor is non-virtual.
In general, the writer of a base class does not know the appropriate action to be done upon destruction.

**Example; bad**:

	struct Base {  // BAD: no virtual destructor
		virtual f();
	};

	struct D : Base {
		string s {"a resource needing cleanup"};
		~D() { /* ... do some cleanup ... */ }
		// ...
	};

    void use()
	{
	    unique_ptr<Base> p = make_unique<D>();
        // ...
	} // p's destruction calls ~Base(), not ~D(), which leaks D::s and possibly more

**Note**: A virtual function defines an interface to derived classes that can be used without looking at the derived classes.
Someone using such an interface is likely to also destroy using that interface.

**Note**: A destructor must be `public` or it will prevent stack allocation and normal heap allocation via smart pointer (or in legacy code explicit `delete`):

	class X {
		~X();	// private destructor
		// ...
	};

	void use()
	{
		X a;						// error: cannot destroy
		auto p = make_unique<X>();	// error: cannot destroy
	}

**Enforcement**: (Simple) A class with any virtual functions should have a virtual destructor.


<a name="Rc-dtor-fail"></a>
### C.36: A destructor may not fail

**Reason**: In general we do not know how to write error-free code if a destructor should fail.
The standard library requires that all classes it deals with have destructors that do not exit by throwing.

**Example**:

	class X {
	public:
		~X() noexcept;
		// ...
	};

	X::~X() noexcept
	{
		// ...
		if (cannot_release_a_resource) terminate();
		// ...
	}

**Note**: Many have tried to devise a fool-proof scheme for dealing with failure in destructors.
None have succeeded to come up with a general scheme.
This can be be a real practical problem: For example, what about a sockets that won't close?
The writer of a destructor does not know why the destructor is called and cannot "refuse to act" by throwing an exception.
See <a =href="#Sd dtor">discussion</a>.
To make the problem worse, many "close/release" operations are not retryable.
If at all possible, consider failure to close/cleanup a fundamental design error and terminate.

**Note**: Declare a destructor `noexcept`. That will ensure that it either completes normally or terminate the program.

**Note**: If a resource cannot be released and the program may not fail, try to signal the failure to the rest of the system somehow
(maybe even by modifying some global state and hope something will notice and be able to take care of the problem).
Be fully aware that this technique is special-purpose and error-prone.
Consider the "my connection will not close" example.
Probably there is a problem at the other end of the connection and only a piece of code responsible for both ends of the connection can properly handle the problem.
The destructor could send a message (somehow) to the responsible part of the system, consider that to have closed the connection, and return normally.

**Note**: If a destructor uses operations that may fail, it can catch exceptions and in some cases still complete successfully
(e.g., by using a different clean-up mechanism from the one that threw an exception).

**Enforcement**: (Simple) A destructor should be declared `noexcept`.


<a name="Rc-dtor-noexcept"></a>
### C.37: Make destructors `noexcept`

**Reason**: [A destructor may not fail](#Rc-dtor-fail). If a destructor tries to exit with an exception, it's a bad design error and the program had better terminate.

**Enforcement**: (Simple) A destructor should be declared `noexcept`.



<a name="SS-ctor"></a>
## C.ctor: Constructors

A constuctor defined how an object is initialized (constructed).


<a name="Rc-ctor"></a>
### C.40: Define a constructor if a class has an invariant

**Reason**: That's what constructors are for.

**Example**:

	class Date {	// a Date represents a valid date
					// in the January 1, 1900 to December 31, 2100 range
		Date(int dd, int mm, int yy)
			:d{dd}, m{mm}, y{yy}
			{
				if (!is_valid(d,m,y)) throw Bad_date{};	// enforce invariant
			}
		// ...
	private:
		int d,m,y;
	};

It is often a good idea to express the invariant as an `Ensure` on the constructor.

**Note**: A constructor can be used for convenience even if a class does not have an invariant. For example:

	struct Rec {
		string s;
		int i {0};
		Rec(const string& ss) : s{ss} {}
		Rec(int ii) :i{ii} {}
	};

	Rec r1 {7};
	Rec r2 {"Foo bar"};

**Note**: The C++11 initializer list rules eliminates the need for many constructors. For example:

	struct Rec2{
		string s;
		int i;
		Rec2(const string& ss, int ii = 0} :s{ss}, i{ii} {}		// redundant
	};

	Rec r1 {"Foo",7};
	Rec r2 {"Bar"};

The `Rec2` constructor is redundant.
Also, the default for `int` would be better done as a [member initializer](#Rc-in-class-initializer).

**See also**: [construct valid object](#Rc-complete) and [constructor throws](#Rc-throw).

**Enforcement**:

* Flag classes with user-define copy operations but no destructor (a user-defined copy is a good indicator that the class has an invariant)


<a name="Rc-complete"></a>
### C.41: A constructor should create a fully initialized object

**Reason**: A constructor establishes the invariant for a class. A user of a class should be able to assume that a constructed object is usable.

**Example; bad**:

	class X1 {
		FILE* f;	// call init() before any other function
		// ...
	public:
		X1() {}
		void init();	// initialize f
		void read();	// read from f
		// ...
	};

	void f()
	{
		X1 file;
		file.read();	// crash or bad read!
		// ...
		file.init();	// too late
		// ...
	}

Compilers do not read comments.

**Exception**: If a valid object cannot conveniently be constructed by a constructor [use a factory function](#Rc-factory).

**Note**: If a constructor acquires a resource (to create a valid object), that resource should be [released by the destructor](#Rc-dtor-release).
The idiom of having constructors acquire resources and destructors release them is called [RAII](#Rr-raii) ("Resource Acquisitions Is Initialization").


<a name="Rc-throw"></a>
### C.42: If a constructor cannot construct a valid object, throw an exception

**Reason**: Leaving behind an invalid object is asking for trouble.

**Example**:

	class X2 {
		FILE* f;	// call init() before any other function
		// ...
	public:
		X2(const string& name)
			:f{fopen(name.c_str(),"r"}
		{
			if (f==nullptr) throw runtime_error{"could not open" + name};
			// ...
		}
			
		void read();	// read from f
		// ...
	};

	void f()
	{
		X2 file {"Zeno"}; // throws if file isn't open
		file.read();	  // fine
		// ...
	}

**Example, bad**:

	class X3 {			// bad: the constructor leaves a non-valid object behind
		FILE* f;	// call init() before any other function
		bool valid;
		// ...
	public:
		X3(const string& name)
			:f{fopen(name.c_str(),"r"}, valid{false}
		{
			if (f) valid=true;
			// ...
		}
			
		void is_valid()() { return valid; }
		void read();		// read from f
		// ...
	};

	void f()
	{
		X3 file {Heraclides"};
		file.read();	// crash or bad read!
		// ...
		if (is_valid()()) {
			file.read();
			// ...
		}
		else {
			// ... handle error ...
		}
		// ...
	}

**Note**: For a variable definition (e.g., on the stack or as a member of another object) there is no explicit function call from which an error code could be returned. Leaving behind an invalid object an relying on users to consistently check an `is_valid()` function before use is tedious, error-prone, and inefficient.

**Exception**: There are domains, such as some hard-real-time systems (think airplane controls) where (without additional tool support) exception handling is not sufficiently predictable from a timing perspective. There the `is_valed()` technique must be used. In such cases, check `is_valid()` consistently and immediately to simulate [RAII](#Rr-raii).

**Alternative**: If you feel tempted to use some "post-constructor initialization" or "two-stage initialization" idiom, try not to do that. If you really have to, look at [factory functions](#Rc-factory).

**Enforcement**:
* (Simple) Every constructor should initialize every member variable (either explicitly, via a delegating ctor call or via default construction).
* (Unknown) If a constructor has an `Ensures` contract, try to see if it holds as a postcondition.


<a name="Rc-default0"></a>
### C.43: Give a class a default constructor

**Reason**: Many language and library facilities rely on default constructors,
e.g. `T a[10]` and `std::vector<T> v(10)` default initializes their elements.

**Example**:

	class Date {
	public:
		Date();
		// ...
	};
	
	vector<Date> vd1(1000);	// default Date needed here
	vector<Date> vd2(1000,Date{Month::october,7,1885});	// alternative

There is no "natural" default date (the big bang is too far back in time to be useful for most people), so this example is non-trivial.
`{0,0,0}` is not a valid date in most calendar systems, so choosing that would be introducing something like floating-point's NaN.
However, most realistic `Date` classes has a "first date" (e.g. January 1, 1970 is popular), so making that the default is usually trivial.

**Enforcement**:

* Flag classes without a default constructor


<a name="Rc-default00"></a>
### C.44: Prefer default constructors to be simple and non-throwing

**Reason**: Being able to set a value to "the default" without operations that might fail simplifies error handling and reasoning about move operations.

**Example, problematic**:

	template<typename T>
	class Vector0 {		// elem points to space-elem element allocated using new
	public:
		Vector0() :Vector0{0} {}
		Vector0(int n) :elem{new T[n]}, space{elem+n}, last{elem} {}
		// ...
	private:
		own<T*> elem;
		T* space;
		T* last;
	};

This is nice and general, but setting a `Vector0` to empty after an error involves an allocation, which may fail.
Also, having a default `Vector` represented as `{new T[0],0,0}` seems wasteful.
For example, `Vector0 v(100)` costs 100 allocations.

**Example**:

	template<typename T>
	class Vector1 {		// elem is nullptr or elem points to space-elem element allocated using new
	public:
		Vector1() noexcept {}	// sets the representation to {nullptr,nullptr,nullptr}; doesn't throw
		Vector1(int n) :elem{new T[n]}, space{elem+n}, last{elem} {}
		// ...
	private:
		own<T*> elem = nullptr;
		T* space = nullptr;
		T* last = nullptr;
	};
	
Using `{nullptr,nullptr,nullptr}` makes `Vector1{}` cheap, but a special case and implies run-time checks.
Setting a `Vector1` to empty after detecting an error is trivial.

**Enforcement**:

* Flag throwing default constructors


<a name="Rc-default"></a>
### C.45: Don't define a default constructor that only initializes data members; use in-class member initializers instead

**Reason**: Using in-class member initializers lets the compiler generate the function for you. The compiler-generated function can be more efficient.

**Example; bad**:

    class X1 { // BAD: doesn't use member initializers
        string s;
        int i;
    public:
        X1() :s{"default"}, i{1} { }
		// ...
    };

**Example**:

    class X2 {
        string s = "default";
        int i = 1;
    public:
        // use compiler-generated default constructor
		// ...
    };

 
**Enforcement**: (Simple) A default constructor should do more than just initialize member variables with constants.


<a name="Rc-explicit"></a>
### C.46: By default, declare single-argument constructors explicit

**Reason**: To avoid unintended conversions.

**Example; bad**:

	class String {
		// ...
	public:
		String(int);	// BAD
		// ...
	};

	String s = 10;	// surprise: string of size 10


**Exception**: If you really want an implicit conversion from the constructor argument type to the class type, don't use `explicit`:

	class Complex {
		// ...
	public:
		Complex(double d);	// OK: we want a conversion from d to {d,0}
		// ...
	};

	Complex z = 10.7;	// unsurprising conversion

**See also**: [Discussion of implicit conversions](#Ro-conversion).

**Enforcement**: (Simple) Single-argument constructors should be declared `explicit`. Good single argument non-`explicit` constructors are rare in most code based. Warn for all that are not on a "positive list".


<a name="Rc-order"></a>
### C.47: Define and initialize member variables in the order of member declaration

**Reason**: To minimize confusion and errors. That is the order in which the initialization happens (independent of the order of member initializers).

**Example; bad**:

	class Foo {
		int m1;
		int m2;
	public:
		Foo(int x) :m2{x}, m1{++x} { }	// BAD: misleading initializer order
		// ...
	};

	Foo x(1); // surprise: x.m1==x.m2==2

**Enforcement**: (Simple) A member initializer list should mention the members in the same order they are declared.

**See also**: [Discussion](#Sd order)


<a name="Rc-in-class-initializer"></a>
### C.48: Prefer in-class initializers to member initializers in constructors for constant initializers

**Reason**: Makes it explicit that the same value is expected to be used in all constructors. Avoids repetition. Avoids maintenance problems. It leads to the shortest and most efficient code.

**Example; bad**:

	class X {	// BAD
		int i;
		string s;
		int j;
	public:
		X() :i{666}, s{"qqq"} { }	// j is uninitialized
		X(int ii) :i{ii} {}			// s is "" and j is uninitialized
		// ...
	};

How would a maintainer know whether `j` was deliberately uninitialized (probably a poor idea anyway) and whether it was intentional to give `s` the default value `""` in one case and `qqq` in another (almost certainly a bug)? The problem with `j` (forgetting to initialize a member) often happens when a new member is added to an existing class.

**Example**:

	class X2 {
		int i {666};
		string s {"qqq"};
		int j {0};
	public:
		X2() = default;			// all members are initialized to their defaults
		X2(int ii) :i{ii} {}		// s and j initialized to their defaults
		// ...
	};

**Alternative**: We can get part of the benefits from default arguments to constructors, and that is not uncommon in older code. However, that is less explicit, causes more arguments to be passed, and is repetitive when there is more than one constructor:

	class X3 {	// BAD: inexplicit, argument passing overhead
		int i;
		string s;
		int j;
	public:
		X3(int ii = 666, const string& ss = "qqq", int jj = 0)
			:i{ii}, s{ss}, j{jj} { }		// all members are initialized to their defaults
		// ...
	};

**Enforcement**:
* (Simple) Every constructor should initialize every member variable (either explicitly, via a delegating ctor call or via default construction).
* (Simple) Default arguments to constructors suggest an in-class initializer may be more appropriate.


<a name="Rc-initialize"></a>
### C.49: Prefer initialization to assignment in constructors

**Reason**: An initialization explicitly states that initialization, rather than assignment, is done and can be more elegant and efficient. Prevents "use before set" errors.

**Example; good**:

	class A {		// Good
	    string s1;
	public:
		A() : s1{"Hello, "} { }    // GOOD: directly construct
		// ...
	};

**Example; bad**:

	class B {		// BAD
    	string s1;
	public:
    	B() { s1 = "Hello, "; }   // BAD: default constructor followed by assignment
		// ...
	};

	class C {		// UGLY, aka very bad
		int* p;
	public:
		C() { cout << *p; p = new int{10};	}	// accidental use before initialized
		// ...
	};



<a name="Rc-factory"></a>
### C.50: Use a factory function if you need "virtual behavior" during initialization

**Reason**: If the state of a base class object must depend on the state of a derived part of the object,
 we need to use a virtual function (or equivalent) while minimizing the window of opportunity to misuse an imperfectly constructed object.

**Example; bad**:

	class B {
	public:
	    B()
		{
			// ...
			f(); 		// BAD: virtual call in constructor
			//...
		}

	    virtual void f() = 0;

	    // ...
	};

**Example*:

	class B {
	protected:
	    B() { /* ... */ }					// create an imperfectly initialized object

	    virtual void PostInitialize()       // to be called right after construction
	    {
			// ...
			f();	// GOOD: virtual dispatch is safe
			// ...
		}

	public:
	    virtual void f() = 0;

	    template<class T>
	    static shared_ptr<T> Create()	// interface for creating objects
		{
	        auto p = make_shared<T>();
	        p->PostInitialize();
	        return p;
	    }
	};

	class D : public B { /* "¦ */ };			// some derived class

	shared_ptr<D> p = D::Create<D>();		// creating a D object

By making the constructor `protected` we avoid an incompletely constructed object escaping into the wild.
By providing the factory function `Create()`, we make construction (on the free store) convenient.

**Note**: Conventional factory functions allocate on the free store, rather than on the stack or in an enclosing object.

**See also**: [Discussion](#Sd factory)


<a name="Rc-delegating"></a>
### C.51: Use delegating constructors to represent common actions for all constructors of a class

**Reason**: To avoid repetition and accidental differences

**Example; bad**:

	class Date {	// BAD: repetitive
		int d;
		Month m;
		int y;
	public:
		Date(int ii, Month mm, year yy)
			:i{ii}, m{mm} y{yy}
			{ if (!valid(i,m,y)) throw Bad_date{}; }

		Date(int ii, Month mm)
			:i{ii}, m{mm} y{current_year()}
			{ if (!valid(i,m,y)) throw Bad_date{}; }
		// ...
	};

The common action gets tedious to write and may accidentally not be common.

**Example**:

	
	class Date2 {
		int d;
		Month m;
		int y;
	public:
		Date2(int ii, Month mm, year yy)
			:i{ii}, m{mm} y{yy}
			{ if (!valid(i,m,y)) throw Bad_date{}; }

		Date2(int ii, Month mm)
			:Date2{ii,mm,current_year()} {}
		// ...
	};

**See also**: If the "repeated action" is a simple initialization, consider [an in-class member initializer](#Rc-in-class-initializer).

**Enforcement**: (Moderate) Look for similar constructor bodies.


<a name="Rc-inheriting"></a>
### C.52: Use inheriting constructors to import constructors into a derived class that does not need further explicit initialization

**Reason**: If you need those constructors for a derived class, re-implementeing them is tedious and error prone.

**Example**: `std::vector` has a lot of tricky constructors, so it I want my own `vector`, I don't want to reimplement them:

	class Rec {
		// ... data and lots of nice constructors ...
	};

	class Oper : public Rec {
		using Rec::Rec;
		// ... no data members ...
		// ... lots of nice utility functions ...
	};

**Example; bad**:

	struct Rec2 : public Rec {
		int x;
		using Rec::Rec;
	};
	
	Rec2 r {"foo", 7};
	int val = r.x;	// uninitialized
	

**Enforcement**: Make sure that every member of the derived class is initialized.



<a name="SS-copy"></a>
## C.copy: Copy and move

Value type should generally be copyable, but interfaces in a class hierarchy should not.
Resource handles, may or may not be copyable.
Types can be defined to move for logical as well as performance reasons.


<a name="Rc-copy-assignment"></a>
### C.60: Make copy assignment non-`virtual`, take the parameter by `const&`, and return by non-`const&`

**Reason**: It is simple and efficient. If you want to optimize for rvalues, provide an overload that takes a `&&` (see [F.24](#Rf-pass-ref-ref)).

**Example**:

	class Foo {
	public:
		Foo& operator=(const Foo& x)
		{
            auto tmp = x;	// GOOD: no need to check for self-assignment (other than performance)
			std::swap(*this,tmp);
			return *this;
		}
		// ...
	};

	Foo a;
	Foo b;
	Foo f();

	a = b;		// assign lvalue: copy
	a = f();	// assign rvalue: potentially move

**Note**: The `swap` implementation technique offers the [strong guarantee](???).

**Example**: But what if you can get significant better performance by not making a temporary copy? Consider a simple `Vector` intended for a domain where assignment of large, equal-sized `Vector`s is common. In this case, the copy of elements implied by the `swap` implementation technique could cause an order of magnitude increase in cost:

	template<typename T>
	class Vector {
	public:
		Vector& operator=(const Vector&);
		// ...
	private:
		T* elem;
		int sz;
	};

	Vector& Vector::operator=(const Vector& a)
	{
		if (a.sz>sz)
		{
			// ... use the swap technique, it can't be bettered ...
			*return *this
		}
		// ... copy sz elements from *a.elem to elem ...
		if (a.sz<sz) {
			// ... destroy the surplus elements in *this* and adjust size ...
		}
		return *this*
	}

By writing directly to the target elements, we will get only [the basic guarantee](#???) rather than the strong guaranteed offered by the `swap` technique. Beware of [self assignment](#Rc-copy-self).

**Alternatives**: If you think you need a `virtual` assignment operator, and understand why that's deeply problematic, don't call it `operator=`. Make it a named function like `virtual void assign(const Foo&)`.
See [copy constructor vs. `clone()`](#Rc-copy-virtual).

**Enforcement**:

* (Simple) An assignment operator should not be virtual. Here be dragons!
* (Simple) An assignment operator should return `T&` to enable chaining, not alternatives like `const T&` which interfere with composability and putting objects in containers.
* (Moderate) An assignment operator should (implicitly or explicitly) invoke all base and member assignment operators.
Look at the destructor to determine if the type has pointer semantics or value semantics.


<a name="Rc-copy-semantic"></a>
### C.61: A copy operation should copy

**Reason**: That is the generally assumed semantics. After `x=y`, we should have `x==y`.
After a copy `x` and `y` can be independent objects (value semantics, the way non-pointer built-in types and the standard-library types work) or refer to a shared object (pointer semantics, the way pointers work).

**Example**:

	class X {	// OK: value semantics
	public:
		X();
		X(const X&);	// copy X
		void modify();	// change the value of X
		// ...
		~X() { delete[] p; }
	private:
		T* p;
		int sz;
	};

	bool operator==(const X& a, const X& b)
	{
		return sz==a.sz && equal(p,p+sz,a.p,a.p+sz);
	}

	X::X(const X& a)
		:p{new T}, sz{a.sz}
	{
		copy(a.p,a.p+sz,a.p);
	}

	X x;
	X y = x;
	if (x!=y) throw Bad{};
	x.modify();
	if (x==y) throw Bad{};	// assume value semantics

**Example**:

	class X2 {	// OK: pointer semantics
	public:
		X2();
		X2(const X&) = default;	// shallow copy
		~X2() = default;
		void modify();			// change the value of X
		// ...
	private:
		T* p;
		int sz;
	};

	bool operator==(const X2& a, const X2& b)
	{
		return sz==a.sz && p==a.p;
	}

	X2 x;
	X2 y = x;
	if (x!=y) throw Bad{};
	x.modify();
	if (x!=y) throw Bad{};	// assume pointer semantics

**Note**: Prefer copy semantics unless you are building a "smart pointer". Value semantics is the simplest to reason about and what the standard library facilities expect.

**Enforcement**: (Not enforceable).
	
	
<a name="Rc-copy-self"></a>
### C.62: Make copy assignment safe for self-assignment

**Reason**: If `x=x` changes the value of `x`, people will be surprised and bad errors will occur (often including leaks).

**Example**: The standard-library containers handle self-assignment elegantly and efficiently:

	std::vector<int> v = {3,1,4,1,5,9};
	v = v;
	// the value of v is still {3,1,4,1,5,9}

**Note**: The default assignment generated from members that handle self-assignment correctly handles self-assignment.

	struct Bar {
		vector<pair<int,int>> v;
		map<string,int> m;
		string s;
	};

	Bar b;
	// ...
	b = b;	// correct and efficient

**Note**: You can handle self-assignment by explicitly testing for self-assignment, but often it is faster and more elegant to cope without such a test (e.g., [using `swap`](#Rc-swap)).

	class Foo {
		string s;
		int i;
	public:
		Foo& operator=(const Foo& a);
		// ...
	};
	
	Foo& Foo::operator=(const Foo& a)	// OK, but there is a cost
	{
		if (this==&a) return *this;
		s = a.s;
		i = a.i;
		return *this;
	}

This is obviously safe and apparently efficient.
However, what if we do one self-assignment per million assignments?
That's about a million redundant tests (but since the answer is essentially always the same, the computer's branch predictor will guess right essentially every time).
Consider:

	Foo& Foo::operator=(const Foo& a)	// simpler, and probably much better
	{
		s = a.s;
		i = a.i;
		return *this;
	}

`std::string` is safe for self-assignment and so are `int`. All the cost is carried by the (rare) case of self-assignment.

**Enforcement**: (Simple) Assignment operators should not contain the pattern `if (this==&a) return *this;` ???

	
<a name="Rc-move-assignment"></a>
### C.63: Make move assignment non-`virtual`, take the parameter by `&&`, and return by non-`const `&`

**Reason**: It is simple and efficient.

**See**: [The rule for copy-assignment](#Rc-copy-assignment).

**Enforcement**: Equivalent to what is done for [copy-assignment](#Rc-copy-assignment).
* (Simple) An assignment operator should not be virtual. Here be dragons!
* (Simple) An assignment operator should return `T&` to enable chaining, not alternatives like `const T&` which interfere with composability and putting objects in containers.
* (Moderate) A move assignment operator should (implicitly or explicitly) invoke all base and member move assignment operators.


<a name="Rc-move-semantic"></a>
### C.64: A move operation should move and leave its source in valid state

**Reason**: That is the generally assumed semantics. After `x=std::move(y)` the value of `x` should be the value `y` had and `y` should be in a valid state.

**Example**:
	
	class X {	// OK: value semantics
	public:
		X();
		X(X&& a);		// move X
		void modify();	// change the value of X
		// ...
		~X() { delete[] p; }
	private:
		T* p;
		int sz;
	};


	X::X(X&& a)
		:p{a.p}, sz{a.sz}	// steal representation
	{
		a.p = nullptr;		// set to "empty"
		a.sz = 0;
	}

	void use()
	{
		X x{};
		// ...
		X y = std::move(x);
		x = X{};	// OK
	} // OK: x can be destroyed

**Note**: Ideally, that moved-from should be the default value of the type. Ensure that unless there is an exceptionally good reason not to. However, not all types have a default value and for some types establishing the default value can be expensive. The standard requires only that the moved-from object can be destroyed.
Often, we can easily and cheaply do better: The standard library assumes that it it possible to assign to a moved-from object. Always leave the moved-from object in some (necessarily specified) valid state.

**Note**: Unless there is an exceptionally strong reason not to, make `x=std::move(y); y=z;` work with the conventional semantics.

**Enforcement**: (Not enforceable) look for assignments to members in the move operation. If there is a default constructor, compare those assignments to the initializations in the default constructor.


<a name="Rc-move-self"></a>
### C.65: Make move assignment safe for self-assignment

**Reason**: If `x=x` changes the value of `x`, people will be surprised and bad errors may occur. However, people don't usually directly write a self-assignment that turn into a move, but it can occur. However, `std::swap` is implemented using move operations so if you accidentally do `swap(a,b)` where `a` and `b` refer to the same object, failing to handle self-move could be a serious and subtle error.

**Example**:

	class Foo {
		string s;
		int i;
	public:
		Foo& operator=(Foo&& a);
		// ...
	};
	
	Foo& Foo::operator=(Foo&& a)	// OK, but there is a cost
	{
		if (this==&a) return *this;	// this line is redundant
		s = std::move(a.s);
		i = a.i;
		return *this;
	}

The one-in-a-million argument against `if (this==&a) return *this;` tests from the discussion of [self-assignment](#Rc-copy-self) is even more relevant for self-move.

**Note**: There is no know general way of avoiding a `if (this==&a) return *this;` test for a move assignment and still get a correct answer (i.e., after `x=x` the value of `x` is unchanged).

**Note** The ISO standard guarantees only a "valid but unspecified" state for the standard library containers. Apparently this has not been a problem in about 10 years of experimental and production use. Please contact the editors if you find a counter example. The rule here is more caution and insists on complete safety.

**Example**: Here is a way to move a pointer without a test (imagine it as code in the implementation a move assignment):

	// move from other.oter to this->ptr
	T* temp = other.ptr;
	other.ptr = nullptr;
	delete ptr;
	ptr = temp;
	
**Enforcement**:

* (Moderate) In the case of self-assignment, a move assignment operator should not leave the object holding pointer members that have been `delete`d or set to nullptr.
* (Not enforceable) Look at the use of standard-library container types (incl. `string`) and consider them safe for ordinary (not life-critical) uses.


<a name="Rc-move-noexcept"></a>
### C.66: Make move operations `noexcept`

**Reason**: A throwing move violates most people's reasonably assumptions.
A non-throwing move will be used more efficiently by standard-library and language facilities.

**Example**:

	class Vector {
		// ...
		Vector(Vector&& a) noexcept :elem{a.elem}, sz{a.sz} { a.sz=0; a.elem=nullptr; }
		Vector& operator=(Vector&& a) noexcept { elem=a.elem; sz=a.sz; a.sz=0; a.elem=nullptr; }
		//...
	public:
		T* elem;
		int sz;
	};

These copy operations do not throw.

**Example, bad**:

	class Vector2 {
		// ...
		Vector2(Vector2&& a) { *this = a; }				// just use the copy
		Vector2& operator=(Vector2&& a) { *this = a; }	// just use the copy
		//...
	public:
		T* elem;
		int sz;
	};

This `Vector2` is not just inefficient, but since a vector copy requires allocation, it can throw.

**Enforcement**: (Simple) A move operation should be marked `noexcept`.




<a name="Rc-copy-virtual"></a>
### C.67: A base class should suppress copying, and provide a virtual `clone` instead if "copying" is desired

**Reason**: To prevent slicing, because the normal copy operations will copy only the base portion of a derived object.

**Example; bad**:

    class B { // BAD: base class doesn't suppress copying
        int data;
        // ... nothing about copy operations, so uses default ...
    };

    class D : public B {
        string moredata; // add a data member
        // ...
    };

    auto d = make_unique<D>();
    auto b = make_unique<B>(d); // oops, slices the object; gets only d.data but drops d.moredata

**Example**:

    class B { // GOOD: base class suppresses copying
        B(const B&) =delete;
        B& operator=(const B&) =delete;
        virtual unique_ptr<B> clone() { return /* B object */; }
        // ...
    };

    class D : public B {
        string moredata; // add a data member
        unique_ptr<B> clone() override { return /* D object */; }
        // ...
    };

    auto d = make_unique<D>();
    auto b = d.clone(); // ok, deep clone

**Note**: It's good to return a smart pointer, but unlike with raw pointers the return type cannot be covariant (for example, `D::clone` can't return a `unique_ptr<D>`. Don't let this tempt you into returning an owning raw pointer; this is a minor drawback compared to the major robustness benefit delivered by the owning smart pointer.

**Enforcement**: A class with any virtual function should not have a copy constructor or copy assignment operator (compiler-generated or handwritten).



## C.other: Other default operations

???

<a name="Rc-=default"></a>
### C.80: Use `=default` if you have to be explicit about using the default semantics

**Reason**: The compiler is more likely to get the default semantics right and you cannot implement these function better than the compiler.

**Example**:

	class Tracer {
		string message;
	public:
		Tracer(const string& m) : message{m} { cerr << "entering " << message <<'\n'; }
		~Tracer() { cerr << "exiting " << message <<'\n'; }
		
		Tracer(const Tracer&) = default;
		Tracer& operator=(const Tracer&) = default;
		Tracer(Tracer&&) = default;
		Tracer& operator=(Tracer&&) = default;
	};

Because we defined the destructor, we must define the copy and move operations. The `=default` is the best and simplest way of doing that.

**Example, bad**:

	class Tracer2 {
		string message;
	public:
		Tracer2(const string& m) : message{m} { cerr << "entering " << message <<'\n'; }
		~Tracer2() { cerr << "exiting " << message <<'\n'; }
		
		Tracer2(const Tracer2& a) : message{a.message} {}
		Tracer2& operator=(const Tracer2& a) { message=a.message; }
		Tracer2(Tracer2&& a) :message{a.message} {}
		Tracer2& operator=(Tracer2&& a) { message=a.message; }
	};

Writing out the bodies of the copy and move operations is verbose, tedious, and error-prone. A compiler does it better.

**Enforcement**: (Moderate) The body of a special operation should not have the same accessibility and semantics as the compiler-generated version, because that would be redundant


<a name="Rc-=delete"></a>
### C.81: Use `=delete` when you want to disable default behavior (without wanting an alternative)

**Reason**: In a few cases, a default operation is not desirable.

**Example**:

	class Immortal {
	public:
		~Immortal() = delete;	// do not allow destruction
		// ...
	};

	void use()
	{
		Immortal ugh;	// error: ugh cannot be destroyed
		Immortal* p = new Immortal{};
		delete p;		// error: cannot destroy *p
	}

**Example**: A `unique_ptr` can be moved, but not copied. To achieve that its copy operations are deleted. To avoid copying it is necessary to `=delete` its copy operations from lvalues:

	template <class T, class D = default_delete<T>> class unique_ptr {
	public:
		// ...
		constexpr unique_ptr() noexcept;
		explicit unique_ptr(pointer p) noexcept;
		// ...
		unique_ptr(unique_ptr&& u) noexcept;	// move constructor
		// ...
		unique_ptr(const unique_ptr&) = delete; // disable copy from lvalue
		// ...
	};

	unique_ptr<int> make();	// make "something" and return it by moving

	void f()
	{
		unique_ptr<int> pi {};
		auto pi2 {pi};		// error: no move constructor from lvalue
		auto pi3 {make()};	// OK, move: the result of make() is an rvalue
	}

**Enforcement**: The elimination of a default operation is (should be) based on the desired semantics of the class. Consider such classes suspect, but maintain a "positive list" of classes where a human has asserted that the semantics is correct.

<a name="Rc-ctor-virtual"></a>
### C.82: Don't call virtual functions in constructors and destructors

**Reason**: The function called will be that of the object constructed so far, rather than a possibly overriding function in a derived class.
This can be most confusing.
Worse, a direct or indirect call to an unimplemented pure virtual function from a constructor or destructor results in undefined behavior.

**Example; bad**:

	class base {
	public:
	    virtual void f() = 0;   // not implemented
	    virtual void g();       // implemented with base version
	    virtual void h();       // implemented with base version
	};

	class derived : public base {
	public:
		void g() override;      // provide derived implementation
	    void h() final;         // provide derived implementation

	    derived()
		{
	        f();                // BAD: attempt to call an unimplemented virtual function

			g();                // BAD: will call derived::g, not dispatch further virtually
			derived::g();       // GOOD: explicitly state intent to call only the visible version

			h();                // ok, no qualification needed, h is final
	    }
	};

Note that calling a specific explicitly qualified function is not a virtual call even if the function is `virtual`.

**See also** [factory functions](#Rc-factory) for how to achieve the effect of a call to a derived class function without risking undefined behavior.


<a name="Rc-swap"></a>
### C.83: For value-like types, consider providing a `noexcept` swap function

**Reason**: A `swap` can be handy for implementing a number of idioms, from smoothly moving objects around to implementing assignment easily to providing a guaranteed commit function that enables strongly error-safe calling code. Consider using swap to implement copy assignment in terms of copy construction. See also [destructors, deallocation, and swap must never fail](#Re-never-fail).

**Example; good**:

    class Foo {
		// ...
    public:
        void swap(Foo& rhs) noexcept
		{
            m1.swap(rhs.m1);
            std::swap(m2, rhs.m2);
        }
    private:
        Bar m1;
        int m2;
    };

Providing a nonmember `swap` function in the same namespace as your type for callers' convenience.

    void swap(Foo& a, Foo& b)
	{
		a.swap(b);
	}

**Enforcement**:
* (Simple) A class without virtual functions should have a `swap` member function declared.
* (Simple) When a class has a `swap` member function, it should be declared `noexcept`.

<a name="Rc-swap-fail"></a>
### C.84: A `swap` function may not fail

**Reason**: `swap` is widely used in ways that are assumed never to fail and programs cannot easily be written to work correctly in the presence of a failing `swap`. The The standard-library containers and algorithms will not work correctly if a swap of an element type fails.

**Example, bad**:

	void swap(My_vector& x, My_vector& y)
	{
		auto tmp = x;	// copy elements
		x = y;
		y = tmp;
	}

This is not just slow, but if a memory allocation occur for the elements in `tmp`, this `swap` may throw and would make STL algorithms fail is used with them.

**Enforcement**: (Simple) When a class has a `swap` member function, it should be declared `noexcept`.


<a name="Rc-swap-noexcept"></a>
### C.85: Make `swap` `noexcept`

**Reason**: [A `swap` may not fail](#Rc-swap-fail).
If a `swap` tries to exit with an exception, it's a bad design error and the program had better terminate.

**Enforcement**: (Simple) When a class has a `swap` member function, it should be declared `noexcept`.


<a name="Rc-eq"></a>
### C.86: Make `==` symmetric with respect to operand types and `noexcept`

**Reason**: Assymetric treatment of operands is surprising and a source of errors where conversions are possible.
`==` is a fundamental operations and programmers should be able to use it without fear of failure.

**Example**:

	class X {
		string name;
		int number;
	};
	
	bool operator==(const X& a, const X& b) noexcept { return a.name==b.name && a.number==b.number; }
	
**Example, bad**:
	
	class B {
		string name;
		int number;
		bool operator==(const B& a) const { return name==a.name && number==a.number; }
		// ...
	};
	
`B`'s comparison accepts conversions for its second operand, but not its first.

**Note**: If a class has a failure state, like `double`'s `NaN`, there is a temptation to make a comparison against the failure state throw.
The alternative is to make two failure states compare equal and any valid state compare false against the failure state.

**Enforcement**: ???


<a name="Rc-eq-base"></a>
### C.87: Beware of `==` on base classes

**Reason**: It is really hard to write a foolproof and useful `==` for a hierarchy.
	
**Example, bad**:
	
	class B {
		string name;
		int number;
		virtual bool operator==(const B& a) const { return name==a.name && number==a.number; }
		// ...
	};
	
// `B`'s comparison accepts conversions for its second operand, but not its first.

	class D :B {
		char character;
		virtual bool operator==(const D& a) const { return name==a.name && number==a.number && character==a.character; }
		// ...
	};
	
	B b = ...
	D d = ...
	b==d;	// compares name and number, ignores d's character
	d==b;	// error: no == defined
	D d2;
	d==d2;	// compares name, number, and character
	B& b2 = d2;
	b2==d;	// compares name and number, ignores d2's and d's character
	
Of course there are way of making `==` work in a hierarchy, but the naive approaches do not scale
	
**Enforcement**: ???


<a name="Rc-lt"></a>
### C.88: Make `<` symmetric with respect to operand types and `noexcept`

**Reason**: ???

**Example**:

	???
	
**Enforcement**: ???


<a name="Rc-hash"></a>
### C.89: Make a `hash` `noexcept`

**Reason**: ???

**Example**:

	???
	
**Enforcement**: ???

<a name="SS-containers"</a>
## C.con: Containers and other resource handles

A container is an object holding a sequence of objects of some type; `std::vector` is the archetypical container.
A resource handle is a class that owns a resource; `std::vector` is the typical resource handle; its resource is its sequence of elements.

Summary of container rules:

* [C.100: Follow the STL when defining a container](#Rcon-stl)
* [C.101: Give a container value semantics](#Rcon-val)
* [C.102: Give a container move operations](#Rcon-move)
* [C.103: Give a container an initializer list constructor](#Rcon-init)
* [C.104: Give a container a default constructor that sets it to empty](#Rcon-empty)
* [C.105: Give a constructor and `Extent` constructor](#Rcon-val)
* ???
* [C.109: If a resource handle has pointer semantics, provide `*` and `->`](#rcon-ptr)

**See also**: [Resources](#S-resource)


<a name="SS-lambdas"></a>
## C.lambdas: Function objects and lambdas

A function object is an object supplying an overloaded `()` so that you can call it.
A lambda expression (colloquially often shortened to "a lambda") is a notation for generating a function object.

Summary:

* [F.50: Use a lambda when a function won't do (to capture local variables, or to write a local function)](#Rf-capture-vs-overload)
* [F.52: Prefer capturing by reference in lambdas that will be used locally, including passed to algorithms](#Rf-reference-capture)
* [F.53: Avoid capturing by reference in lambdas that will be used nonlocally, including returned, stored on the heap, or passed to another thread](#Rf-value-capture)
* [ES.28: Use lambdas for complex initialization, especially of `const` variables](#Res-lambda-init)



<a name="SS-hier"></a>
## C.hier: Class hierarchies (OOP)

A class hierarchy is constructed to represent a set of hierarchically organized concepts (only).
Typically base classes act as interfaces.
There are two major uses for hierarchies, often named implementation inheritance and interface inheritance.

Class hierarchy rule summary:

* [C.120: Use class hierarchies to represent concepts with inherent hierarchical structure](#Rh-domain)
* [C.121: If a base class is used as an interface, make it a pure abstract class](#Rh-abstract)
* [C.122: Use abstract classes as interfaces when complete separation of interface and implementation is needed](#Rh-separation)

Designing rules for classes in a hierarchy summary:

* [C.126: An abstract class typically doesn't need a constructor](#Rh-abstract-ctor)
* [C.127: A class with a virtual function should have a virtual destructor](#Rh-dtor)
* [C.128: Use `override` to make overriding explicit in large class hierarchies](#Rh-override)
* [C.129: When designing a class hierarchy, distinguish between implementation inheritance and interface inheritance](#Rh-kind)
* [C.130: Redefine or prohibit copying for a base class; prefer a virtual `clone` function instead](#Rh-copy)
	
* [C.131: Avoid trivial getters and setters](#Rh-get)
* [C.132: Don't make a function `virtual` without reason](#Rh-virtual)
* [C.133: Avoid `protected` data](#Rh-protected)
* [C.134: Ensure all data members have the same access level](#Rh-public)
* [C.135: Use multiple inheritance to represent multiple distinct interfaces](#Rh-mi-interface)
* [C.136: Use multiple inheritance to represent the union of implementation attributes](#Rh-mi-implementation)
* [C.137: Use `virtual` bases to avoid overly general base classes](#Rh-vbase)
* [C.138: Create an overload set for a derived class and its bases with `using`](#Rh-using)

Accessing objects in a hierarchy rule summary:

* [C.145: Access polymorphic objects through pointers and references](#Rh-poly)
* [C.146: Use `dynamic_cast` where class hierarchy navigation is unavoidable](#Rh-dynamic_cast)
* [C.147: Use `dynamic_cast` to a reference type when failure to find the required class is considered an error](#Rh-ptr-cast)
* [C.148: Use `dynamic_cast` to a pointer type when failure to find the required class is considered a valid alternative](#Rh-ref-cast)
* [C.149: Use `unique_ptr` or `shared_ptr` to avoid forgetting to `delete` objects created using `new`](#Rh-smart)
* [C.150: Use `make_unique()` to construct objects owned by `unique_ptr`s or another smart pointer](#Rh-make_unique)
* [C.151: Use `make_shared()` to construct objects owned by `shared_ptr`s](#Rh-make_shared)
* [C.152: Never assign a pointer to an array of derived class objects to a pointer to its base](#Rh-array)


<a name="Rh-domain"></a>
### C.120: Use class hierarchies to represent concepts with inherent hierarchical structure (only)

**Reason**: Direct representation of ideas in code eases comprehension and maintenance. Make sure the idea represented in the base class exactly matches all derived types and there is not a better way to express it than using the tight coupling of inheritance.

Do *not* use inheritance when simply having a data member will do. Usually this means that the derived type needs to override a base virtual function or needs access to a protected member.

**Example**:

	??? Good old Shape example?
	
**Example, bad**:
Do *not* represent non-hierarchical domain concepts as class hierarchies.

	template<typename T>
	class Container {
	public:
		// list operations:
		virtual T& get() = 0;
		virtual void put(T&) = 0;
		virtual void insert(Position) = 0;
		// ...
		// vector operations:
		virtual T& operator[](int) = 0;
		virtual void sort() = 0;
		// ...
		// tree operations:
		virtual void balance() = 0;
		// ...
	};
	
Here most overriding classes cannot implement most of the functions required in the interface well.
Thus the base class becomes an implementation burden.
Furthermore, the user of `Container` cannot rely on the member functions actually performing a meaningful operations reasonably efficiently;
it may throw an exception instead.
Thus users have to resort to run-time checking and/or
not using this (over)general interface in favor of a particular interface found by a run-time type inquiry (e.g., a `dynamic_cast`).

**Enforcement**:

* Look for classes with lots of members that do nothing but throw.
* Flag every use of a nonpublic base class where the derived class does not override a virtual function or access a protected base member.


<a name="Rh-abstract"></a>
### C.121: If a base class is used as an interface, make it a pure abstract class

**Reason**: A class is more stable (less brittle) if it does not contain data. Interfaces should normally be composed entirely of public pure virtual functions.

**Example**:

	???

**Enforcement**:

* Warn on any class that contains data members and also has an overridable (non-`final`) virtual function.


<a name="Rh-separation"></a>
### C.122: Use abstract classes as interfaces when complete separation of interface and implementation is needed

**Reason**: Such as on an ABI (link) boundary.

**Example**:

	???

**Enforcement**: ???



## C.hierclass: Designing classes in a hierarchy:


<a name="Rh-abstract-ctor"></a>
### C.126: An abstract class typically doesn't need a constructor

**Reason**: An abstract class typically does not have any data for a constructor to initialize.

**Example**:

	???
	
**Exceptions**:
* A base class constructor that does work, such as registering an object somewhere, may need a constructor.
* In extremely rare cases, you might find a reasonable for an abstract class to have a bit of data shared by all derived classes
(e.g., use statistics data, debug information, etc.); such classes tend to have constructors. But be warned: Such classes also tend to be prone to requiring virtual inheritance.

**Enforcement**: Flag abstract classes with constructors.


<a name="Rh-dtor"></a>
### C.127: A class with a virtual function should have a virtual destructor

**Reason**: A class with a virtual function is usually (and in general) used via a pointer to base, including that the last user has to call delete on a pointer to base, often via a smart pointer to base.

**Example, bad**:

	struct B {
		// ... no destructor ...
	};

	struct D : B {		// bad: class with a resource derived from a class without a virtual destructor
		string s {"default"};
	};

	void use()
	{
		B* p = new D;
		delete p;	// leak the string
	}

**Note**: There are people who don't follow this rule because they plan to use a class only through a `shared_ptr`: `std::shared_ptr<B> p = std::make_shared<D>(args);` Here, the shared pointer will take care of deletion, so no leak will occur from and inappropriate `delete` of the base. People who do this consistently can get a false positive, but the rule is important -- what if one was allocated using `make_unique`? It's not safe unless the author of `B` ensures that it can never be misused, such as by making all constructors private and providing a factory functions to enforce the allocation with `make_shared`.

**Enforcement**:

* Flag a class with a virtual function and no virtual destructor. Note that this rule needs only be enforced for the first (base) class in which it occurs, derived classes inherit what they need. This flags the place where the problem arises, but can give false positives.
* Flag `delete` of a class with a virtual function but no virtual destructor.


<a name="Rh-override"></a>
### C.128: Use `override` to make overriding explicit in large class hierarchies

**Reason**: Readability. Detection of mistakes. Explicit `override` allows the compiler to catch mismatch of types and/or names between base and derived classes.

**Example, bad**:

	struct B {
		void f1(int);
		virtual void f2(int);
		virtual void f3(int);
		// ...
	};

	struct D : B {
		void f1(int);		// warn: D::f1() hides B::f1()
		void f2(int);		// warn: no explicit override
		void f3(double);	// warn: D::f3() hides B::f3()
		// ...
	};

**Enforcement**:

* Compare names in base and derived classes and flag uses of the same name that does not override.
* Flag overrides without `override`.


<a name="Rh-kind"><a>
### C.129: When designing a class hierarchy, distinguish between implementation inheritance and interface inheritance

**Reason**: ??? Herb: I've become a non-fan of implementation inheritance -- seems most often an antipattern. Are there reasonable examples of it?

**Example**:

	???

**Enforcement**: ???


<a name="Rh-copy"></a>
### C.130: Redefine or prohibit copying for a base class; prefer a virtual `clone` function instead

**Reason**: Copying a base is usually slicing. If you really need copy semantics, copy deeply: Provide a virtual `clone` function that will copy the actual most-derived type, and in derived classes return the derived type (use a covariant return type).

**Example**:

	class base {
	public:
	    virtual base* clone() =0;
	};
	
	class derived : public base {
	public:
	    derived* clone() override;
	};
	
Note that because of language rules, the covariant return type cannot be a smart pointer.

**Enforcement**:

* Flag a class with a virtual function and a non-user-defined copy operation.
* Flag an assignment of base class objects (objects of a class from which another has been derived).
 
  
<a name="Rh-get"></a>
### C.131: Avoid trivial getters and setters

**Reason**: A trivial getter or setter adds no semantic value; the data item could just as well be `public`.

**Example**:

	class point {
	    int x;
		int y;
	public:
	    point(int xx, int yy) : x{xx}, y{yy} { }
	    int get_x() { return x; }
		void set_x(int xx) { x = xx; }
	    int get_y() { return y; }
		void set_y(int yy) { y = yy; }
		// no behavioral member functions
	};

Consider making such a class a `struct` -- that is, a behaviorless bunch of variables, all public data and no member functions.

	struct point {
		int x = 0;
		int y = 0;
	};

**Note**: A getter or a setter that converts from an internal type to an interface type is not trivial (it provides a form of information hiding).

**Enforcement**: Flag multiple `get` and `set` member functions that simply access a member without additional semantics.


<a name="Rh-virtual"></a>
### C.132: Don't make a function `virtual` without reason

**Reason**: Redundant `virtual` increases run-time and object-code size.
A virtual function can be overridden and is thus open to mistakes in a derived class.
A virtual function ensures code replication in a templated hierarchy.

**Example, bad**:

	template<class T>
	class Vector {
	public:
		// ...
		virtual int size() const { return sz; }	// bad: what good could a derived class do?
	private:
		T* elem;	// the elements
		int sz; 	// number of elements
	};

This kind of "vector" isn't meant to be used as a base class at all.

**Enforcement**:

* Flag a class with virtual functions but no derived classes.
* Flag a class where all member functions are virtual and have implementations.


<a name="Rh-protected"></a>
### C.133: Avoid `protected` data

**Reason**: `protected` data is a source of complexity and errors.
`protected` data complicated the statement of invariants.
`protected` data inherently violates the guidance against putting data in base classes, which usually leads to having to deal virtual inheritance as well.

**Example**:

	???
	
**Note**: Protected member function can be just fine.

**Enforcement**: Flag classes with `protected` data.


<a name="Rh-public"></a>
### C.134: Ensure all data members have the same access level

**Reason**: If they don't, the type is confused about what it's trying to do. Only if the type is not really an abstraction, but just a convenience bundle to group individual variables with no larger behavior (a behaviorless bunch of variables), make all data members `public` and don't provide functions with behavior. Otherwise, the type is an abstraction, so make all its data members `private`. Don't mix `public` and `private` data.

**Example**:

	???

**Enforcement**: Flag any class that has data members with different access levels.


<a name="Rh-mi-interface"></a>
### C.135: Use multiple inheritance to represent multiple distinct interfaces

**Reason**: Not all classes will necessarily support all interfaces, and not all callers will necessarily want to deal with all operations. Especially to break apart monolithic interfaces into "aspects" of behavior supported by a given derived class.

**Example**:

	???
	
**Note**: This is a very common use of inheritance because the need for multiple different interfaces to an implementation is common
and such interfaces are often not easily or naturally organized into a single-rooted hierarchy.

**Note**: Such interfaces are typically abstract classes.

**Enforcement**: ???


<a name="Rh-mi-implementation"></a>
### C.136: Use multiple inheritance to represent the union of implementation attributes

**Reason**: ??? Herb: Here's the second mention of implementation inheritance. I'm very skeptical, even of single implementation inheritance, never mind multiple implementation inheritance which just seems frightening -- I don't think that even policy-based design really needs to inherit from the policy types. Am I missing some good examples, or could we consider discouraging this as an anti-pattern?

**Example**:

	???
	
**Note**: This a relatively rare use because implementation can often be organized into a single-rooted hierarchy.

**Enforcement**: ??? Herb: How about opposite enforcement: Flag any type that inherits from more than one non-empty base class?


<a name="Rh-vbase"></a>
### C.137: Use `virtual` bases to avoid overly general base classes

**Reason**: ???

**Example**:

	???
	
**Note**: ???

**Enforcement**: ???

<a name="Rh-using"></a>
### C.138: Create an overload set for a derived class and its bases with `using`

**Reason**: ???

**Example**:

	???


## C.hier-access: Accessing objects in a hierarchy


<a name="Rh-poly"></a>
### C.145: Access polymorphic objects through pointers and references

**Reason**: If you have a class with a virtual function, you don't (in general) know which class provided the function to be used.

**Example**:

	struct B { int a; virtual int f(); };
	struct D : B { int b; int f() override; };

	void use(B b)
	{
		D d;
		B b2 = d;	// slice
		B b3 = b;
	}

	void use2()
	{
		D d;
		use(d);	// slice
	}

Both `d`s are sliced.

**Exception**: You can safely access a named polymorphic object in the scope of its definition, just don't slice it.

	void use3()
	{
		D d;
		d.f();	// OK
	}

**Enforcement**: Flag all slicing.


<a name="Rh-dynamic_cast"></a>
### C.146: Use `dynamic_cast` where class hierarchy navigation is unavoidable

**Reason**: `dynamic_cast` is checked at run time.

**Example**:

	struct B {	// an interface
		virtual void f();
		virtual void g();
	};
	
	struct D : B {	// a wider interface
		void f() override;
		virtual void h();
	};
	
	void user(B* pb)
	{
		if (D* pd = dynamic_cast<D*>(pb)) {
			// ... use D's interface ...
		}
		else {
			// .. make do with B's interface ...
		}
	}

**Note**: Like other casts, `dynamic_cast` is overused.
[Prefer virtual functions to casting](#???).
Prefer [static polymorphism](#???) to hierarchy navigation where it is possible (no run-time resolution necessary)
and reasonably convenient.

**Exception**: If your implementation provided a really slow `dynamic_cast`, you may have to use a workaround.
However, all workarounds that cannot be statically resolved involve explicit casting (typically `static_cast`) and are error-prone.
You will basically be crafting your own special-purpose `dynamic_cast`.
So, first make sure that your `dynamic_cast` really is as slow as you think it is (there are a fair number of unsupported rumors about)
and that your use of `dynamic_cast` is really performance critical.

**Enforcement**: Flag all uses of `static_cast` for downcasts, including C-style casts that perform a `static_cast`.


<a name="Rh-ptr-cast"></a>
### C.147: Use `dynamic_cast` to a reference type when failure to find the required class is considered an error

**Reason**: Casting to a reference expresses that you intend to end up with a valid object, so the cast must succeed. `dynamic_cast` will then throw if it does not succeed.

**Example**:

	???

**Enforcement**: ???


<a name="Rh-ref-cast"></a>
### C.148: Use `dynamic_cast` to a pointer type when failure to find the required class is considered a valid alternative

**Reason**: ???

**Example**:

	???

**Enforcement**: ???


<a name="Rh-smart"></a>
### C.149: Use `unique_ptr` or `shared_ptr` to avoid forgetting to `delete` objects created using `new`

**Reason**: Avoid resource leaks.

**Example**:

	void use(int i)
	{
		auto p = new int {7};			// bad: initialize local pointers with new
		auto q = make_unique<int>(9);	// ok: guarantee the release of the memory allocated for 9
		if(0<i) return;	// maybe return and leak
		delete p;		// too late
	}

**Enforcement**:

* Flag initialization of a naked pointer with the result of a `new`
* Flag `delete` of local variable


<a name="Rh-make_unique"></a>
### C.150: Use `make_unique()` to construct objects owned by `unique_ptr`s or other smart pointers

**Reason**: `make_unique` gives a more concise statement of the construction.

**Example**:

	unique_ptr<Foo> p {new<Foo>{7});	// OK: but repetitive

	auto q = make_unique<Foo>(7);		// Better: no repetition of Foo

**Enforcement**:

* Flag the repetitive usage of template specialization list `<Foo>`
* Flag variables declared to be `unique_ptr<Foo>`


<a name="Rh-make_shared"></a>
### C.151: Use `make_shared()` to construct objects owned by `shared_ptr`s

**Reason**: `make_shared` gives a more concise statement of the construction.
It also gives an opportunity to eliminate a separate allocation for the reference counts, by placing the `shared_ptr`'s use counts next to its object.

**Example**:

	shared_ptr<Foo> p {new<Foo>{7});	// OK: but repetitive; and separate allocations for the Foo and shared_ptr's use count

	auto q = make_shared<Foo>(7);		// Better: no repetition of Foo; one object

**Enforcement**:

* Flag the repetive usage of template specialization list`<Foo>`
* Flag variables declared to be `shared_ptr<Foo>`


<a name="Rh-array"></a>
### C.152: Never assign a pointer to an array of derived class objects to a pointer to its base

**Reason**: Subscripting the resulting base pointer will lead to invalid object access and probably to memory corruption.

**Example**:

	struct B { int x; };
	struct D : B { int y; };

	void use(B*);

	D a[] = { {1,2}, {3,4}, {5,6} };
	B* p = a;	// bad: a decays to &a[0] which is converted to a B*
	p[1].x = 7;	// overwrite D[0].y

	use(a);		// bad: a decays to &a[0] which is converted to a B*

**Enforcement**:

* Flag all combinations of array decay and base to derived conversions.
* Pass an array as an `array_view` rather than as a pointer, and don't let the array name suffer a derived-to-base conversion before getting into the `array_view`


<a name="SS-overload"></a>
# C.over: Overloading and overloaded operators

You can overload ordinary functions, template functions, and operators.
You cannot overload function objects.

Overload rule summary:

* [C.160: Define operators primarily to mimic conventional usage](#Ro-conventional)
* [C.161: Use nonmember functions for symmetric operators](#Ro-symmetric)
* [C.162: Overload operations that are roughly equivalent](#Ro-equivalent)
* [C.163: Overload only for operations that are roughly equivalent](#Ro-equivalent-2)
* [C.164: Avoid conversion operators](#Ro-conversion)
* [C.170: If you feel like overloading a lambda, use a generic lambda](#Ro-lambda)

<a name="Ro-conventional"></a>
### C.140: Define operators primarily to mimic conventional usage

**Reason**: Minimize surprises.

**Example, bad**:

	X operator+(X a, X b) { return a.v-b.v; }	// bad: makes + subtract

???. Non-member operators: namespace-level definition (traditional?) vs friend definition (as used by boost.operator, limits lookup to ADL only)

**Enforcement**: Possibly impossible.


<a name="Ro-symmetric"></a>
### C.141: Use nonmember functions for symmetric operators

**Reason**: If you use member functions, you need two.
Unless you use a non-member function for (say) `==`, `a==b` and `b==a` will be subtly different.

**Example**:

	bool operator==(Point a, Point b) { return a.x==b.x && a.y==b.y; }

**Enforcement**: Flag member operator functions.


<a name="Ro-equivalent"></a>
### C.142: Overload operations that are roughly equivalent

**Reason**: Having different names for logically equivalent operations on different argument types is confusing, leads to encoding type information in function names, and inhibits generic programming.

**Example**: Consider

	void print(int a);
	void print(int a, int base);
	void print(const string&);

These three functions all prints their arguments (appropriately). Conversely

	void print_int(int a);
	void print_based(int a, int base);
	void print_string(const string&);

These three functions all prints their arguments (appropriately). Adding to the name just introduced verbosity and inhibits generic code.

**Enforcement**: ???


<a name="Ro-equivalent-2"></a>
### C.143: Overload only for operations that are roughly equivalent

**Reason**: Having the same name for logically different functions is confusing and leads to errors when using generic programming.

**Example**: Consider
	
	void open_gate(Gate& g);	// remove obstacle from garage exit lane
	void fopen(const char*name, const char* mode);	// open file

The two operations are fundamentally different (and unrelated) so it is good that their names differ. Conversely:

	void open(Gate& g);	// remove obstacle from garage exit lane
	void open(const char*name, const char* mode ="r");	// open file

The two operations are still fundamentally different (and unrelated) but the names have been reduced to their (common) minimum, opening opportunities for confusion.
 Fortunately, the type system will catch many such mistakes.

**Note**: be particularly careful about common and popular names, such as `open`, `move`, `+`, and `==`.

**Enforcement**: ???


<a name="Ro-conversion"></a>
### C.144: Avoid conversion operators

**Reason**: Implicit conversions can be essential (e.g., `double` to '`int`) but often cause surprises (e.g., `String` to C-style string).

**Note**: Prefer explicitly named conversions until a serious need is demonstrated.
By "serious need" we mean a reason that is fundamental in the application domain (such as an integer to complex number conversion)
and frequently needed. Do not introduce implicit conversions (through conversion operators or non-`explicit` constructors)
just to gain a minor convenience.

**Example, bad**:

	class String {	// handle ownership and access to a sequence of characters
		// ...
		String(czstring p); // copy from *p to *(this->elem)
		// ...
		operator zstring() { return elem; }
		// ...
	};
	
	void user(zstring p)
	{
		if (*p=="") {
			String s {"Trouble ahead!"};
			// ...
			p = s;
		}
		// use p
	}

The string allocated for `s` and assigned to `p` is destroyed before it can be used.

**Enforcement**: Flag all conversion operators.

<a name="Ro-lambda"></a>
### C.170: If you feel like overloading a lambda, use a generic lambda

**Reason**: You can overload by defining two different lambdas with the same name

**Example**:

	void f(int);
	void f(double);
	auto f = [](char);	// error: cannot overload variable and function
	
	auto g = [](int) { /* ... */ };
	auto g = [](double) { /* ... */ };	// error: cannot overload variables
	
	auto h = [](auto) { /* ... */ };	// OK
	
**Enforcement**: The compiler catches attempt to overload a lambda.


<a name="SS-union"></a>
## C.union: Unions

???

Union rule summary:

* [C.180: Use `union`s to ???](#Ru-union)
* [C.181: Avoid "naked" `union`s](#Ru-naked)
* [C.182: Use anonymous `union`s to implement tagged unions](#Ru-anonymous)
* ???


<a name="Ru-union"></a>
### C.180: Use `union`s to ???

??? When should unions be used, if at all? What's a good future-proof way to re-interpret object representations of PODs?
??? variant

**Reason**: ???

**Example**:

	???

**Enforcement**: ???


<a name="Ru-naked"></a>
### C.181: Avoid "naked" `union`s

**Reason**: Naked unions are a source of type errors.

**Alternative**: Wrap them in a class together with a type field.

**Alternative**: Use `variant`.

**Example**:

	???

**Enforcement**: ???



<a name="Ru-anonymous"></a>
### C.182: Use anonymous `union`s to implement tagged unions

**Reason**: ???

**Example**:

	???

**Enforcement**: ???



<a name="S-enum"></a>
# Enum: Enumerations

Enumerations are used to define sets of integer values and for defining types for such sets of values. There are two kind of enumerations, "plain" `enum`s and `class enum`s.

Enumeration rule summary:

* [Enum.1: Prefer enums over macros](#Renum-macro)
* [Enum.2: Use enumerations to represent sets of named constants](#Renum-set)
* [Enum.3: Prefer class enums over ``plain'' enums](#Renum-class)
* [Enum.4: Define operations on enumerations for safe and simple use](#Renum-oper)
* [Enum.5: Don't use ALL_CAPS for enumerators](#Renum-caps)
* [Enum.6: Use unnamed enumerations for ???](#Renum-unnamed)
* ???


<a name="Renum-macro"></a>
### Enum.1: Prefer enums over macros

**Reason**: Macros do not obey scope and type rules.

**Example**:

	???

**Enforcement**: ???


<a name="Renum-set"></a>
### Enum.2: Use enumerations to represent sets of named constants

**Reason**: ???

**Example**:

	???

**Enforcement**: ???


<a name="Renum-class"></a>
### Enum.3: Prefer class enums over ``plain'' enums

**Reason**: to minimize surprises

**Example**:

	???

**Enforcement**: ???


<a name="Renum-oper"></a>
### Enum.4: Define operations on enumerations for safe and simple use

**Reason**: Convenience of us and avoidance of errors.

**Example**:

	???

**Enforcement**: ???


<a name="Renum-caps"></a>
### Enum.5: Don't use ALL_CAPS for enumerators

**Reason**: Avoid clashes with macros

**Example**:

	???

**Enforcement**: ???


<a name="Renum-unnamed"></a>
### Enum.6: Use unnamed enumerations for ???

**Reason**: ???

**Example**:

	???

**Enforcement**: ???



<a name="S-resource"></a>
# R: Resource management

This section contains rules related to resources.
A resource is anything that must be acquired and (explicitly or implicitly) released, such as memory, file handles, sockets, and locks.
The reason it must be released is typically that it can be in short supply, so even delayed release may do harm.
The fundamental aim is to ensure that we don't leak any resources and that we don't hold a resource longer than we need to.
An entity that is responsible for releasing a resource is called an owner.

There are a few cases where leaks can be acceptable or even optimal:
if you are writing a program that simply produces an output based on an input and the amount of memory needed is proportional to the size of the input,
the optimal strategy (for performance and ease of programming) is sometimes simply never to delete anything.
If you have enough memory to handle your largest input, leak away, but be sure to give a good error message if you are wrong.
Here, we ignore such cases.

Resource management rule summary:

* [R.1: Manage resources automatically using resource handles and RAII (resource acquisition is initialization)](#Rr-raii)
* [R.2: In interfaces, use raw pointers to denote individual objects (only)](#Rr-use-ptr)
* [R.3: A raw pointer (a `T*`) is non-owning](#Rr-ptr)
* [R.4: A raw reference (a `T&`) is non-owning](#Rr-ref)
* [R.5: Prefer scoped objects](#Rr-scoped)
* [R.6: Avoid non-`const` global variables](#Rr-global)
	
Alocation and deallocation rule summary:
	
* [R.10: Avoid `malloc()` and `free()`](#Rr-mallocfree)
* [R.11: Avoid calling `new` and `delete` explicitly](#Rr-newdelete)
* [R.12: Immediately give the result of an explicit resource allocation to a manager object](#Rr-immediate-alloc)
* [R.13: Perform at most one explicit resource allocation in a single expression statement](#Rr-single-alloc)
* [R.14: ??? array vs. pointer parameter](#Rr-ap)
* [R.15: Always overload matched allocation/deallocation pairs](#Rr-pair)
	
<a name ="Rr-summary-smartptrs"></a>Smart pointer rule summary:

* [R.20: Use `unique_ptr` or `shared_ptr` to represent ownership](#Rr-owner)
* [R.21: Prefer `unique_ptr` over `shared_ptr` unless you need to share ownership](#Rr-unique)
* [R.22: Use `make_shared()` to make `shared_ptr`s](#Rr-make_shared)
* [R.23: Use `make_unique()` to make `unique_ptr`s](#Rr-make_unique)
* [R.24: Use `std::weak_ptr` to break cycles of `shared_ptr`s](#Rr-weak_ptr)
* [R.30: Take smart pointers as parameters only to explicitly express lifetime semantics](#Rr-smartptrparam)
* [R.31: If you have non-`std` smart pointers, follow the basic pattern from `std`](#Rr-smart)
* [R.32: Take a `unique_ptr<widget>` parameter to express that a function assumes ownership of a `widget`](#Rr-uniqueptrparam)
* [R.33: Take a `unique_ptr<widget>&` parameter to express that a function reseats the`widget`](#Rr-reseat)
* [R.34: Take a `shared_ptr<widget>` parameter to express that a function is part owner](#Rr-sharedptrparam-owner)
* [R.35: Take a `shared_ptr<widget>&` parameter to express that a function might reseat the shared pointer](#Rr-sharedptrparam)
* [R.36: Take a `const shared_ptr<widget>&` parameter to express that it might retain a reference count to the object ???](#Rr-sharedptrparam-const&)
* [R.37: Do not pass a pointer or reference obtained from an aliased smart pointer](#Rr-smartptrget)


<a name ="Rr-raii"></a>
### Rule R.1: Manage resources automatically using resource handles and RAII (resource acquisition is initialization)

**Reason**: To avoid leaks and the complexity of manual resource management.
 C++'s language-enforced constructor/destructor symmetry mirrors the symmetry inherent in resource acquire/release function pairs such as `fopen`/`fclose`, `lock`/`unlock`, and `new`/`delete`.
 Whenever you deal with a resource that needs paired acquire/release function calls,
 encapsulate that resource in an object that enforces pairing for you -- acquire the resource in its constructor, and release it in its destructor.

**Example, bad**: Consider

	void send( X* x, cstring_view destination ) {
        auto port = OpenPort(destination);
        my_mutex.lock();
        // ...
        Send(port, x);
        // ...
        my_mutex.unlock();
        ClosePort(port);
        delete x;
	}

In this code, you have to remember to `unlock`, `ClosePort`, and `delete` on all paths, and do each exactly once.
Further, if any of the code marked `...` throws an exception, then `x` is leaked and `my_mutex` remains locked.

**Example**: Consider

	void send( unique_ptr<X> x, cstring_view destination ) { // x owns the X
        Port port{destination};            // port owns the PortHandle
        lock_guard<mutex> guard{my_mutex}; // guard owns the lock
        // ...
        Send(port, x);
        // ...
	} // automatically unlocks my_mutex and deletes the pointer in x

Now all resource cleanup is automatic, performed once on all paths whether or not there is an exception. As a bonus, the function now advertises that it takes over ownership of the pointer.

What is `Port`? A handy wrapper that encapsulates the resource:

    class Port {
        PortHandle port;
    public:
        Port( cstring_view destination ) : port{OpenPort(destination)} { }
        ~Port() { ClosePort(port); }
        operator PortHandle() { return port; }

        // port handles can't usually be cloned, so disable copying and assignment if necessary
        Port(const Port&) =delete;
        Port& operator=(const Port&) =delete;
    };

**Note**: Where a resource is "ill-behaved" in that it isn't represented as a class with a destructor, wrap it in a class or use [`finally`](#S-gsl)

**See also**: [RAII](#Rr-raii).


<a name ="Rr-use-ptr"></a>
### R.2: In interfaces, use raw pointers to denote individual objects (only)

**Reason**: Arrays are best represented by a container type (e.g., `vector` (owning)) or an `array_view` (non-owning).
Such containers and views hold sufficient information to do range checking.

**Example, bad**:

	void f(int* p, int n)	// n is the number of elements in p[]
	{
		// ...
		p[2] = 7;	// bad: subscript raw pointer
		// ...
	}

The compiler does not read comments, and without reading other code you do not know whether `p` really points to `n` elements.
Use an `array_view` instead.

**Example**:

	void g(int* p, int fmt)	// print *p using format #fmt
	{
		// ... uses *p and p[0] only ...
	}
	
**Exception**: C-style strings are passed as single pointers to a zero-terminated sequence of characters.
Use `zstring` rather than `char*` to indicate that you rely on that convention.

**Note**: Many current uses of pointers to a single element could be references.
However, where `nullptr` is a possible value, a reference may not be an reasonable alternative.

**Enforcement**:

* Flag pointer arithmetic (including `++`) on a pointer that is not part of a container, view, or iterator.
This rule would generate a huge number of false positives if applied to an older code base.
* Flag array names passed as simple pointers



<a name="Rr-ptr"></a>
### R.3: A raw pointer (a `T*`) is non-owning

**Reason**: There is nothing (in the C++ standard or in most code) to say otherwise and most raw pointers are non-owning.
We want owning pointers identified so that we can reliably and efficiently delete the objects pointed to by owning pointers.

**Example**:

	void f()
	{
		int* p1 = new int{7};			// bad: raw owning pointer
		auto p2 = make_unique<int>(7);	// OK: the int is owned by a unique pointer
		// ...
	}

The `unique_ptr` protects against leaks by guaranteeing the deletion of its object (even in the presence of exceptions). The `T*` does not.

**Example**:

	template<typename T>
	class X {
		// ...
	public:
		T* p;	// bad: it is unclear whether p is owning or not
		T* q;	// bad: it is unclear whether q is owning or not
	};

We can fix that problem by making ownership explicit:

	template<typename T>
	class X2 {
		// ...
	public:
		owner<T> p;	// OK: p is owning
		T* q;		// OK: q is not owning
	};

**Note**: The fact that there are billions of lines of code that violates this rule against owning `T*`s cannot be ignored.
This code cannot all be rewritten (ever assuming good code transformation software).
This problem cannot be solved (at scale) by transforming all owning pointer to `unique_ptr`s and `shared_ptr`s, partly because we need/use owning "raw pointers" in the implementation of our fundamental resource handles. For example, most `vector` implementations have one owning pointer and two non-owning pointers.
Also, many ABIs (and essentially all interfaces to C code) use `T*`s, some of them owning.

**Note**: `owner<T>` has no default semantics beyond `T*` it can be used without changing any code using it and without affecting ABIs.
It is simply a (most valuable) indicator to programmers and analysis tools.
For example, if an `owner<T>` is a member of a class, that class better have a destructor that `delete`s it.
	
**Example**, bad:
Returning a (raw) pointer imposes a life-time management burden on the caller; that is, who deletes the pointed-to object?

	Gadget* make_gadget(int n)
	{
		auto p = new Gadget{n};
		// ...
		return p;
    }
	
	void caller(int n)
	{
		auto p = make_gadget(n);	// remember to delete p
		// ...
		delete p;
	}

In addition to suffering from then problem from [leak](#???), this adds a spurious allocation and deallocation operation,
and is needlessly verbose. If Gadget is cheap to move out of a function (i.e., is small or has an efficient move operation),
just return it "by value:'

	Gadget make_gadget(int n)
	{
		Gadget g{n};
		// ...
		return g;
	}

**Note**: This rule applies to factory functions.

**Note**: If pointer semantics is required (e.g., because the return type needs to refer to a base class of a class hierarchy (an interface)),
return a "smart pointer."

**Enforcement**:

* (Simple) Warn on `delete` of a raw pointer that is not an `owner<T>`.
* (Moderate) Warn on failure to either `reset` or explicitly `delete` an `owner<T>` pointer on every code path.
* (Simple) Warn if the return value of `new` or a function call with return value of pointer type is assigned to a raw pointer.
* (Simple) Warn if a function returns an object that was allocated within the function but has a move constructor.
Suggest considering returning it by value instead.


<a name="Rr-ref"></a>
### R.4: A raw reference (a `T&`) is non-owning

**Reason**: There is nothing (in the C++ standard or in most code) to say otherwise and most raw references are non-owning.
We want owners identified so that we can reliably and efficiently delete the objects pointed to by owning pointers.

**Example**:

	void f()
	{
		int& r = *new int{7};			// bad: raw owning reference
		// ...
		delete &r;						// bad: violated the rule against deleting raw pointers
	}
	
**See also**: [The raw pointer rule](#Rr-ptr)

**Enforcement**: See [the raw pointer rule](#Rr-ptr)


<a name="Rr-scoped"></a>
### R.5: Prefer scoped objects

**Reason**: A scoped object is a local object, a global object, or a member.
This implies that there is no separate allocation and deallocation cost in excess that already used for the containing scope or object.
The members of a scoped object are themselves scoped and the scoped object's constructor and destructor manage the members' lifetimes.

**Example**: the following example is inefficient (because it has unnecessary allocation and deallocation), vulnerable to exception throws and returns in the "¦ part (leading to leaks), and verbose:

	void some_function(int n)
	{
		auto p = new Gadget{n};
		// ...
		delete p;
	}

Instead, use a local variable:

	void some_function(int n)
	{
		Gadget g{n};
		// ...
	}

**Enforcement**:

* (Moderate) Warn if an object is allocated and then deallocated on all paths within a function. Suggest it should be a local `auto` stack object instead.
* (Simple) Warn if a local `Unique_ptr` or `Shared_ptr` is not moved, copied, reassigned or `reset` before its lifetime ends.



<a name="Rr-global"></a>
### R.6: Avoid non-`const` global variables

**Reason**: Global variables can be accessed from everywhere so they can introduce surprising dependencies between apparently unrelated objects.
They are a notable source of errors.

**Warning**: The initialization of global objects is not totally ordered. If you use a global object initialize it with a constant.

**Exception**: a global object is often better than a singleton.

**Exception**: An immutable (`const`) global does not introduce the problems we try to avoid by banning global objects.

**Enforcement**: [[??? NM: Obviously we can warn about non-const statics....do we want to?]]


<a name="SS-alloc"></a>
## R.alloc: Alocation and deallocation


<a name ="Rr-mallocfree"></a>
### R.10: Avoid `malloc()` and `free()`

**Reason**: `malloc()` and `free()` do not support construction and destruction, and do not mix well with `new` and `delete`.

**Example**:

		class Record {
			int id;
			string name;
			// ...
		};
		
		void use()
		{
			Record* p1 = static_cast<Record*>(malloc(sizeof(Record)));
			// p1 may be nullptr
			// *p1 is not initialized; in particular, that string isn't a string, but a string-sizes bag of bits
			
			auto p2 = new Record;
			
			// unless an exception is thrown, *p2 is default initialized
			auto p3 = new(nothrow) Record;
			// p3 may be nullptr; if not, *p2 is default initialized
			
			// ...
			
			delete p1;	// error: cannot delete object allocated by malloc()
			free(p2);	// error: cannot free() object allocatedby new
		}

In some implementaions that `delete` and that `free()` might work, or maybe they will cause run-time errors.
		
**Exception**: There are applications and sections of code where exceptions are not acceptable.
Some of the best such example are in life-critical hard real-time code.
Beware that many bans on exception use are based on superstition (bad)
or by concerns for older code bases with unsystematics resource management (unfortunately, but sometimes necessary).
In such cases, consider the `nothrow` versions of `new`.

**Enforcement**: Flag explicit use of `malloc` and `free`.


<a name ="Rr-newdelete"></a>
### R.11: Avoid calling `new` and `delete` explicitly

**Reason**: The pointer returned by `new` should belong to a resource handle (that can call `delete`).
If the pointer returned from `new` is assigned to a plain/naked pointer, the object can be leaked.

**Note**: In a large program, a naked `delete` (that is a `delete` in application code, rather than part of code devoted to resource management)
is a likely bug: if you have N `delete`s, how can you be certain that you don't need N+1 or N-1?
The bug may be latent: it may emerge only during maintenace.
If you have a naked `new`, you probably need a naked `delete` somewhere, so you probably have a bug.

**Enforcement**: (Simple) Warn on any explicit use of `new` and `delete`. Suggest using `make_unique` instead.


<a name ="Rr-immediate-alloc"></a>
### R.12: Immediately give the result of an explicit resource allocation to a manager object

**Reason**: If you don't, an exception or a return may lead to a leak.

**Example, bad**:

	void f(const string& name)
	{
		FILE* f = fopen(name,"r");			// open the file
		vector<char> buf(1024);
		auto _ = finally([] { fclose(f); }	// remember to close the file
		// ...
	}

The allocation of `buf` may fail and leak the file handle.

**Example**:

	void f(const string& name)
	{
		ifstream {name,"r"};			// open the file
		vector<char> buf(1024);
		// ...
	}

The use of the file handle (in `ifstream`) is simple, efficient, and safe.

**Enforcement**:

* Flag explicit allocations used to initialize pointers  (problem: how many direct resource allocations can we recognize?)


<a name ="Rr-single-alloc"></a>
### R.13: Perform at most one explicit resource allocation in a single expression statement

**Reason**: If you perform two explicit resource allocations in one statement,
you could leak resources because the order of evaluation of many subexpressions, including function arguments, is unspecified.

**Example**:

    void fun( shared_ptr<Widget> sp1, shared_ptr<Widget> sp2 );

This `fun` can be called like this:

    fun( shared_ptr<Widget>(new Widget(a,b)), shared_ptr<Widget>(new Widget(c,d)) );	// BAD: potential leak

This is exception-unsafe because the compiler may reorder the two expressions building the function's two arguments.
In particular, the compiler can interleave execution of the two expressions:
Memory allocation (by calling `operator new`) could be done first for both objects, followed by attempts to call the two `Widget` constructors.
If one of the constructor calls throws an exception, then the other object's memory will never be released!

This subtle problem has a simple solution: Never perform more than one explicit resource allocation in a single expression statement.
For example:

	    shared_ptr<Widget> sp1(new Widget(a,b)); // Better, but messy
	    fun( sp1, new Widget(c,d) );

The best solution is to avoid explicit allocation entirely use factory functions that return owning objects:

	    fun( make_shared<Widget>(a,b), make_shared<Widget>(c,d) ); // Best

Write your own factory wrapper if there is not one already.
  
**Enforcement**:

* Flag expressions with multiple explicit resource allocations (problem: how many direct resource allocations can we recognize?)


<a name ="Rr-ap"></a>
### R.14: ??? array vs. pointer parameter

**Reason**: An array decays to a pointer, thereby losing its size, opening the opportunity for range errors.

**Example**:

	??? what do we recommend: f(int*[]) or f(int**) ???

**Alternative**: Use `array_view` to preserve size information.

**Enforcement**: Flag `[]` parameters.


<a name="Rr-pair"></a>
### R.15: Always overload matched allocation/deallocation pairs

**Reason**. Otherwise you get mismatched operations and chaos.

**Example**:

		class X {
			// ...
			void* operator new(size_t s);
			void operator delete(void*);
			// ...
		};
		
**Note**: If you want memory that cannot be deallocated, `=delete` the deallocation operation.
Don't leave it undeclared.

**Enforcement**: Flag incomplate pairs.


<a name="SS-smart"></a>
## R.smart: Smart pointers


<a name="Rr-owner"></a>
### Rule R.20: Use `unique_ptr` or `shared_ptr` to represent ownership

**Reason**: They can prevent resource leaks.

**Example**: Consider

	void f()
	{
		X x;
		X* p1 { new X };			// see also ???
		unique_ptr<T> p2 { new X };	// unique ownership; see also ???
		shared_ptr<T> p3 { new X };	// shared ownership; see also ???
	}

This will leak the object used to initialize `p1` (only).

**Enforcement:** (Simple) Warn if the return value of `new` or a function call with return value of pointer type is assigned to a raw pointer.


<a name="Rr-unique"></a>
### Rule R.21: Prefer `unique_ptr` over `shared_ptr` unless you need to share ownership

**Reason**: a `unique_ptr` is conceptually simpler and more predictable (you know when destruction happens) and faster (you don't implicitly maintain a use count).

**Example, bad**: This needlessly adds and maintains a reference count

    void f()
    {
        shared_ptr<Base> base = make_shared<Derived>();
        // use base locally, without copying it -- refcount never exceeds 1
    } // destroy base

**Example**: This is more efficient

    void f()
    {
        unique_ptr<Base> base = make_unique<Derived>();
        // use base locall
    } // destroy base


**Enforcement**: (Simple) Warn if a function uses a `Shared_ptr` with an object allocated within the function, but never returns the `Shared_ptr` or passes it to a function requiring a `Shared_ptr&`. Suggest using `unique_ptr` instead.


<a name ="Rr-make_shared"></a>
### R.22: Use `make_shared()` to make `shared_ptr`s

**Reason**: If you first make an object and then gives it to a `shared_ptr` constructor, you (most likely) do one more allocation (and later deallocation) than if you use `make_shared()` because the reference counts must be allocated separately from the object.

**Example**: Consider

	shared_ptr<X> p1 { new X{2} }; // bad
	auto p = make_shared<X>(2);    // good

The `make_shared()` version mentions `X` only once, so it is usually shorter (as well as faster) than the version with the explicit `new`.

**Enforcement**: (Simple) Warn if a `shared_ptr` is constructed from the result of `new` rather than `make_shared`.


<a name ="Rr-make_unique"></a>
### Rule R.23: Use `make_unique()` to make `unique_ptr`s

**Reason**: for convenience and consistency with `shared_ptr`.

**Note**: `make_unique()` is C++14, but widely available (as well as simple to write).

**Enforcement**: (Simple) Warn if a `Shared_ptr` is constructed from the result of `new` rather than `make_unique`.


<a name ="Rr-weak_ptr"></a>
### R.30: Use `std::weak_ptr` to break cycles of `shared_ptr`s

**Reason**: `shared_ptr's rely on use counting and the use count for a cyclic structure never goes to zero, so we need a mechanism to
be able to destroy a cyclic structure.

**Example**:

	???

**Note**:  ??? [[HS: A lot of people say "to break cycles", while I think "temporary shared ownership" is more to the point.]]
???[[BS: breaking cycles is what you must do; temporarily sharing ownership is how you do it.
You could "temporarily share ownership simply by using another `stared_ptr`.]]

**Enforcement**: ???probably impossible. If we could statically detect cycles, we wouldn't need `weak_ptr`


<a name="Rr-smart"></a>
### R.31: If you have non-`std` smart pointers, follow the basic pattern from `std`

**Reason**: The rules in the following section also work for other kinds of third-party and custom smart pointers and are very useful for diagnosing common smart pointer errors that cause performance and correctness problems.
You want the rules to work on all the smart pointers you use.

Any type (including primary template or specialization) that overloads unary `*` and `->` is considered a smart pointer:

* If it is copyable, it is recognized as a reference-counted `Shared_ptr`.
* If it not copyable, it is recognized as a unique `Unique_ptr`.

**Example**:

    // use Boost's intrusive_ptr
    #include <boost/intrusive_ptr.hpp>
    void f(boost::intrusive_ptr<widget> p) { 	// error under rule 'sharedptrparam'
        p->foo();
    }

    // use Microsoft's CComPtr
	#include <atlbase.h>
    void f(CComPtr<widget> p) {                // error under rule 'sharedptrparam'
        p->foo();
    }

Both cases are an error under the [`sharedptrparam` guideline](#Rr-smartptrparam):
`p` is a `Shared_ptr`, but nothing about its sharedness is used here and passing it by value is a silent pessimization;
these functions should accept a smart pointer only if they need to participate in the widget's lifetime management. Otherwise they should accept a `widget*`, if it can be `nullptr`. Otherwise, and ideally, the function should accept a `widget&`.
These smart pointers match the `Shared_ptr` concept,
so these guideline enforcement rules work on them out of the box and expose this common pessimization.


<a name="Rr-smartptrparam"></a>
### R.32: Take smart pointers as parameters only to explicitly express lifetime semantics

**Reason**: Accepting a smart pointer to a `widget` is wrong if the function just needs the `widget` itself.
It should be able to accept any `widget` object, not just ones whose lifetimes are managed by a particular kind of smart pointer.
A function that does not manipulate lifetime should take raw pointers or references instead.

**Example; bad**:

    // callee
    void f( shared_ptr<widget>& w ) {
        // ...
        use( *w ); // only use of w -- the lifetime is not used at all
        // ...
    };

    // caller
    shared_ptr<widget> my_widget = /*...*/;
    f( my_widget );

    widget stack_widget;
    f( stack_widget ); // error

**Example; good**:

    // callee
    void f( widget& w ) {
        // ...
        use( w );
        // ...
    };

    // caller
    shared_ptr<widget> my_widget = /*...*/;
    f( *my_widget );

    widget stack_widget;
    f( stack_widget ); // ok -- now this works

**Enforcement**:

* (Simple) Warn if a function takes a parameter of a type that is a `Unique_ptr` or `Shared_ptr` and the function only calls any of: `operator*`, `operator->` or `get()`).
	Suggest using a `T*` or `T&` instead.


<a name="Rr-uniqueptrparam"></a>
### R.33: Take a `unique_ptr<widget>` parameter to express that a function assumes ownership of a `widget`

**Reason**: Using `unique_ptr` in this way both documents and enforces the function call's ownership transfer.

**Example**:

	void sink(unique_ptr<widget>); // consumes the widget

    void sink(widget*); 			// just uses the widget

**Example; bad**:

    void thinko(const unique_ptr<widget>&); // usually not what you want
		

**Enforcement**:

* (Simple) Warn if a function takes a `Unique_ptr<T>` parameter by lvalue reference and does not either assign to it or call `reset()` on it on at least one code path. Suggest taking a `T*` or `T&` instead.
* (Simple) ((Foundation)) Warn if a function takes a `Unique_ptr<T>` parameter by reference to `const`. Suggest taking a `const T*` or `const T&` instead.
* (Simple) ((Foundation)) Warn if a function takes a `Unique_ptr<T>` parameter by rvalue reference. Suggest using pass by value instead.


<a name="Rr-reseat"></a>
### R.34: Take a `unique_ptr<widget>&` parameter to express that a function reseats the`widget`

**Reason**: Using `unique_ptr` in this way both documents and enforces the function call's reseating semantics.

**Note**: "reseat" means "making a reference or a smart pointer refer to a different object."

**Example**:

    void reseat( unique_ptr<widget>& ); // "will" or "might" reseat pointer

**Example; bad**:

    void thinko( const unique_ptr<widget>& ); // usually not what you want
		

**Enforcement**:

* (Simple) Warn if a function takes a `Unique_ptr<T>` parameter by lvalue reference and does not either assign to it or call `reset()` on it on at least one code path. Suggest taking a `T*` or `T&` instead.
* (Simple) ((Foundation)) Warn if a function takes a `Unique_ptr<T>` parameter by reference to `const`. Suggest taking a `const T*` or `const T&` instead.
* (Simple) ((Foundation)) Warn if a function takes a `Unique_ptr<T>` parameter by rvalue reference. Suggest using pass by value instead.


<a name="Rr-sharedptrparam-owner"></a>
### R.35: Take a `shared_ptr<widget>` parameter to express that a function is part owner

**Reason**: This makes the function's ownership sharing explicit.

**Example; good**:

    void share( shared_ptr<widget> );            // share – "will" retain refcount

    void reseat( shared_ptr<widget>& );          // "might" reseat ptr

    void may_share( const shared_ptr<widget>& ); // "might" retain refcount

**Enforcement**:

* (Simple) Warn if a function takes a `Shared_ptr<T>` parameter by lvalue reference and does not either assign to it or call `reset()` on it on at least one code path. Suggest taking a `T*` or `T&` instead.
* (Simple) ((Foundation)) Warn if a function takes  a `Shared_ptr<T>` by value or by reference to `const` and does not copy or move it to another `Shared_ptr` on at least one code path. Suggest taking a `T*` or `T&` instead.
* (Simple) ((Foundation)) Warn if a function takes a `Shared_ptr<T>` by rvalue reference. Suggesting taking it by value instead.


<a name="Rr-sharedptrparam"></a>
### R.36: Take a `shared_ptr<widget>&` parameter to express that a function might reseat the shared pointer

**Reason**: This makes the function's reseating explicit.

**Note**: "reseat" means "making a reference or a smart pointer refer to a different object."

**Example; good**:

    void share( shared_ptr<widget> );            // share – "will" retain refcount

    void reseat( shared_ptr<widget>& );          // "might" reseat ptr

    void may_share( const shared_ptr<widget>& ); // "might" retain refcount

**Enforcement**:

* (Simple) Warn if a function takes a `Shared_ptr<T>` parameter by lvalue reference and does not either assign to it or call `reset()` on it on at least one code path. Suggest taking a `T*` or `T&` instead.
* (Simple) ((Foundation)) Warn if a function takes  a `Shared_ptr<T>` by value or by reference to `const` and does not copy or move it to another `Shared_ptr` on at least one code path. Suggest taking a `T*` or `T&` instead.
* (Simple) ((Foundation)) Warn if a function takes a `Shared_ptr<T>` by rvalue reference. Suggesting taking it by value instead.


<a name="Rr-sharedptrparam-const&"></a>
### R.37: Take a `const shared_ptr<widget>&` parameter to express that it might retain a reference count to the object ???

**Reason**: This makes the function's ??? explicit.

**Example; good**:

    void share( shared_ptr<widget> );            // share – "will" retain refcount

    void reseat( shared_ptr<widget>& );          // "might" reseat ptr

    void may_share( const shared_ptr<widget>& ); // "might" retain refcount

**Enforcement**:

* (Simple) Warn if a function takes a `Shared_ptr<T>` parameter by lvalue reference and does not either assign to it or call `reset()` on it on at least one code path. Suggest taking a `T*` or `T&` instead.
* (Simple) ((Foundation)) Warn if a function takes  a `Shared_ptr<T>` by value or by reference to `const` and does not copy or move it to another `Shared_ptr` on at least one code path. Suggest taking a `T*` or `T&` instead.
* (Simple) ((Foundation)) Warn if a function takes a `Shared_ptr<T>` by rvalue reference. Suggesting taking it by value instead.
	

<a name="Rr-smartptrget"></a>
### R.38: Do not pass a pointer or reference obtained from an aliased smart pointer
	
**Reason**: Violating this rule is the number one cause of losing reference counts and finding yourself with a dangling pointer.
Functions should prefer to pass raw pointers and references down call chains.
At the top of the call tree where you obtain the raw pointer or reference from a smart pointer that keeps the object alive.
You need to be sure that smart pointer cannot be inadvertently be reset or reassigned from within the call tree below

**Note**: To do this, sometimes you need to take a local copy of a smart pointer, which firmly keeps the object alive for the duration of the function and the call tree.

**Example**: Consider this code:

    // global (static or heap), or aliased local...
    shared_ptr<widget> g_p = ...;

    void f( widget& w ) {
		g();
		use(w);  // A
    }

    void g() {
		g_p = ... ; // oops, if this was the last shared_ptr to that widget, destroys the widget
	}

The following should not pass code review:

    void my_code() {
	    f( *g_p );	// BAD: passing pointer or reference obtained from a nonlocal smart pointer
	                //      that could be inadvertently reset somewhere inside f or it callees
		g_p->func(); // BAD: same reason, just passing it as a "this" pointer
	}

The fix is simple -- take a local copy of the pointer to "keep a ref count" for your call tree:

    void my_code() {
		auto pin = g_p; // cheap: 1 increment covers this entire function and all the call trees below us
		f( *pin );      // GOOD: passing pointer or reference obtained from a local unaliased smart pointer
		pin->func();    // GOOD: same reason
	}

**Enforcement**:

* (Simple) Warn if a pointer or reference obtained from a smart pointer variable (`Unique_ptr` or `Shared_ptr`) that is nonlocal, or that is local but potentially aliased, is used in a function call. If the smart pointer is a `Shared_ptr` then suggest taking a local copy of the smart pointer and obtain a pointer or reference from that instead.




<a name="S-expr"></a>
# ES: Expressions and Statements

Expressions and statements are the lowest and most direct way of expressing actions and computation. Declarations in local scopes are statements.

For naming, commenting, and indentation rules, see [NL: Naming and layout](#S-naming).

General rules:

* [ES.1: Prefer the standard library to other libraries and to "handcrafted code"](#Res-lib)
* [ES.2: Prefer suitable abstractions to direct use of language features](#Res-abstr)


Declaration rules:

* [ES.5: Keep scopes small](#Res-scope)
* [ES.6: Declare names in for-statement initializers and conditions to limit scope](#Res-cond)
* [ES.7: Keep common and local names short, and keep uncommon and nonlocal names longer](#Res-name-length)
* [ES.8: Avoid similar-looking names](#Res-name-similar)
* [ES.9: Avoid `ALL_CAPS` names](#Res-!CAPS)
* [ES.10: Declare one name (only) per declaration](#Res-name-one)
* [ES.11: Use `auto` to avoid redundant repetition of type names](#Res-auto)
* [ES.20: Always initialize an object](#Res-always)
* [ES.21: Don't introduce a variable (or constant) before you need to use it](#Res-introduce)
* [ES.22: Don't declare a variable until you have a value to initialize it with](#Res-init)
* [ES.23: Prefer the `{}`-initializer syntax](#Res-list)
* [ES.24: Use a `unique_ptr<T>` to hold pointers in code that may throw](#Res-unique)
* [ES.25: Declare an object `const` or `constexpr` unless you want to modify its value later on](#Res-const)
* [ES.26: Don't use a variable for two unrelated purposes](#Res-recycle)
* [ES.27: Use `std::array` or `stack_array` for arrays on the stack](#Res-stack)
* [ES.28: Use lambdas for complex initialization, especially of `const` variables](#Res-lambda-init)
* [ES.30: Don't use macros for program text manipulation](#Res-macros)
* [ES.31: Don't use macros for constants or "functions"](#Res-macros2)
* [ES.32: Use `ALL_CAPS` for all macro names](#Res-CAPS!)
* [ES.40: Don't define a (C-style) variadic function](#Res-ellipses)


Expression rules:

* [ES.40: Avoid complicated expressions](#Res-complicated)
* [ES.41: If in doubt about operator precedence, parenthesize](#Res-parens)
* [ES.42: Keep use of pointers simple and straightforward](#Res-ptr)
* [ES.43: Avoid expressions with undefined order of evaluation](#Res-order)
* [ES.44: Don't depend on order of evaluation of function arguments](#Res-order-fct)
* [ES.45: Avoid narrowing conversions](#Res-narrowing)
* [ES.46: Avoid "magic constants"; use symbolic constants](#Res-magic)
* [ES.47: Use `nullptr` rather than `0` or `NULL`](#Res-nullptr)
* [ES.48: Avoid casts](#Res-casts)
* [ES.49: If you must use a cast, use a named cast](#Res-casts-named)
* [ES.50: Don't cast away `const`](#Res-casts-const)
* [ES.55: Avoid the need for range checking](#Res-range-checking)
* [ES.60: Avoid `new` and `delete[]` outside resource management functions](#Res-new)
* [ES.61: delete arrays using `delete[]` and non-arrays using `delete`](#Res-del)
* [ES.62: Don't compare pointers into different arrays](#Res-arr2)



Statement rules:

* [ES.70: Prefer a `switch`-statement to an `if`-statement when there is a choice](#Res-switch-if)
* [ES.71: Prefer a range-`for`-statement to a `for`-statement when there is a choice](#Res-for-range)
* [ES.72: Prefer a `for`-statement to a `while`-statement when there is an obvious loop variable](#Res-for-while)
* [ES.73: Prefer a `while`-statement to a `for`-statement when there is no obvious loop variable](#Res-while-for)
* [ES.74: Prefer to declare a loop variable in the initializer part of as `for`-statement](#Res-for-init)
* [ES.75: Avoid `do`-statements](#Res-do)
* [ES.76: Avoid `goto`](#Res-goto)
* [ES.77: ??? `continue`](#Res-continue)
* [ES.78: ??? `break`](#Res-break)
* [ES.79: ??? `default`](#Res-default)
* [ES.85: Make empty statements visible](#Res-empty)

Arithmetic rules:

* [ES.100: Don't mix signed and unsigned arithmetic](#Res-mix)
* [ES.101: use unsigned types for bit manipulation](#Res-unsigned)
* [ES.102: Used signed types for arithmetic](#Res-signed)
* [ES.103: Don't overflow](#Res-overflow)
* [ES.104: Don't underflow](#Res-overflow)
* [ES.105: Don't divide by zero](#Res-zero)


<a name="Res-lib"></a>
### ES.1: Prefer the standard library to other libraries and to "handcrafted code"

**Reason**: Code using a library can be much easier to write than code working directly with language features, much shorter, tend to be of a higher level of abstraction, and the library code is presumably already tested.
The ISO C++ standard library is among the most widely know and best tested libraries.
It is available as part of all C++ Implementations.

**Example**:

	auto sum = accumulate(begin(a),end(a),0.0);	// good

a range version of `accumulate` would be even better

	auto sum = accumulate(v,0.0); // better

but don't hand-code a well-known algorithm

	int max = v.size();		// bad: verbose, purpose unstated
	double sum = 0.0;
	for (int i = 0; i<max; ++i)
		sum = sum+v[i];

**Exception**: Large parts of the standard library rely on dynamic allocation (free store). These parts, notably the containers but not the algorithms, are unsuitable for some hard-real time and embedded applications. In such cases, consider providing/using similar facilities, e.g.,  a standard-library-style container implemented using a pool allocator.

**Enforcement**: Not easy. ??? Look for messy loops, nested loops, long functions, absence of function calls, lack of use of non-built-in types. Cyclomatic complexity?


<a name="Res-abstr"></a>
### ES.2: Prefer suitable abstractions to direct use of language features

**Reason**: A "suitable abstraction" (e.g., library or class) is closer to the application concepts than the bare language, leads to shorter and clearer code, and is likely to be better tested.


**Example**:

	vector<string> read1(istream& is)	// good
	{
		vector<string> res;
		for (string s; is>>s; )
			res.push_back(s);
		return res;
	}

The more traditional and lower-level near-equivalent is longer, messier, harder to get right, and most likely slower:

	char** read2(istream& is, int maxelem, int maxstring, int* nread)	// bad: verbose and incomplete
	{
		auto res = new char*[maxelem];
		int elemcount = 0;
		while (is && elemcount<maxelem) {
			auto s = new char[maxstring];
			is.read(s,maxstring);
			res[elemcount++] = s;
		}
		nread = elemcount;
		return res;
	}

Once the checking for overflow and error handling has been added that code gets quite messy, and there is the problem remembering to `delete` the returned pointer and the C-style strings that array contains.

**Enforcement**: Not easy. ??? Look for messy loops, nested loops, long functions, absence of function calls, lack of use of non-built-in types. Cyclomatic complexity?


## ES.dcl: Declarations

A declaration is a statement. a declaration introduces a name into a scope and may cause the construction of a named object.


<a name="Res-scope"></a>
### ES.5: Keep scopes small

**Reason**: Readability. Minimize resource retention. Avoid accidental misuse of value.

**Alternative formulation**: Don't declare a name in an unnecessarily large scope.

**Example**:

	void use()
	{
		int i;									// bad: i is needlessly accessible after loop
		for (i=0; i<20; ++i) { /* ... */ }
		// no intended use of i here
		for (int i=0; i<20; ++i) { /* ... */ }	// good: i is local to for-loop

		if (auto pc = dynamic_cast<Circle*>(ps)) {	// good: pc is local to if-statement
			// ...deal with Circle ...
		}
		else {
			// ... handle error ...
		}
	}
	
**Example, bad**:

	void use(const string& name)
	{
		string fn = name+".txt";
		ifstream is {fn};
		Record r;
		is >> r;
		// ... 200 lines of code without intended use of fn or is ...
	}

This function is by most measure too long anyway, but the point is that the used by `fn` and the file handle held by `is`
are retained for much longer than needed and that unanticipated use of `is` and `fn` could happen later in the function.
In this case, it might be a good ide to factor out the read:

	void fill_record(Record& r, const string& name)
	{
		string fn = name+".txt";
		ifstream is {fn};
		Record r;
		is >> r;
	}
	
	void use(const string& name)
	{
		Record r;
		fill_record(r,name);
		// ... 200 lines of code ...
	}
	
I am assuming that `Record` is large and doesn't have a good move operation so that an out-parameter is preferable to returning a `Record`.

**Enforcement**:

* Flag loop variable declared outside a loop and not used after the loop
* Flag when expensive resources, such as file handles and locks are not used for N-lines (for some suitable N)


<a name="Res-cond"></a>
### ES.6: Declare names in for-statement initializers and conditions to limit scope

**Reason**: Readability. Minimize resource retention.

**Example**:

	void use()
	{
		for (string s; cin>>s; )
			v.push_back(s);
		
		for (int i=0; i<20; ++i) {	// good: i is local to for-loop
			//* ...
		}

		if (auto pc = dynamic_cast<Circle*>(ps)) {	// good: pc is local to if-statement
			// ...deal with Circle ...
		}
		else {
			// ... handle error ...
		}
	}

**Enforcement**:

* Flag loop variables declared before the loop and not used after the loop
* (hard) Flag loop variables declared before the loop and used after the loop for an unrelated purpose.


<a name="Res-name-length"></a>
### ES.7: Keep common and local names short, and keep uncommon and nonlocal names longer

**Reason**: Readability. Lowering the chance of clashes between unrelated non-local names.

**Example**: Conventional short, local names increase readability:

	template<typename T>							// good
	void print(ostream& os, const vector<T>& v)
	{
		for (int i = 0; i<v.end(); ++i)
			os << v[i] << '\n';
	}

An index is conventionally called `i` and there is no hint about the meaning of the vector in this generic function, so `v` is as good name as any. Compare

	template<typename Element_type>					// bad: verbose, hard to read
	void print(ostream& target_stream, const vector<Element_type>& current_vector)
	{
		for (int current_element_index = 0;
				current_element_index<current_vector.end();
				++current_element_index
			)
		target_stream << current_vector[i] << '\n';
	}

Yes, it is a caricature, but we have seen worse.

**Example**: Unconventional and short non-local names obscure code:

	void use1(const string& s)
	{
		// ...
		tt(s);		// bad: what is tt()?
		// ...
	}
	

Better, give non-local entities readable names:

	void use1(const string& s)
	{
		// ...
		trim_tail(s);		// better
		// ...
	}
	
Here, there is a chance that the reader knows what `trim_tail` means and that the reader can remember it after looking it up.

**Example, bad**: Argument names of large functions are de facto non-local and should be meaningful:

	void complicated_algorithm(vector<Record>&vr, const vector<int>& vi, map<string,int>& out)
		// read from events in vr (marking used Records) for the indices in vi placing (name,index) pairs into out
	{
		// ... 500 lines of code using vr, vi, and out ...
	}

We recommend keeping functions short, but that rule isn't universally adhered to and naming should reflect that.

**Enforcement**: Check length of local and non-local names. Also take function length into account.


<a name="Res-name-similar"></a>
### ES.8: Avoid similar-looking names

**Reason**: Such names slow down comprehension and increase the likelihood of error.

**Example**:

	if (readable(i1+l1+ol+o1+o0+ol+o1+I0+l0)) surprise();

**Enforcement**: Check names against a list of known confusing letter and digit combinations.


<a name="Res-!CAPS"></a>
### ES.9: Avoid `ALL_CAPS` names

**Reason**: Such names are commonly used for macros. Thus, ALL_CAPS name are vulnerable to unintended macro substitution.

**Example**:

	// somewhere in some header:
	#define NE !=

	// somewhere else in some other header:
	enum Coord { N, NE, NW, S, SE, SW, E, W };
	
	// somewhere third in some poor programmer's .cpp:
	switch (direction) {
	case N:
		// ...
	case NE:
		// ...
	// ...
	}

**Note**: Do not use `ALL_CAPS` for constants just because constants used to be macros.

**Enforcement**: Flag all uses of ALL CAPS. For older code, accept ALL CAPS for macro names and flag all non-ALL-CAPS macro names.


<a name="Res-name-one"></a>
### ES.10: Declare one name (only) per declaration

**Reason**: One-declaration-per line increases readability and avoid mistake related to the C/C++ grammar. It leaves room for a `//`-comment

**Example; bad**:

       char *p, c, a[7], *pp[7], **aa[10];	// yuck!

**Exception**: a function declaration can contain several function argument declarations.

**Example**:
	
	template <class InputIterator, class Predicate>
	bool any_of(InputIterator first, InputIterator last, Predicate pred);

or better using concepts

	bool any_of(InputIterator first, InputIterator last, Predicate pred);

**Example**:

	double scalbn(double x, int n);	 	// OK: x*pow(FLT_RADIX,n); FLT_RADIX is usually 2

or

	double scalbn(		// better: x*pow(FLT_RADIX,n); FLT_RADIX is usually 2
		double x; 	// base value
		int n;		// exponent
	);

or

	double scalbn(double base, int exponent);	// better: base*pow(FLT_RADIX,exponent); FLT_RADIX is usually 2

**Enforcement**: Flag non-function arguments with multiple declarators involving declarator operators (e.g., `int* p, q;`)


<a name="Res-auto"></a>
###  ES.11: Use `auto` to avoid redundant repetition of type names

**Reason**:

* Simple repetition is tedious and error prone.
* When you us `auto`, the name of the declared entity is in a fixed position in the declaration, increasing readability.
* In a template function declaration the return type can be a member type.

**Example**: Consider

	auto p = v.begin();	// vector<int>::iterator
	auto s = v.size();
	auto h = t.future();
	auto q = new int[s];
	auto f = [](int x){ return x+10; }

In each case, we save writing a longish, hard-to-remember type that the compiler already knows but a programmer could get wrong.

**Example**:

	template<class T>
		auto Container<T>::first() -> Iterator;	// Container<T>::Iterator

**Exception**: Avoid `auto` for initializer lists and in cases where you know exactly which type you want and where an initializer might require conversion.

**Example**:

	auto lst = { 1, 2, 3 };	// lst is an initializer list (obviously)
	auto x{1};	// x is an int (after correction of the C++14 standard; initializer_list in C++11)

**Note**: When concepts become available, we can (and should) be more specific about the type we are deducing:

	// ...
	ForwardIterator p = algo(x,y,z);

**Enforcement**: Flag redundant repetition of type names in a declaration.


<a name="Res-always"></a>
### ES.20: Always initialize an object

**Reason**: Avoid used-before-set errors and their associated undefined behavior.

**Example**:

	void use(int arg)	// bad: uninitialized variable
	{
		int i;
		// ...
		i = 7;	// initialize i
	}

No, `i=7` does not initialize `i`; it assigns to it. Also, `i` can be read in the `...` part. Better:

	void use(int arg)	// OK
	{
		int i = 7;		// OK: initialized
		string s;		// OK: default initialized
		// ...
	}

**Exception**: It you are declaring an object that is just about to be initialized from input, initializing it would cause a double initialization.
However, beware that this may leave uninitialized data beyond the input - and that has been a fertile source of errors and security breaches:

	constexpr int max = 8*1024;
	int buf[max];					// OK, but suspicious
	f.read(buf,max);

The cost of initializing that array could be significant in some situations.
However, such examples do tend to leave uninitialized variables accessible, so they should be treated with suspicion.
	
	constexpr int max = 8*1024;
	int buf[max] = {0};				// better in some situations
	f.read(buf,max);

When feasible use a library function that is know not to overflow. For example:

	string s;	// s is default initialized to ""
	cin>>s;		// s expands to hold the string

Don't consider simple variables that are targets for input operations exceptions to this rule:

	int i;		// bad
	// ...
	cin>>i;

In the not uncommon case where the input target and the input operation get separated (as they should not) the possibility of used-before-set opens up.

	int i2 = 0;	// better
	// ...
	cin>>i;

A good optimizer should know about input operations and eliminate the redundant operation.

**Exception**: Sometimes, we want to initialize a set of variables with a call to a function that returns several values.
That can lead to uninitialized variables (exceptly as for input operations):

	error_code ec;
	Value v;
	tie(ec,v) = get_value();	// get_value() returns a pair<error_code,Value>

**Note**: Sometimes, a lambda can be used as an initializer to avoid an uninitialized variable.

	error_code ec;
	Value v = [&]() {
		auto p = get_value();	// get_value() returns a pair<error_code,Value>
		ec = p.first;
		return p.second;
	};
	
or maybe

	Value v = []() {
		auto p = get_value();	// get_value() returns a pair<error_code,Value>
		if (p.first) throw Bad_value{p.first};
		return p.second;
	};
	
**See also**: [ES.28](#Res-lambda-init)
	
**Enforcement**:

* Flag every uninitialized variable.
Don't flag variables of user-defined types with default constructors.
* Check that the unitialized buffer is read into *immediately* after declaration.


<a name="Res-introduce"></a>
### ES.21: Don't introduce a variable (or constant) before you need to use it

**Reason**: Readability. To limit the scope in which the variable can be used.

**Example**:

	int x = 7;
	// ... no use of x here ...
	++x;

**Enforcement**: Flag declaration that distant from their first use.


<a name="Res-init"></a>
### ES.22: Don't declare a variable until you have a value to initialize it with

**Reason**: Readability. Limit the scope in which a variable can be used. Don't risk used-before-set. Initialization is often more efficient than assignment.

**Example, bad**:

	string s;
	// ... no use of s here ...
	s = "what a waste";

**Example, bad**:

	SomeLargeType var;	// ugly CaMeLcAsEvArIaBlE
	
	if( cond )	// some non-trivial condition
		Set( &var );
	else if (cond2 || !cond3) {
		var = Set2( 3.14 );
	}
	else {
		var = 0;
		for (auto& e : something)
			var += e;
	}

	// use var; that this isn't done too early can be enforced statically with only control flow

This would be fine if there was a default initialization for `SomeLargeType` that wasn't too expensive.
Otherwise, a programmer might very well wonder if every possible path through the maze of conditions has been covered.
If not, we have a "use before set" bug. This is a maintenance trap.

For initializers of moderate complexity, including for `const` variables, consider using a lambda to express the initializer; see [ES.28](#Res-lambda-init).

**Enforcement**:

* Flag declarations with default initialization that are assigned to before they are first read.
* Flag any complicated computation after an uninitialized variable and before its use.


<a name="Res-list"></a>
### ES.23: Prefer the `{}` initializer syntax

**Reason**: The rules for `{}` initialization is simpler, more general, and safer than for other forms of initialization, and unambiguous.

**Example**:

       int x {f(99)};
       vector<int> v = {1,2,3,4,5,6};

**Exception**: For containers, there is a tradition for using `{...}` for a list of elements and `(...)` for sizes:

	vector<int> v1(10);		// vector of 10 elements with the default value 0
	vector<int> v2 {10};	// vector of 1 element with the value 10

**Note**: `{}`-initializers do not allow narrowing conversions.

**Example**:

	int x {7.9};	// error: narrowing
	int y = 7.9;	// OK: y becomes 7. Hope for a compiler warning

**Note**: `{}` initialization can be used for all initialization; other forms of initialization can't:

	auto p = new vector<int> {1,2,3,4,5};	// initialized vector
	D::D(int a, int b) :m{a,b} {	// member initializer (e.g., m might be a pair)
		// ...
	};
	X var {};				// initialize var to be empty
	struct S {
		int m {7};	// default initializer for a member
		// ...
	};

**Note**: Initialization of a variable declared `auto` with a single value `{v}` surprising results until recently:

	auto x1 {7};	// x1 is sn int with the value 7
	auto x2 = {7};	// x2 is and initializer_int<int> with an element 7

	auto x11 {7,8};		// error: two initializers
	auto x22 = {7,8};	// x2 is and initializer_int<int> with elements 7 and 8

**Exception**: Use `={...}` if you really want an `initializer_list<T>`

	auto fib10 = {0,1,2,3,5,8,13,25,38,63};	// fib10 is a list
	
**Example**:

	template<typename T>
	void f()
	{
		T x1(1);	// T initialized with 1
		T x0();		// bad: function declaration (often a mistake)
		
		T y1 {1};	// T initialized with 1
		T y0 {};	// default initialized T
		// ...
	}

**See also**: [Discussion](#???)

**Enforcement**: Tricky.

* Don't flag uses of `=` for simple initializers.
* Look for `=` after `auto` has been seen.


<a name="Res-unique"></a>
### ES.24: Use a `unique_ptr<T>` to hold pointers in code that may throw

**Reason**: Using `std::unique_ptr` is the simplest way to avoid leaks. And it is free compared to alternatives

**Example**:

	void use(bool leak)
	{
		auto p1 = make_unique<int>(7);	// OK
		int* p2 = new int{7};			// bad: might leak
		// ...
		if (leak) return;
		// ...
	}

If `leak==true` the object pointer to by `p2` is leaked and the object pointed to by `p1` is not.

**Enforcement**: Look for raw pointers that are targets of `new`, `malloc()`, or functions that may return such pointers.


<a name="Res-const"></a>
### ES.25: Declare an objects `const` or `constexpr` unless you want to modify its value later on

**Reason**: That way you can't change the value by mistake. That way may offer the compiler optimization opportunities.

**Example**:

	void f(int n)
	{
		const int bufmax = 2*n+2;	// good: we can't change bufmax by accident
		int xmax = n;				// suspicious: is xmax intended to change?
		// ...
	}

**Enforcement**: Look to see if a variable is actually mutated, and flag it if not. Unfortunately, it may be impossible to detect when a non-`const` was not intended to vary.


<a name="Res-recycle"></a>
### ES.26: Don't use a variable for two unrelated purposes

**Reason**: Readability.

**Example, bad**:

	void use()
	{
		int i;
		for (i=0; i<20; ++i) { /* ... */ }
		for (i=0; i<200; ++i) { /* ... */ }	// bad: i recycled
	}

**Enforcement**: Flag recycled variables.


<a name="Res-stack"></a>
### ES.27: Use `std::array` or `stack_array` for arrays on the stack

**Reason**: They are readable and don't impicitly convert to pointers.
They are not confused with non-standard extensions of built-in arrays.

**Example, bad**:

	const int n = 7;
	int m = 9;
	
	void f()
	{
		int a1[n];
		int a2[m];	// error: not ISO C++
		// ...
	}

**Note**: The definition of `a1` is legal C++ and has always been.
There is a lot of such code.
It is error-prone, though, especially when the bound is non-local.
Also, it is a "popular" source of errors (buffer overflow, pointers from array decay, etc.).
The definition of `a2` is C but not C++ and is considered a security risk

**Example**:

	const int n = 7;
	int m = 9;
	
	void f()
	{
		array<int,n> a1;
		stack_array<int> a2(m);
		// ...
	}
		
**Enforcement**:

* Flag arrays with non-constant bounds (C-style VLAs)
* Flag arrays with non-local constant bounds


<a name="Res-lambda-init"></a>
### ES.28: Use lambdas for complex initialization, especially of `const` variables

**Reason**: It nicely encapsulates local initialization, including cleaning up scratch variables needed only for the initialization, without needing to create a needless nonlocal yet nonreusable function. It also works for variables that should be `const` but only after some initialization work.

**Example; bad**:

	widget x;	// should be const, but:
	for(auto i=2; i <= N; ++i) {             // this could be some
	    x += some_obj.do_something_with(i);  // arbitrarily long code
	}                                        // needed to initialize x
	// from here, x should be const, but we can’t say so in code in this style

**Example; good**:

	const widget x = [&]{
	    widget val; 		// assume that widget has a default constructor
		for(auto i=2; i <= N; ++i) {             // this could be some
		    val += some_obj.do_something_with(i);// arbitrarily long code
		}                                        // needed to initialize x
		return val;
	}();

**Example**:

	string var = [&]{
		if (!in) return "";	// default
		string s;
		for (char c : in>>c)
			s += toupper(c);
		return s;
	}(); // note ()

If at all possible, reduce the conditions to a simple set of alternatives (e.g., an `enum`) and don't mix up selection and initialization.

**Example**:
	
	owner<istream&> in = [&]{
		switch (source) {
		case default: 		owned=false; return cin;
		case command_line:  owned=true;  return *new istringstream{argv[2]};
		case file: 			owned=true;  return *new ifstream{argv[2]};
	}();

**Enforcement:** Hard. At best a heuristic. Look for an unitialized variable followed by a loop assigning to it.


<a name="Res-macros"></a>
### ES.30: Don't use macros for program text manipulation

**Reason**: Macros are a major source of bugs.
Macros don't obey the usual scope and type rules.
Macros ensure that the human reader see something different from whet the compiler sees.
Macros complicates tool building.

**Example, bad**

	#define Case break; case	/* BAD */

This innocuous-looking macro makes a single lower case `c` instead of a `C` into a bad flow-control bug.
	
**Note**: This rule does not ban the use of macros for "configuration control" use in `#ifdef`s, etc.
	
**Enforcement**: Scream when you see a macro that isn't just use for source control (e.g., `#ifdef`)


<a name="Res-macros2"></a>
### ES.31: Don't use macros for constants or "functions"

**Reason**: Macros are a major source of bugs.
Macros don't obey the usual scope and type rules.
Macros don't obey the usual rules for argument passing.
Macros ensure that the human reader see something different from whet the compiler sees.
Macros complicates tool building.

**Example, bad**:

	#define PI 3.14
	#define SQUARE(a,b) (a*b)
	
Even if we hadn't left a well-know bug in `SQUARE` there are much better behaved alternatives; for example:

	constexpr double  pi = 3.14;
	template<typename T> T square(T a, T b) { return a*b; }

**Enforcement**: Scream when you see a macro that isn't just use for source control (e.g., `#ifdef`)


<a name="Res-CAPS!"></a>
### ES.32: Use `ALL_CAPS` for all macro names

**Reason**: Convention. Readability. Distinguishing macros.

**Example**:

	#define forever for(;;)		/* very BAD */
	
	#define FOREVER for(;;)		/* Still evil, but at least visible to humans */
	
**Enforcement**: Scream when you see a lower case macro.


<a name="Res-ellipses"></a>
### ES.40: Don't define a (C-style) variadic function

**Reason**: Not type safe. Requires messy cast-and-macro-laden code to get working right.

**Example**:

	??? <vararg>

**Alternative**: Overloading. Templates. Veriadic templates.

**Note**: There are rare used of variadic functions in SFINAE code, but those don't actually run and don't need the `<vararg>` implementation mess.

**Enforcement**: Flag definitions of C-style variadic functions.



## ES.stmt: Statements

Statements control the flow of control (except for function calls and exception throws, which are expressions).


<a name="Res-switch-if"></a>
### ES.70: Prefer a `switch`-statement to an `if`-statement when there is a choice

**Reason**:

* Readability.
* Efficiency: A `switch` compares against constants and is usually better optimized than a series of tests in an `if`-`then`-`else` chain.
* a `switch` is enables some heuristic consistency checking. For example, has all values of an `enum` been covered? If not, is there a `default`?

**Example**:

	void use(int n)
	{
		switch (n) {	// good
		case 0:	// ...
		case 7:	// ...
		}
	}

rather than

	void use2(int n)
	{
		if (n==0)	// bad: if-then-else chain comparing against a set of constants
			// ...
		else if (n==7)
			// ...
	}

**Enforcement**: Flag if-then-else chains that check against constants (only).


<a name="Res-for-range"></a>
### ES.71: Prefer a range-`for`-statement to a `for`-statement when there is a choice

**Reason**: Readability. Error prevention. Efficiency.

**Example**:

	for(int i=0; i<v.size(); ++i)	// bad
			cout << v[i] << '\n';

	for(auto p = v.begin(); p!=v.end(); ++p)	// bad
		cout << *p << '\n';

	for(auto& x : v) 	// OK
		cout << x << '\n';

	for(int i=1; i<v.size(); ++i) // touches two elements: can't be a range-for
		cout << v[i]+v[-1] << '\n';
	
	for(int i=1; i<v.size(); ++i) // possible side-effect: can't be a range-for
		cout << f(&v[i]) << '\n';

	for(int i=1; i<v.size(); ++i) { // body messes with loop variable: can't be a range-for
		if (i%2)
			++i;	// skip even elements
		else
			cout << v[i] << '\n';
	}

A human or a good static analyzer may determine that there really isn't a side effect on `v` in `f(&v[i])` so that the loop can be rewritten.

"Messing with the loop variable" in the body of a loop is typically best avoided.

**Note**: Don't use expensive copies of the loop variable of a range-`for` loop:

	for (string s : vs) // ...

This will copy each elements of `vs` into `s`. Better

	for (string& s : vs) // ...


**Enforcement**: Look at loops, if a traditional loop just looks at each element of a  sequence, and there are no side-effects on what it does with the elements, rewrite the loop to a for loop.


<a name="Res-for-while"></a>
### ES.72: Prefer a `for`-statement to a `while`-statement when there is an obvious loop variable

**Reason**: Readability: the complete logic of the loop is visible "up front". The scope of the loop variable can be limited.

**Example**:

	for (int i = 0; i < vec.size(); i++) {
	 // do work
	}

**Example, bad**:

	int i = 0;
	while (i < vec.size()) {
	 // do work
	 i++;
	}

**Enforcement**: ???


<a name="Res-while-for"></a>
### ES.73: Prefer a `while`-statement to a `for`-statement when there is no obvious loop variable

**Reason**: ???

**Example**:

	???

**Enforcement**: ???


<a name="Res-for-init"></a>
### ES.74: Prefer to declare a loop variable in the initializer part of as `for`-statement

**Reason**: Limit the loop variable visibility to the scope of the loop.
Avoid using the loop variable for other purposes after the loop.

**Example**:

	for (int i=0; i<100; ++i) {	// GOOD: i var is visible only inside the loop
		// ...
	}

**Example; don't**:

	int j;						// BAD: j is visible outside the loop
	for (j=0; j<100; ++j) {
		// ...
	}
	// j is still visible here and isn't needed

**See also**: [Don't use a variable for two unrelated purposes](#Res-recycle)

**Enforcement**: Warn when a variable modified inside the `for`-statement is declared outside the loop and not being used outside the loop.

**Discussion**: Scoping the loop variable to the loop body also helps code optimizers greatly. Recognizing that the induction variable
is only accessible in the loop body unblocks optimizations such as hoisting, strength reduction, loop-invariant code motion, etc.


<a name="Res-do"></a>
### ES.75: Avoid `do`-statements

**Reason**: Readability, avoidance of errors.
The termination conditions is at the end (where it can be overlooked) and the condition is not checked the first time through. ???

**Example**:

	int x;
	do {
		cin >> x;
		x
	} while (x<0);

**Enforcement**: ???

<a name="Res-goto"></a>
### ES.76: Avoid `goto`

**Reason**: Readability, avoidance of errors. There are better control structures for humans; `goto` is for machine generated code.

**Exception**: Breaking out of a nested loop. In that case, always jump forwards.

**Example**:

	???

**Example**: There is a fair amount of use of the C goto-exit idiom:

	void f()
	{
		// ...
			goto exit;
		// ...
			goto exit;
		// ...
	exit:
		... common cleanup code ...
	}

This is an ad-hoc simulation of destructors. Declare your resources with handles with destructors that clean up.

**Enforcement**:

* Flag `goto`. Better still flag all `goto`s that do not jump from a nested loop to the statement immediately after a nest of loops.



<a name="Res-continue"></a>
### ES.77: ??? `continue`

**Reason**: ???

**Example**:

	???

**Enforcement**: ???


<a name="Res-break"></a>
### ES.78: Always end a `case` with a `break`

**Reason**: ??? loop, switch ???

**Example**:

	???
	
**Note**: Multiple case labels of a single statement is OK:

	switch (x) {
	case 'a':
	case 'b':
	case 'f':
		do_something(x);
		break;
	}

**Enforcement**: ???


<a name="Res-default"></a>
### ES.79: ??? `default`

**Reason**: ???

**Example**:

	???

**Enforcement**: ???


<a name="Res-empty"></a>
### ES.85: Make empty statements visible

**Reason**: Readability.

**Example**:

	for (i=0; i<max; ++i);	// BAD: the empty statement is easily overlooked
		v[i] = f(v[i]);
		
	for (auto x : v) {		// better
		// nothing
	}

**Enforcement**: Flag empty statements that are not blocks and doesn't "contain" comments.


## ES.expr: Expresssions

Expressions manipulate values.

<a name="Res-complicated"></a>
### ES.40: Avoid complicated expressions

**Reason**: Complicated expressions are error-prone.

**Example**:

	while ((c=getc())!=-1)	// bad: assignment hidden in subexpression

	while ((cin>>c1, cin>>c2),c1==c2) // bad: two non-local variables assigned in a sub-expressions

	for (char c1,c2; cin>>c1>>c2 && c1==c2; )	// better, but possibly still too complicated

	int x = ++i + ++j;	// OK: iff i and j are not aliased

	v[i] = v[j]+v[k];	// OK: iff i!=j and i!=k

	x = a+(b=f())+(c=g())*7;	// bad: multiple assignments "hidden" in subexpressions

	x = a&b+c*d&&e^f==7;		// bad: relies on commonly misunderstood precedence rules

	x = x++ + x++ + ++x;		// bad: undefined behavior

Some of these expressions are unconditionally bad (e.g., they rely on undefined behavior). Others are simply so complicated and/or unusual that even good programmers could misunderstand them or overlook a problem when in a hurry.

**Note**: A programmer should know and use the basic rules for expressions.

**Example**:

	x=k*y+z;	// OK

	auto t1 = k*y;	// bad: unnecessarily verbose
	x = t1+z;

	if(0<=x && x<max) // OK

	auto t1 = 0<=x;		// bad: unnecessarily verbose
	aoto t2 = x<max;
	if(t1 && t2) // ...

	
**Enforcement**: Tricky. How complicated must an expression be to be considered complicated? Writing computations as statements with one operation each is also confusing. Things to consider:

* side effects: side effects on multiple non-local variables (for some definition of non-local) can be suspect, especially if the side effects are in separate subexpressions
* writes to aliased variables
* more than N operators (and what should N be?)
* reliance of subtle precedence rules
* uses undefined behavior (can we catch all undefined behavior?)
* implementation defined behavior?
* ???


<a name="Res-parens"></a>
### ES.41: If in doubt about operator precedence, parenthesize

**Reason**: Avoid errors. Readability. Not everyone has the operator table memorized.

**Example**:

	if (a && b==1)	// OK?
	if (a & b==1)	// OK?

Note: We recommend that programmers know their precedence table for the arithmetic operations, the logical operations,
but consider mixing bitwise logical operations with other operators in need of parentheses.

	if (a && b==1)	// OK: means a&&(b==1)
	if (a & b==1)	// bad: means (a&b)==1
	
**Note**: You should know enough not to need parentheses for

	if (a<0 || a<=max) {
		// ...
	}

**Enforcement**:

* Flag combinations of bitwise-logical operators and other operators.
* Flag assignment operators not as the leftmost operator.
* ???


<a name="Res-ptr"></a>
### ES.42: Keep use of pointers simple and straightforward

**Reason**: Complicated pointer manipulation is a major source of errors.

* Do all pointer arithmetic on an `array_view` (exception ++p in simple loop???)
* Avoid pointers to pointers
* ???

**Example**:

	???

**Enforcement**: We need a heuristic limiting the complexity of pointer arithmetic statement.


<a name="Res-order"></a>
### ES.43: Avoid expressions with undefined order of evaluation

**Reason**: You have no idea what such code does. Portability.
Even if it does something sensible for you, it may do something different on another compiler (e.g., the next release of your compiler) or with a different optimizer setting.

**Example**:

	v[i]=++i;	//  the result is undefined

A good rule of thumb is that you should not read a value twice in an expression where you write to it.

**Example**:

	???

**Note**: What is safe?

**Enforcement**: Can be detected by a good analyzer.


<a name="Res-order-fct"></a>
### ES.44: Don't depend on order of evaluation of function arguments

**Reason**: that order is unspecified

**Example**:

	int i=0;
	f(++i,++i);

The call will most likely be `f(0,1)` or `f(1,0)`, but you don't know which. Technically, the behavior is undefined.

**Example**: ??? oveloaded operators can lead to order of evaluation problems (shouldn't :-()

	f1()->m(f2());	// m(f1(),f2())
	cout << f1() << f2();	// operator<<(operator<<(cout,f1()),f2())


**Enforcement**: Can be detected by a good analyzer.


<a name="Res-magic"></a>
### ES.45: Avoid "`magic constants"; use symbolic constants

**Reason**: Unnamed constants embedded in expressions are easily overlooked and often hard to understand:

**Example**:

	for (int m = 1; m<=12; ++m)	// don't: magic constant 12
		cout << month[m] << '\n';

No, we don't all know that there a 12 month, numbered 1..12, in a year. Better:

	constexp int last_month = 12;	// months are numbered 1..12

	for (int m = first_month; m<=last_month; ++m)	// better
		cout << month[m] << '\n';

Better still, don't expose constants:

	for(auto m : month)
		cout << m <<'\n';

**Enforcement**: Flag literals in code. Give a pass to `0`, `1`, `nullptr`, `\n`, `""`, and others on a positive list.


<a name="Res-narrowing"></a>
### ES.46: Avoid lossy (narrowing, truncating) arithmetic conversions

**Reason**: A narrowing conversion destroys information, often unexpectedly so.

**Example**:

A key example is basic narrowing:

	double d = 7.9;
	int i = d;		// bad: narrowing: i becomes 7
    i = (int)d;     // bad: we're going to claim this is still not explicit enough
 
	void f(int x, long y, double d)
	{
		char c1 = x;	// bad: narrowing
		char c2 = y;	// bad: narrowing
		char c3 = d;	// bad: narrowing
	}

**Note**: The guideline support library offers a `narrow` operation for specifying that narrowing is acceptable and a `narrow` ("narrow if") that throws an exception if a narrowing would throw away information:

	i = narrow_cast<int>(d);		// OK (you asked for it): narrowing: i becomes 7
	i = narrow<int>(d);				// OK: throws narrowing_error

We also include lossy arithmetic casts, such as from a negative floating point type to an unsigned integral type:

    double d = -7.9;
    unsigned u = 0;
    
    u = d;                          // BAD
    u = narrow_cast<unsigned>(d);   // OK (you asked for it): u becomes 0
    u = narrow<unsigned>(d);        // OK: throws narrowing_error


**Enforcement**: A good analyzer can detect all narrowing conversions. However, flagging all narrowing conversions will lead to a lot of false positives. Suggestions:

* flag all floating-point to integer conversions (maybe only float->char and double->int. Here be dragons! we need data)
* flag all long->char (I suspect int->char is very common. Here be dragons! we need data)
* consider narrowing conversions for function arguments especially suspect


<a name="Res-nullptr"></a>
### ES.47: Use `nullptr` rather than `0` or `NULL`

**Reason**: Readability. Minimize surprises: `nullptr` cannot be confused with an `int`.

**Example**: Consider

	void f(int);
	void f(char*);
	f(0); 			// call f(int)
	f(nullptr); 	// call f(char*)

**Enforcement**: Flag uses of `0` and `NULL` for pointers. The transformation may be helped by simple program transformation.


<a name="Res-casts"></a>
### ES.48: Avoid casts

**Reason**: Casts are a well-known source of errors. Makes some optimizations unreliable.

**Example**:

	???

**Note**: Programmer who write casts typically assumes that they know what they are doing.
In fact, they often disable the general rules for using values.
Overload resolution and template instantiation usually pick the right function if there is a right function to pick.
If there is not, maybe there ought to be, rather than applying a local fix (cast).

**Note**: Casts are necessary in a systems programming language.
For example, how else would we get the address of a device register into a pointer.
However, casts are seriously overused as well as a major source of errors.

**Note**: If you feel the need for a lot of casts, there may be a fundamental design problem.

**Enforcement**:

* Force the elimination of C-style casts
* Warn against named casts
* Warn if there are many functional style casts (there is an obvious problem in quantifying 'many').


<a name="Res-casts-named"></a>
### ES.49: If you must use a cast, use a named cast

**Reason**: Readability. Error avoidance.
Named casts are more specific than a C-style or functional cast, allowing the compiler to catch some errors.

The named casts are:

* `static_cast`
* `const_cast`
* `reinterpret_cast`
* `dynamic_cast`
* `std::move`		// `move(x)` is an rvalue reference to `x`
* `std::forward`	// `forward(x)` is an rvalue reference to `x`
* `gsl::narrow_cast`	// `narrow_cast<T>(x)` is `static_cast<T>(x)`
* `gsl::narrow`			// `narrow<T>(x)` is `static_cast<T>(x)` if `static_cast<T>(x)==x` or it throws `narrowing_error`

**Example**:

	???

**Note**: ???

**Enforcement**: Flag C-style and functional casts.


<a name="Res-casts-const"></a>
## ES.50: Don't cast away `const`

**Reason**: It makes a lie out of `const`

**Note**: Usually the reason to "cast away `const`" is to allow the updating of some transient information of an otherwise immutable object.
Examples are cashing, mnemorization, and precomputation.
Such examples are often handled as well or better using `mutable` or an indirection than with a `const_cast`.

**Example**:

	???

**Enforcement**: Flag `const_cast`s.


<a name="Res-range-checking"></a>
### ES.55: Avoid the need for range checking

**Reason**: Constructs that cannot overflow, don't, and usually runs faster:

**Example**:

	for (auto& x : v)		// print all elements of v
		cout << x << '\n';
	
	auto p = find(v,x);		// find x in v

**Enforcement**: Look for explicit range checks and heuristically suggest alternatives.


<a name="Res-new"></a>
### ES.60: Avoid `new` and `delete[]` outside resource management functions

**Reason**: Direct resource management in application code is error-prone and tedious.

**Note**: also known as "No naked `new`!"

**Example, bad**:

	void f(int n)
	{
		auto p = new X[n];	// n default constructed Xs
		// ...
		delete[] p;
	}

There can be code in the `...` part that causes the `delete` never to happen.

**See also**: [R: Resource management](#S-resource).

**Enforcement**: Flag naked `new`s and naked `delete`s.


<a name="Res-del"></a>
### ES.61: delete arrays using `delete[]` and non-arrays using `delete`

**Reason**: That's what the language requires and mistakes can lead to resource release errors and/or memory corruption.

**Example, bad**:

	void f(int n)
	{
		auto p = new X[n];	// n default constructed Xs
		// ...
		delete p;			// error: just delete the object p, rather than delete the array p[]
	}
	
**Note**: This example not only violates the [no naked `new` rule](#Res-new) as in the previous example, it has many more problems.

**Enforcement**:

* if the `new` and the `delete` is in the same scope, mistakes can be flagged.
* if the `new` and the `delete` are in a constructor/destructor pair, mistakes can be flagged.


<a name="Res-arr2"></a>
### ES.62: Don't compare pointers into different arrays

**Reason**: The result of doing so is undefined.

**Example, bad**:

	void f(int n)
	{
		int a1[7];
		int a2[9];
		if (&a1[5]<&a2[7]) 		// bad: undefined
		if (0<&a1[5]-&a2[7])	// bad: undefined
	}
	
**Note**: This example has many more problems.

**Enforcement**:


<a name="SS-numbers"></a>
## Arithmetic

<a name="Res-mix"></a>
### ES.100: Don't mix signed and unsigned arithmetic

**Reason**: Avoid wrong results.

**Example**:

	???
	
**Note** Unfortunately, C++ uses signed integers for array subscripts and the standard library uses unsigned integers for container subscripts.
This precludes consistency.

**Enforcement**: Compilers already know and sometimes warn.


<a name="Res-unsigned"></a>
### ES.101: use unsigned types for bit manipulation

**Reason**: Unsigned types support bit manipulation without surprises from sign bits.

**Example**:

	???
	
**Exception**: Use unsigned types if you really want modulo arithmetic.

**Enforcement**: ???
	

<a name="Res-signed"></a>
### ES.102: Used signed types for arithmetic

**Reason**: Unsigned types support bit manipulation without surprises from sign bits.

**Example**:

	???
	
**Exception**: Use unsigned types if you really want modulo arithmetic.

**Enforcement**: ???


<a name="Res-overflow"></a>
### ES.103: Don't overflow

**Reason**: Overflow usually makes your numeric algorithm meaningless.
Incrementing a value beyond a maximum value can lead to memory corruption and undefined behavior.

**Example, bad**:

	int a[10];
	a[10] = 7;		// bad
	
	int n = 0;
	while (n++<10)
		a[n-1] = 9; // bad (twice)
	
**Example, bad**:

	int n = numeric_limits<int>::max();
	int m = n+1;	// bad
	
**Example, bad**:

	int area(int h, int w) { return h*w; }
	
	auto a = area(10'000'000*100'000'000);	// bad
	
**Exception**: Use unsigned types if you really want modulo arithmetic.

**Alternative**: For critical applications that can afford some overhead, use a range-checked integer and/or floating-point type.

**Enforcement**: ???


<a name="Res-overflow"></a>
### ES.104: Don't underflow

**Reason**: Decrementing a value beyond a maximum value can lead to memory corruption and undefined behavior.

**Example, bad**:

	int a[10];
	a[-2] = 7;		// bad
	
	int n = 101;
	while (n--)
		a[n-1] = 9; // bad (twice)
	
**Exception**: Use unsigned types if you really want modulo arithmetic.

**Enforcement**: ???


<a name="Res-zero"></a>
### ES.105: Don't divide by zero

**Reason**: The result is undefined and probably a crash.

**Note**: this also applies to `%`.

**Example**:

	???

**Alternative**: For critical applications that can afford some overhead, use a range-checked integer and/or floating-point type.

**Enforcement**: ???


<a name="S-performance"></a>
# PER: Performance

???should this section be in the main guide???

This section contains rules for people who needs high performance or low-latency.
That is, rules that relates to how to use as little time and as few resources as possible to achieve a task in a predictably short time.
The rules in this section are more restrictive and intrusive than what is needed for many (most) applications.
Do not blindly try to follow them in general code because achieving the goals of low latency requires extra work.

Performance rule summary:

* [PER.1: Don't optimize without reason](#Rper-reason)
* [PER.2: Don't optimize prematurely](#Rper-Knuth)
* [PER.3: Don't optimize something that's not performance critical](#Rper-critical)
* [PER.4: Don't assume that complicated code is necessarily faster than simple code](#Rper-simple)
* [PER.5: Don't assume that low-level code is necessarily faster than high-level code](#Rper-low)
* [PER.6: Don't make claims about performance without measurements](#Rper-measure)
* [PER.10: Rely on the static type system](#Rper-type)
* [PER.11: Move computation from run time to compile time](#Rper-Comp)
* [PER.12: Eliminate redundant aliases](#Rper-alias)
* [PER.13: Eliminate redundant indirections](#Rper-indirect)
* [PER.14: Minimize the number of allocations and deallocations](#Rper-alloc)
* [PER.15: Do not allocate on a critical branch](#Rper-alloc0)
* [PER.16: Use compact data structures](#Rper-compact)
* [PER.17: Declare the most used member of a time critical struct first](#Rper-struct)
* [PER.18: Space is time](#Rper-space)
* [PER.19: Access memory predictably](#Rper-access)
* [PER.30: Avoid context switches on the critical path](#Rper-context)


<a name="Rper-reason"></a>
### PER.1: Don't optimize without reason

**Reason**: If there is no need for optimization, the main result of the effort will be more errors and higher maintenance costs.

**Note**: Some people optimize out of habit or because it's fun.

???


<a name="Rper-Knuth"></a>
### PER.2: Don't optimize prematurely

**Reason**: Elaborately optimized code is usually larger and harder to change than unoptimized code.

???


<a name="Rper-critical"></a>
### PER.3: Don't optimize something that's not performance critical

**Reason**: Optimizing a non-performance-critical part of a program has no effect on system performance.

**Note**: If your program spends most of its time waiting for the web or for a human, optimization of in-memory computation is problably useless.

???



<a name="Rper-simple"></a>
### PER.4: Don't assume that complicated code is necessarily faster than simple code

**Reason**: Simple code can be very fast. Optimizers sometimes do marvels with simple code

**Note**: ???

???


<a name="Rper-low"></a>
### PER.5: Don't assume that low-level code is necessarily faster than high-level code

**Reason**: Low-level code sometimes inhibits optimizations. Optimizers sometimes do marvels with high-level code

**Note**: ???

???


<a name="Rper-measure"></a>
### PER.6: Don't make claims about performance without measurements

**Reason**: The field of performance is littered with myth and bogus folklore.
Modern hardware and optimizers defy naive assumptions; even experts are regularly surprised.

**Note**: Getting good performance measurements can be hard and require specialized tools.

**Note**: A few simple microbenchmarks using Unix `time` or the standard library `<chrono>` can help dispell the most obvious myths.
If you can't measure your complete system accurately, at least try to measure a few of your key operations and algorithms.
A profiler can help tell you which parts of your system are performance critical.
Often, you will be surprised.

???


<a name="Rper-type"></a>
### PER.10: Rely on the static type system

**Reason**: Type violations, weak types (e.g. `void*`s), and low level code (e.g., manipulation of sequences as individual bytes)
make the job of the optimizer much harder. Simple code often optimizes better than hand-crafted complex code.

???


<a name="Rper-Comp"></a>
### PER.11: Move computation from run time to compile time

???


<a name="Rper-alias"></a>
### PER.12: Eliminate redundant aliases

???


<a name="Rper-indirect"></a>
### PER.13: Eliminate redundant indirections

???


<a name="Rper-alloc"></a>
### PER.14: Minimize the number of allocations and deallocations

???


<a name="Rper-alloc0"></a>
### PER.15: Do not allocate on a critical branch

???


<a name="Rper-compact"></a>
### PER.16: Use compact data structures

**Reason**: Performance is typically dominated by memory access times.

???


<a name="Rper-struct"></a>
### PER.17: Declare the most used member of a time critical struct first

???


<a name="Rper-space"></a>
### PER.18: Space is time

**Reason**: Performance is typically dominated by memory access times.

???


<a name="Rper-access"></a>
### PER.19: Access memory predictably

**Reason**: Performance is very sensitive to cache performance and cache algorithms favor simple (usually linear) access to adjacent data.

???


<a name="Rper-context"></a>
### PER.30: Avoid context switches on the critical path

???


<a name="S-concurrency"></a>
# CP: Concurrency and Parallelism

???

Concurrency and parallism rule summary:

* [CP.1: Assume that your code will run as part of a multi-threaded program](#Rconc-multi)
* [CP.2: Avoid data races](#Rconc-races)

See also:

* [CP.con: Concurrency](#SScp-con)
* [CP.par: Parallelism](#SScp-par)
* [CP.simd: SIMD](#SScp-simd)
* [CP.free: Lock-free programming](#SScp-free)


<a name="Rconc-multi"></a>
### CP.1: Assume that your code will run as part of a multi-threaded program

**Reason**: It is hard to be certain that concurrency isn't used now or sometime in the future.
Code gets re-used.
Libraries using threads my be used from some other part of the program.

**Example**:

	???

**Exception**: There are examples where code will never be run in a multi-threaded environment.
However, here are also many examples where code that was "known" to never run in a multi-threaded program
was run as part of a multi-threaded program. Often years later.
Typically, such programs leads to a painful effort to remove data races.

<a name="Rconc-races"></a>
### CP.2: Avoid data races

**Reason**: Unless you do, nothing is guaranteed to work and subtle errors will persist.

**Note**: If you have any doubts about what this means, go read a book.

**Enforcement**: Some is possible, do at least something.


<a name="SScp-con"></a>
## CP.con: Concurrency

???

Concurrency rule summary:

* ???
* ???

???? should there be a "use X rather than std::async" where X is something that would use a better specified thread pool?
Â 
Speaking of concurrency, should there be a note about the dangers of std::atomic (weapons)?
A lot of people, myself included, like to experiment with std::memory_order, but it is perhaps best to keep a close watch on those things in production code.
Even vendors mess this up: Microsoft had to fix their `shared_ptr`
(weak refcount decrement wasn't synchronized-with the destructor, if I recall correctly, although it was only a problem on ARM, not Intel)
and everyone (gcc, clang, Microsoft, and intel) had to fix their `compare_exchange_*` this year,
after an implementation bug caused losses to some finance company and they were kind enough to let the community know.

It should definitely mention that `volatile` does not provide atomicity, does not synchronize between threads, and does not prevent instruction reordering (neither compiler nor hardware), and simply has nothing to do with concurrency.

	if(source->pool != YARROW_FAST_POOL && source->pool != YARROW_SLOW_POOL) {
		THROW( YARROW_BAD_SOURCE );
	}
	
??? Is `std::async` worth using in light of future (and even existing, as libraries) parallelism facilities? What should the guidelines recommend if someone wants to parallelize, e.g., `std::accumulate` (with the additional precondition of commutativity), or merge sort?

???UNIX signal handling???. May be worth reminding how little is async-signal-safe, and how to communicate with a signal handler (best is probably "not at all")


<a name="SScp-par"></a>
## CP.par: Parallelism

???

Parallelism rule summary:

* ???
* ???


<a name="SScp-simd"></a>
## CP.simd: SIMD

???

SIMD rule summary:

* ???
* ???

<a name="SScp-free"></a>
## CP.free: Lock-free programming

???

Lock-free programming rule summary:

* ???
* ???

### <a name="Rconc">Don't use lock-free programming unless you absolutely have to</a>

**Reason**: It's error-prone and requires expert level knowledge of language features, machine architecture, and data structures.

**Alternative**: Use lock-free data structures implemented by others as part of some library.

<a name="S-errors"></a>
# E: Error handling

Error handling involves:

* Detecting an error
* Transmitting information about an error to some handler code
* Preserve the state of a program in a valid state
* Avoid resource leaks

It is not possible to recover from all errors. If recovery from an error is not possible, it is important to quickly "get out" in a well-defined way. A strategy for error handling must be simple, or it becomes a source of even worse errors.

The rules are designed to help avoid several kinds of errors:

* Type violations (e.g., misuse of `union`s and casts)
* Resource leaks (including memory leaks)
* Bounds errors
* Lifetime errors (e.g., accessing an object after is has been `delete`d)
* Complexity errors (logical errors make likely by overly complex expression of ideas)
* Interface errors (e.g., an unexpected value is passed through an interface)

Error-handling rule summary:

* [E.1: Develop an error-handling strategy early in a design](#Re-design)
* [E.2: Throw an exception to signal that a function can't perform its assigned task](#Re-throw)
* [E.3: Use exceptions for error handling only](#Re-errors)
* [E.4: Design your error-handling strategy around invariants](#Re-design-invariants)
* [E.5: Let a constructor establish an invariant, and throw if it cannot](#Re-invariant)
* [E.6: Use RAII to prevent leaks](#Re-raii)
* [E.7: State your preconditions](#Re-precondition)
* [E.8: State your postconditions](#Re-postcondition)

* [E.12: Use `noexcept` when exiting a function because of a `throw` is impossible or unacceptable](#Re-noexcept)
* [E.13: Never throw while being the direct owner of an object](#Re-never-throw)
* [E.14: Use purpose-designed user-defined types as exceptions (not built-in types)](#Re-exception-types)
* [E.15: Catch exceptions from a hierarchy by reference](#Re-exception-ref)
* [E.16: Destructors, deallocation, and `swap` must never fail](#Re-never-fail)
* [E.17: Don't try to catch every exception in every function](#Re-not-always)
* [E.18: Minimize the use of explicit `try`/`catch`](#Re-catch)
* [E.19: Use a `Final_action` object to express cleanup if no suitable resource handle is available](#Re-finally)

* [E.25: ??? What to do in programs where exceptions cannot be thrown](#Re-no-throw)
* ???


<a name="Re-design"></a>
### E.1: Develop an error-handling strategy early in a design

**Reason**: a consistent and complete strategy for handling errors and resource leaks is hard to retrofit into a system.

<a name="Re-throw"></a>
### E.2: Throw an exception to signal that a function can't perform its assigned task

**Reason**: To make error handling systematic, robust, and non-repetitive.

**Example**:

	struct Foo {
		vector<Thing> v;
		File_handle f;
		string s;
	};

	void use()
	{
		Foo bar { {Thing{1}, Thing{2}, Thing{monkey}}, {"my_file","r"}, "Here we go!"};
		// ...
	}

Here, `vector` and `string`s constructors may not be able to allocate sufficient memory for their elements,
`vector`s constructor may not be able copy the `Thing`s in its initializer list, and `File_handle` may not be able to open the required file.
In each case, they throw an exception for `use()`'s caller to handle.
If `use()` could handle the failure to construct `bar` it can take control using `try`/`catch`.
In either case, `Foo`'s constructor correctly destroys constructed members before passing control to whatever tried to create a `Foo`.
Note that there is no return value that could contain an error code.

The `File_handle` constructor might defined like this

		File_handle::File_handle(const string& name, const string& mode)
			:f{fopen(name.c_str(),mode.c_str())}
		{
			if (!f)
				throw runtime_error{"File_handle: could not open "S-+ name + " as " + mode"}
		}

**Note**: It is often said that exceptions are meant to signal exceptional events and failures.
However, that's a bit circular because "what is exceptional?"
Examples:

* A precondition that cannot be met
* A constructor that cannot construct an object (failure to establish its class's [invariant](#Rc-struct))
* An out-of-range error (e.g., `v[v.size()]=7`)
* Inability to acquire a resource (e.g., the network is down)

In contrast, termination of an ordinary loop is not exceptional.
Unless the loop was meant to be infinite, termination is normal and expected.

**Note**: Don't use a `throw` as simply an alternative way of returning a value from a function.

**Exception**: Some systems, such as hard-real time systems require a guarantee that an action is taken in a (typically short) constant maximum time known before execution starts. Such systems can use exceptions only if there is tool support for accurately predicting the maximum time to recover from a `throw`.

**See also**: [RAII](#Re-raii)

**See also**: [discussion](#Sd-noexcept)


<a name="Re-errors"></a>
### E.3: Use exceptions for error handling only

**Reason**. To keep error handling separated from "ordinary code."
C++ implementations tend to be optimized based on the assumption that exceptions are rare.

**Example; don't**:

	int find_index(vector<string>& vec, const string& x)	// don't: exception not used for error handling
	{
		try {
			for (int i =0; i<vec.size(); ++i)
				if (vec[i]==x) throw i;	// found x
		} catch (int i) {
			return i;
		}
		return -1;	// not found
	}

This is more complicated and most likely runs much slower than the obvious alternative.
There is nothing exceptional about finding a value in a `vector`.


<a name="Re-design-invariants"></a>
### E.4: Design your error-handling strategy around invariants

**Reason**: To use an objects it must be in a valid state (defined formally or informally by an invariant)
and to recover from an error every object not destroyed must be in a valid state.

**Note**: An [invariant](#Rc-struct) is logical condition for the members of an object that a constructor must establish for the public member functions to assume.


<a name="Re-invariant"></a>
### E.5: Let a constructor establish an invariant, and throw if it cannot

**Reason**: Leaving an object without its invariant established is asking for trouble.
Not all member function can be called.

**Example**:

	???

**See also**: [If a constructor cannot construct a valid object, throw an exception](#Rc-throw)

**Enforcement**: ???


<a name="Re-raii"></a>
### E.6: Use RAII to prevent leaks

**Reason**: Leaks are typically unacceptable. RAII ("Resource Acquisition Is Initialization") is the simplest, most systematic way of preventing leaks.

**Example**:

	void f1(int i)	// Bad: possibly leak
	{
		int* p = new int[12];
		// ...
		if (i<17) throw Bad {"in f()",i};
		// ...
	}

We could carefully release the resource before the throw

	void f2(int i)	// Clumsy: explicit release
	{
		int* p = new int[12];
		// ...
		if (i<17) {
			delete p;
			throw Bad {"in f()",i};
		}
		// ...
	}

This is verbose. In larger code with multiple possible `throw`s explicit releases become repetitive and error-prone.

	void f3(int i)	// OK: resource management done by a handle
	{
		auto p = make_unique<int[12]>();
		// ...
		if (i<17) throw Bad {"in f()",i};
		// ...
	}

Note that this works even when the `throw` is implicit because it happened in a called function:

	void f4(int i)	// OK: resource management done by a handle
	{
		auto p = make_unique<int[12]>();
		// ...
		helper(i);	// may throw
		// ...
	}

Unless you really need pointer semantics, use a local resource object:

	void f5(int i)	// OK: resource management done by local object
	{
		vector<int> v(12);
		// ...
		helper(i);	// may throw
		// ...
	}

**Note**: If there is no obvious resource handle, cleanup actions can be represented by a [`Finally` object](#Re-finally)

**Note**: But what do we do if we are writing a program where exceptions cannot be used?
First challenge that assumption; there are many anti-exceptions myths around.
We know of only a few good reasons:

* We are on a system so small that the exception support would eat up most of our 2K or memory.
* We are in a hard-real-time system and we don't have tools that allows us that an exception is handled withon the required time.
* We are in a system with tons of legacy code using lots of pointers in difficult-to-understand ways
(in particular without a recognizable ownership strategy) so that exceptions could cause leaks.
* We get fired if we challenge our manager's ancient wisdom.

Only the first of these reasons is fundamental,
so whenever possible, use exception to implement RAII.
When exceptions cannot be used, simulate RAII.
That is, systematically check that objects are valid after construction and still release all resources in the destructor.
One strategy is to add a `valid()` operation to every resource handle:

	void f()
	{
		Vector<string> vs(100);	// not std::vector: valid() added
		if (!vs.valid()) {
			// handle error or exit
		}
		
		Ifstream fs("foo");		// not std::ifstream: valid() added
		if (!fs.valid()) {
			// handle error or exit
		}

		// ...
	} // destructors clean up as usual
	
Obviously, this increases the size of the code,
doesn't allow for implicit propagation of "exceptions" (`valid()` checks),
and `valid()` checks can be forgotten.
Prefer to use exceptions.

**See also**: [discussion](##Sd-noexcept).

**Enforcement**: ???


<a name="Re-precondition"></a>
### E.7: State your preconditions

**Reason**: To avoid interface errors.

**See also**: [precondition rule](#Ri-pre).


<a name="Re-postcondition"></a>
### E.8: State your postconditions

**Reason**: To avoid interface errors.

**See also**: [postcondition rule](#Ri-post).


<a name="Re-noexcept"></a>
### E.12: Use `noexcept` when exiting a function because of a `throw` is impossible or unacceptable

**Reason**: To make error handling systematic, robust, and efficient.

**Example**:

	double compute(double d) noexcept
	{
		return log(sqrt(d<=0? 1 : d));
	}

Here, I know that `compute` will not throw because it is composed out of operations that don't throw. By declaring `compute` to be `noexcept` I give the compiler and human readers information that can make it easier for them to understand and manipulate 'compute`.

**Note**: Many standard library functions are `noexcept` including all the standard library functions "inherited" from the C standard library.

**Example**:

	vector<double> munge(const vector<double>& v) noexcept
	{
		vector<double> v2(v.size());
		// ... do something ...
	}

The `noexcept` here states that I am not willing or able to handle the situation where I cannot construct the local `vector`. That is, I consider memory exhaustion a serious design error (on line with hardware failures) so that I'm willing to crash the program if it happens.

**See also**: [discussion](#Sd-noexcept).


<a name="Re-never-throw"></a>
### E.13: Never throw while being the direct owner of an object

**Reason**: That would be a leak.

**Example**:

	void leak(int x)	// don't: may leak
	{
		auto p = new int{7};
		if (x<0) throw Get_me_out_of_here{};	// may leak *p
		// ...
		delete p;	// we may never get here
	}

One way of avoiding such problems is to use resource handles consistently:

	void no_leak(int x)
	{
		auto p = make_unique<int>(7);
		if (x<0) throw Get_me_out_of_here{};	// will delete *p if necessary
		// ...
		// no need for delete p
	}

**See also**: ???resource rule ???


<a name="Re-exception-types"></a>
### E.14: Use purpose-designed user-defined types as exceptions (not built-in types)

**Reason**: A user-defined type is unlikely to clash with other people's exceptions.

**Example**:

	void my_code()
	{
		// ...
		throw Moonphase_error{};
		// ...
	}

	void your_code()
	{
		try {
			// ...
			my_code();
			// ...
		}
		catch(Bufferpool_exhausted) {
			// ...
		}
	}

**Example; don't**:

	void my_code()	// Don't
	{
		// ...
		throw 7;	// 7 means "moon in the 4th quarter"
		// ...
	}

	void your_code()	// Don't
	{
		try {
			// ...
			my_code();
			// ...
		}
		catch(int i) {	// i==7 means "input buffer too small"
			// ...
		}
	}

**Note**: The standard-library classes derived from `exception` should be used only as base classes or for exceptions that require only "generic" handling. Like built-in types, their use could class with other people's use of them.

**Example; don't**:

	void my_code()	// Don't
	{
		// ...
		throw runtime_error{"moon in the 4th quarter"};
		// ...
	}

	void your_code()	// Don't
	{
		try {
			// ...
			my_code();
			// ...
		}
		catch(runtime_error) {	// runtime_error means "input buffer too small"
			// ...
		}
	}


**See also**: [Discussion](#Sd-???)

**Enforcement**: Catch `throw` and `catch` of a built-in type. Maybe warn about `throw` and `catch` using an standard-library `exception` type. Obviously, exceptions derived from the `std::exception` hierarchy is fine.


<a name="Re-exception-ref"></a>
### E.15: Catch exceptions from a hierarchy by reference

**Reason**: To prevent slicing.

**Example**:

	void f()
	try {
		// ...
	}
	catch (exception e) {	// don't: may slice
		// ...
	}

Instead, use

	catch (exception& e) { /* ... */ }

**Enforcement**: Flag by-value exceptions if their type are part of a hierarchy (could require whole-program analysis to be perfect).


<a name="Re-never-fail"></a>
### E.16: Destructors, deallocation, and `swap` must never fail

**Reason**: We don't know how to write reliable programs if a destructor, a swap, or a memory deallocation fails; that is, if it exits by an exception or simply doesn't perform its required action.

**Example; don't**:

	class Connection {
		// ...
	public:
		~Connection()	// Don't: very bad destructor
		{
			if (cannot_disconnect()) throw I_give_up{information};
			// ...
		}
	};

**Note**: Many have tried to write reliable code violating this rule for examples such as a network connection that "refuses to close". To the best of our knowledge nobody has found a general way of doing this though occasionally, for very specific examples, you can get away with  setting some state for future cleanup. Every example, we have seen of this is error-prone, specialized, and usually buggy.

**Note**: The standard library assumes that destructors, deallocation functions (e.g., `operator delete`), and `swap` do not throw. If they do, basic standard library invariants are broken.

**Note**: Deallocation functions, including `operator delete`, must be `noexcept`. `swap` functions must be `noexcept`. Most destructors are implicitly `noexcept` by default. destructors, make them `noexcept`.

**Enforcement**: Catch destructors, deallocation operations, and `swap`s that `throw`. Catch such operations that are not `noexcept`.

**See also**: [discussion](#Sd-never-fail)


<a name="Re-not-always"></a>
### E.17: Don't try to catch every exception in every function

**Reason**: Catching an exception in a function that cannot take a meaningful recovery action leads to complexity and waste.
Let an exception propagate until it reaches a function that can handle it.
Let cleanup actions on the unwinding path be handled by [RAII](#Re-raii).

**Example; don't**:

	void f()	// bad
	{
		try {
			// ...
		}
		catch (...) {
			throw;	// propagate exception
		}
	}

**Enforcement**:

* Flag nested try-blocks.
* Flag source code files with a too high ratio of try-blocks to functions. (??? Problem: define "too high")


<a name="Re-catch"></a>
### E.18: Minimize the use of explicit `try`/`catch`

**Reason**: `try`/`catch` is verbose and non-trivial uses error-prone.

**Example**:

	???

**Enforcement**: ???


<a name="Re-finally"></a>
### E.19: Use a `Final_action` object to express cleanup if no suitable resource handle is available

**Reason**: `finally` is less verbose and harder to get wrong than `try`/`catch`.

**Example**:

	void f(int n)
	{
		void* p = malloc(1,n);
		auto __ = finally([] { free(p); });
		// ...
	}

**See also** ????
 

<a name="Re-no-throw"></a>
### E.25: ??? What to do in programs where exceptions cannot be thrown

**Note**: ??? mostly, you can afford exceptions and code gets simpler with exceptions ???


**See also**: [Discussion](#Sd-???).


<a name="S-const"></a>
# Con: Constants and Immutability

You can't have a race condition on a constant.
it is easier to reason about a program when many of the objects cannot change threir values.
Interfaces that promises "no change" of objects passed as arguments greatly increase readability.

Constant rule summary:

* [Con.1: By default, make objects immutable](#Rconst-immutable)
* [Con.2: By default, make member functions `const`](#Rconst-fct)
* [Con.3: By default, pass pointers and references to `const`s](#Rconst-ref)
* [Con.4: Use `const` to define objects with values that do not change after construction](#Rconst-const)
* [Con.5: Use `constexpr` for values that can be computed at compile time](#Rconst-constexpr)


<a name="Rconst-immutable"></a>
### Con.1: By default, make objects immutable

**Reason**: Immutable objects are easier to reason about, so make object non-`const` only when there is a need to change their value.

**Example**:

	for (
	container
	???

**Enforcement**: ???


<a name="Rconst-fct"></a>
### Con.2: By default, make member functions `const`

**Reason**: ???

**Example**:

	???

**Enforcement**: ???


<a name="Rconst-ref"></a>
### Con.3: By default, pass pointers and references to `const`s

**Reason**: ???

**Example**:

	???

**Enforcement**: ???


<a name="Rconst-const"></a>
### Con.4: Use `const` to define objects with values that do not change after construction

**Reason**: ???

**Example**:

	???

**Enforcement**: ???


<a name="Rconst-constexpr"></a>
### Con.5: Use `constexpr` for values that can be computed at compile time

**Reason**: ???

**Example**:

	???

**Enforcement**: ???


<a name="S-templates"></a>
# T: Templates and generic programming

Generic programming is programming using types and algorithms parameterized by types, values, and algorithms.
In C++, generic programming is supported by the `template` language mechanisms.

Arguments to generic functions are characterized by sets of requirements on the argument types and values involved.
In C++, these requirements are expressed by compile-time predicates called concepts.

Templates can also be used for meta-programming; that is, programs that compose code at compile time.

Template use rule summary:

* [T.1: Use templates to raise the level of abstraction of code](#Rt-raise)
* [T.2: Use templates to express algorithms that apply to many argument types](#Rt-algo)
* [T.3: Use templates to express containers and ranges](#Rt-cont)
* [T.4: Use templates to express syntax tree manipulation](#Rt-expr)
* [T.5: Combine generic and OO techniques to amplify their strengths, not their costs](#Rt-generic-oo)

Concept use rule summary:

* [T.10: Specify concepts for all template arguments](#Rt-concepts)
* [T.11: Whenever possible use standard concepts](#Rt-std)
* [T.12: Prefer concept names over `auto` for local variables](#Rt-auto)
* [T.13: Prefer the shorthand notation for simple, single-type argument concepts](#Rt-shorthand)
* ???

Concept definition rule summary:

* [T.20: Avoid "concepts" without meaningful semantics](#Rt-low)
* [T.21: Define concepts to define complete sets of operations](#Rt-complete)
* [T.22: Specify axioms for concepts](#Rt-axiom)
* [T.23: Differentiate a refined concept from its more general case by adding new use patterns](#Rt-refine)
* [T.24: Use tag classes or traits to differentiate concepts that differ only in semantics](#Rt-tag)
* [T.25: Avoid negating constraints](#Rt-not)
* [T.26: Prefer to define concepts in terms of use-patterns rather than simple syntax](#Rt-use)
* ???

Template interface rule summary:

* [T.40: Use function objects to pass operations to algorithms](#Rt-fo)
* [T.41: Require complete sets of operations for a concept](#Rt-operations)
* [T.42: Use template aliases to simplify notation and hide implementation details](#Rt-alias)
* [T.43: Prefer `using` over `typedef` for defining aliases](#Rt-using)
* [T.44: Use function templates to deduce class template argument types (where feasible)](#Rt-deduce)
* [T.46: Require template arguments to be at least `Regular` or `SemiRegular`](#Rt-regular)
* [T.47: Avoid highly visible unconstrained templates with common names](#Rt-visible)
* [T.48: If your compiler does not support concepts, fake them with `enable_if`](#Rt-concept-def)
* [T.49: Where possible, avoid type-erasure](#Rt-erasure)
* [T.50: Avoid writing an unconstrained template in the same namespace as a type](#Rt-unconstrained-adl)

Template definition rule summary:

* [T.60: Minimize a template's context dependencies](#Rt-depend)
* [T.61: Do not over-parameterize members (SCARY)](#Rt-scary)
* [T.62: Place non-dependent template members in a non-templated base class](#Rt-nondependent)
* [T.64: Use specialization to provide alternative implementations of class templates](#Rt-specialization)
* [T.65: Use tag dispatch to provide alternative implementations of functions](#Rt-tag-dispatch)
* [T.66: Use selection using `enable_if` to optionally define a function](#Rt-enable_if)
* [T.67: Use specialization to provide alternative implementations for irregular types](#Rt-specialization2)
* [T.68: Use `{}` rather than `()` within templates to avoid ambiguities](#Rt-cast)
* [T.69: Inside a template, don't make an unqualified nonmember function call unless you intend it to be a customization point](#Rt-customization)

Template and hierarchy rule summary:

* [T.80: Do not naively templatize a class hierarchy](#Rt-hier)
* [T.81: Do not mix hierarchies and arrays](#Rt-array) // ??? somewhere in "hierarchies"
* [T.82: Linearize a hierarchy when virtual functions are undesirable](#Rt-linear)
* [T.83: Do not declare a member function template virtual](#Rt-virtual)
* [T.84: Use a non-template core implementation to provide an ABI-stable interface](#Rt-abi)
* [T.??: ????](#Rt-???)

Variadic template rule summary:

* [T.100: Use variadic templates when you need a function that takes a variable number of arguments of a variety of types](#Rt-variadic)
* [T.101: ??? How to pass arguments to a variadic template ???](#Rt-variadic-pass)
* [T.102: ??? How to process arguments to a variadic template ???](#Rt-variadic-process)
* [T.103: Don't use variadic templates for homogeneous argument lists](#Rt-variadic-not)
* [T.??: ????](#Rt-???)

Metaprogramming rule summary:

* [T.120: Use template metaprogramming only when you really need to](#Rt-metameta)
* [T.121: Use template metaprogramming primarily to emulate concepts](#Rt-emulate)
* [T.122: Use templates (usually template aliases) to compute types at compile time](#Rt-tmp)
* [T.123: Use `constexpr` functions to compute values at compile time](#Rt-fct)
* [T.124: Prefer to use standard-library TMP facilities](#Rt-std)
* [T.125: If you need to go beyond the standard-library TMP facilities, use an existing library](#Rt-lib)
* [T.??: ????](#Rt-???)

Other template rules summary:

* [T.140: Name all nontrivial operations](#Rt-name)
* [T.141: Use an unnamed lambda if you need a simple function object in one place only](#Rt-lambda)
* [T.142: Use template variables to simplify notation](#Rt-var)
* [T.143: Don't write unintentionally nongeneric code](#Rt-nongeneric)
* [T.144: Don't specialize function templates](#Rt-specialize-function)
* [T.??: ????](#Rt-???)


<a name="SS-GP"></a>
## T.gp: Generic programming

Generic programming is programming using types and algorithms parameterized by types, values, and algorithms.


<a name="Rt-raise"></a>
### T.1: Use templates to raise the level of abstraction of code

**Reason**: Generality. Re-use. Efficiency. Encourages consistent definition of user types.

**Example, bad**: Conceptually, the following requirements are wrong because what we want of `T` is more than just the very low-level concepts of "can be incremented" or "can be added":

	template<typename T, typename A>
		// requires Incrementable<T>
	A sum1(vector<T>& v, A s)
	{
		for (auto x : v) s+=x;
		return s;
	}
		
	template<typename T, typename A>
		// requires Simple_number<T>
	A sum2(vector<T>& v, A s)
	{
		for (auto x : v) s = s+x;
		return s;
	}

Assuming that `Incrementable` does not support `+` and `Simple_number` does not support `+=`, we have overconstrained implementers of `sum1` and `sum2`.
And, in this case, missed an opportunity for a generalization.

**Example**:

	template<typename T, typename A>
		// requires Arithmetic<T>
	A sum(vector<T>& v, A s)
	{
		for (auto x : v) s+=x;
		return s;
	}

Assuming that `Arithmetic` requires both `+` and `+=`, we have constrained the user of `sum` to provide a complete arithmetic type.
That is not a minimal requirement, but it gives the implementer of algorithms much needed freedom and ensures that any `Arithmetic` type
can be user for a wide variety of algorithms.

For additional generality and reusability, we could also use a more general `Container` or `Range` concept instead of committing to only one container, `vector`.

**Note**: If we define a template to require exactly the operations required for a single implementation of a single algorithm
(e.g., requiring just `+=` rather than also `=` and `+`) and only those,
we have overconstrained maintainers.
We aim to minimize requirements on template arguments, but the absolutely minimal requirements of an implementation is rarely a meaningful concept.

**Note**: Templates can be used to express essentially everything (they are Turing complete), but the aim of generic programming (as expressed using templates)
is to efficiently generalize operations/algorithms over a set of types with similar semantic properties.

**Enforcement**:

* Flag algorithms with "overly simple" requirements, such as direct use of specific operators without a concept.
* Do not flag the definition of the "overly simple" concepts themselves; they may simply be building blocks for more useful concepts.


<a name="Rt-algo"></a>
### T.2: Use templates to express algorithms that apply to many argument types

**Reason**: Generality. Minimizing the amount of source code. Interoperability. Re-use.

**Example**: That's the foundation of the STL. A single `find` algorithm easily works with any kind of input range:

	template<typename Iter, typename Val>
		// requires Input_iterator<Iter>
		//       && Equality_comparable<Value_type<Iter>,Val>
	Iter find(Iter b, Iter e, Val v)
	{
		// ...
	}

**Note**: Don't use a template unless you have a realistic need for more than one template argument type.
Don't overabstract.

**Enforcement**: ??? tough, probably needs a human


<a name="Rt-cont"></a>
### T.3: Use templates to express containers and ranges

**Reason**: Containers need an element type, and expressing that as a template argument is general, reusable, and type safe.
It also avoids brittle or inefficient workarounds. Convention: That's the way the STL does it.

**Example**:

	template<typename T>
		// requires Regular<T>
	class Vector {
		// ...
		T* elem;	// points to sz Ts
		int sz;
	};
	
	Vector<double> v(10);
	v[7] = 9.9;

**Example, bad**:

	class Container {
		// ...
		void* elem;	// points to size elements of some type
		int sz;
	};
	
	Container c(10,sizeof(double));
	((double*)c.elem)[] = 9.9;
	
This doesn't directly express the intent of the programmer and hides the structure of the program from the type system and optimizer.

Hiding the `void*` behind macros simply obscures the problems and introduces new opportunities for confusion.

**Exceptions**: If you need an ABI-stable interface,
you might have to provide a base implementation and express the (type-safe) template in terms of that.
See [Stable base](#Rt-abi).

**Enforcement**:

* Flag uses of `void*`s and casts outside low-level implementation code


<a name="Rt-expr"></a>
### T.4: Use templates to express syntax tree manipulation

**Reason**: ???

**Example**:

	???

**Exceptions**: ???



<a name="Rt-generic-oo"></a>
### T.5: Combine generic and OO techniques to amplify their strengths, not their costs

**Reason**: Generic and OO techniques are complementary.

**Example**: Static helps dynamic: Use static polymorphism to implement dynamically polymorphic interfaces.

	class Command {
		// pure virtual functions
	};
	
	// implementations
	template</*...*/>
	class ConcreteCommand : public Command {
		// implement virtuals
	};

**Example**: Dynamic helps static: Offer a generic, comfortable, statically bound interface, but internally dispatch dynamically, so you offer a uniform object layout. Examples include type erasure as with `std::shared_ptr`’s deleter. (But [don't overuse type erasure](#Rt-erasure).)

**Note**: In a class template, nonvirtual functions are only instantiated if they're used -- but virtual functions are instantiated every time. This can bloat code size, and may overconstrain a generic type by instantiating functionality that is never needed. Avoid this, even though the standard facets made this mistake.

**Enforcement**:
* Flag a class template that declares new (non-inherited) virtual functions.



<a name="SS-concepts"></a>
## TPG.concepts: Concept rules

Concepts is a facility for specifying requirements for template arguments.
It is an [ISO technical specification](#Ref-conceptsTS), but not yet supported by currently shipping compilers.
Concepts are, however, crucial in the thinking about generic programming and the basis of much work on future C++ libraries
(standard and other).

Concept use rule summary:

* [T.10: Specify concepts for all template arguments](#Rt-concepts)
* [T.11: Whenever possible use standard concepts](#Rt-std)
* [T.14: Prefer concept names over `auto`](#Rt-auto)
* [T.15: Prefer the shorthand notation for simple, single-type argument concepts](#Rt-shorthand)
* ???

Concept definition rule summary:

* [T.20: Avoid "concepts" without meaningful semantics](#Rt-low)
* [T.21: Define concepts to define complete sets of operations](#Rt-complete)
* [T.22: Specify axioms for concepts](#Rt-axiom)
* [T.23: Differentiate a refined concept from its more general case by adding new use patterns](#Rt-refine)
* [T.24: Use tag classes or traits to differentiate concepts that differ only in semantics](#Rt-tag)
* [T.25: Avoid negating constraints](#Rt-not)
* [T.26: Prefer to define concepts in terms of use-patterns rather than simple syntax](#Rt-use)
* ???


<a name="SS-concept-use"></a>
## T.con-use: Concept use

<a name="Rt-concepts"></a>
### T.10: Specify concepts for all template arguments

**Reason**: Correctness and readability.
The assumed meaning (syntax and semantics) of a template argument is fundamental to the interface of a template.
A concept dramatically improves documentation and error handling for the template.
Specifying concepts for template arguments is a powerful design tool.

**Example**:

	template<typename Iter, typename Val>
		requires Input_iterator<Iter>
		         && Equality_comparable<Value_type<Iter>,Val>
	Iter find(Iter b, Iter e, Val v)
	{
		// ...
	}

or equivalently and more succinctly

	template<Input_iterator Iter, typename Val>
		requires Equality_comparable<Value_type<Iter>,Val>
	Iter find(Iter b, Iter e, Val v)
	{
		// ...
	}

**Note**: Until your compilers support the concepts language feature, leave the concepts in comments:

	template<typename Iter, typename Val>
		// requires Input_iterator<Iter>
		//       && Equality_comparable<Value_type<Iter>,Val>
	Iter find(Iter b, Iter e, Val v)
	{
		// ...
	}
	
**Note**: Plain `typename` (or `auto`) is the least constraining concept.
It should be used only rarely when nothing more than "it's a type" can be assumed.
This is typically only needed when (as part of template metaprogramming code) we manipulate pure expression trees, postponing type checking.

**References**: TC++PL4, Palo Alto TR, Sutton

**Enforcement**: Flag template type arguments without concepts


<a name="Rt-std"></a>
### T.11: Whenever possible use standard concepts

**Reason**: "Standard" concepts (as provided by the GSL, the ISO concepts TS, and hopefully soon the ISO standard itself)
saves us the work of thinking up our own concepts, are better thought out than we can manage to do in a hurry, and improves interoperability.

**Note**: Unless you are creating a new generic library, most of the concepts you need will already be defined by the standard library.

**Example**:

	concept<typename T>
	Ordered_container = Sequence<T> && Random_access<Iterator<T>> && Ordered<Value_type<T>>;	// don't define this: Sortable is in the GSL

	void sort(Ordered_container& s);

This `Ordered_container` is quite plausible, but it is very similar to the `Sortable` concept in the GSL (and the Range TS).
Is it better? Is it right? Does it accurately reflect the standard's requirements for `sort`?
It is better and simpler just to use `Sortable`:

	void sort(Sortable& s);		// better

**Note**: The set of "standard" concepts is evolving as we approaches real (ISO) standardization.

**Note**: Designing a useful concept is challenging.

**Enforcement**: Hard.

* Look for unconstrained arguments, templates that use "unusual"/non-standard concepts, templates that use "homebrew" concepts without axioms.
* Develop a concept-discovery tool (e.g., see [an early experiment](http://www.stroustrup.com/sle2010_webversion.pdf).

	
<a name="Rt-auto"></a>
### T.12: Prefer concept names over `auto` for local variables

**Reason**: `auto` is the weakest concept. Concept names convey more meaning than just `auto`.

**Example**:

	vector<string> v;
	auto& x = v.front();	// bad
	String& s = v.begin();	// good

**Enforcement**:

* ???


<a name="Rt-shorthand"></a>
### T.13: Prefer the shorthand notation for simple, single-type argument concepts

**Reason**: Readability. Direct expression of an idea.

**Example**: To say "`T` is `Sortable`":

	template<typename T>		// Correct but verbose: "The parameter is
		requires Sortable<T>	// of type T which is the name of a type
	void sort(T&);				// that is Sortable"

	template<Sortable T>		// Better: "The parameter is of type T
	void sort(T&);				// which is Sortable"

	void sort(Sortable&);		// Best: "The parameter is Sortable"

The shorter versions better match the way we speak. Note that many templates don't need to use the `template` keyword.

**Enforcement**:

* Not feasible in the short term when people convert from the `<typename T>` and `<class T`> notation.
* Later, flag declarations that first introduces a typename and then constrains it with a simple, single-type-argument concept.


<a name="SS=concept-def"></a>
## T.con-def: Concept definition rules

???


<a name="Rt-low"></a>
### T.20: Avoid "concepts" without meaningful semantics

**Reason**: Concepts are meant to express semantic notions, such as "a number", "a range" of elements, and "totally ordered."
Simple constraints, such as "has a `+` operator" and "has a `>` operator" cannot be meaningfully specified in isolation
and should be used only as building blocks for meaningful concepts, rather than in user code.

**Example, bad**:

	template<typename T>
	concept Addable = has_plus<T>;    // bad; insufficient
			
	template<Addable N> auto algo(const N& a, const N& b) // use two numbers
	{
		// ...
		return a+b;
	}
	
	int x = 7;
	int y = 9;
	auto z = plus(x,y);	// z = 18
	
	string xx = "7";
	string yy = "9";
	auto zz = plus(xx,yy);	// zz = "79"

Maybe the concatenation was expected. More likely, it was an accident. Defining minus equivalently would give dramatically different sets of accepted types.
This `Addable` violates the mathematical rule that addition is supposed to be commutative: `a+b == b+a`,

**Note**: The ability to specify a meaningful semantics is a defining characteristic of a true concept, as opposed to a syntactic constraint.

**Example (using TS concepts)**:

	template<typename T>
	// The operators +, -, *, and / for a number are assumed to follow the usual mathematical rules
	concept Number = has_plus<T>
				  && has_minus<T>
				  && has_multiply<T>
				  && has_divide<T>;

	template<Number N> auto algo(const N& a, const N& b) // use two numbers
	{
		// ...
		return a+b;
	}

	int x = 7;
	int y = 9;
	auto z = plus(x,y);	// z = 18
	
	string xx = "7";
	string yy = "9";
	auto zz = plus(xx,yy);	// error: string is not a Number
	
**Note**: Concepts with multiple operations have far lower chance of accidentally matching a type than a single-operation concept.
	
**Enforcement**:

* Flag single-operation `concepts` when used outside the definition of other `concepts`.
* Flag uses of `enable_if` that appears to simulate single-operation `concepts`.


<a name="Rt-complete"></a>
### T.21: Define concepts to define complete sets of operations

**Reason**: Improves interoperability. Helps implementers and maintainers.

**Example, bad**:

	template<typename T> Subtractable = requires(T a, T,b) { a-b; }	// correct syntax?

This makes no semantic sense. You need at least `+` to make `-` meaningful and useful.

Examples of complete sets are

* `Arithmetic`: `+`, `-`, `*`, `/`, `+=`, `-=`, `*=`, `/=`
* `Comparable`: `<`, `>`, `<=`, `>=`, `==`, `!=`

**Enforcement**: ???


<a name="Rt-axiom"></a>
### T.22: Specify axioms for concepts

**Reason**: A meaningful/useful concept has a semantic meaning.
Expressing this semantics in a informal, semi-formal, or informal way makes the concept comprehensible to readers and the effort to express it can catch conceptual errors.
Specifying semantics is a powerful design tool.

**Example**:

	template<typename T>
		// The operators +, -, *, and / for a number are assumed to follow the usual mathematical rules
		// axiom(T a, T b) { a+b == b+a; a-a == 0; a*(b+c)==a*b+a*c; /*...*/ }
		concept Number = requires(T a, T b) {
			{a+b} -> T;	// the result of a+b is convertible to T
			{a-b} -> T;
			{a*b} -> T;
			{a/b} -> T;
		};
	
**Note** This is an axiom in the mathematical sense: something that may be assumed without proof.
In general, axioms are not provable, and when they are the proof is often beyond the capability of a compiler.
An axiom may not be general, but the template writer may assume that it holds for all inputs actually used (similar to a precondition).

**Note**: In this context axioms are Boolean expressions.
See the [Palo Alto TR](#S-references) for examples.
Currently, C++ does not support axioms (even the ISO Concepts TS), so we have to make do with comments for a longish while.
Once language support is available, the `//` in front of the axiom can be removed

**Note**: The GSL concepts have well defined semantics; see the Palo Alto TR and the Ranges TS.

**Exception**: Early versions of a new "concept" still under development will often just define simple sets of contraints without a well-specified semantics.
Finding good semantics can take effort and time.
An incomplete set of constraints can still be very useful:

	??? binary tree: rotate(), ...
	
A "concept" that is incomplete or without a well-specified semantics can still be useful.
However, it should not be assumed to be stable. Each new use case may require such an incomplete concepts to be improved.

**Enforcement**:

* Look for the word "axiom" in concept definition comments


<a name="Rt-refine"></a>
### T.23: Differentiate a refined concept from its more general case by adding new use patterns.

**Reason**: Otherwise they cannot be distinguished automatically by the compiler.

**Example**:

	template<typename I>
	concept bool Input_iterator = requires (I iter) { ++iter; };

	template<typename I>
	concept bool Fwd_iter = Input_iter<I> && requires (I iter) { iter++; }

The compiler can determine refinement based on the sets of required operations.
If two concepts have exactly the same requirements, they are logically equivalent (there is no refinement).

This also decreases the burden on implementers of these types since
they do not need any special declarations to "hook into the concept".

**Enforcement**:
* Flag a concept that has exactly the same requirements as another already-seen concept (neither is more refined). To disambiguate them, see [T.24](#Rt-tag).


<a name="Rt-tag"></a>
### T.24: Use tag classes or traits to differentiate concepts that differ only in semantics.

**Reason**: Two concepts requiring the same syntax but having different semantics leads to ambiguity unless the programmer differentiates them.

**Example**:

	template<typename I>		// iterator providing random access
	concept bool RA_iter = ...;

	template<typename I>		// iterator providing random access to contiguous data
	concept bool Contiguous_iter =
  		RA_iter<I> && is_contiguous<I>::value;  // ??? why not is_contiguous<I>() or is_contiguous_v<I>?

The programmer (in a library) must define `is_contiguous` (a trait) appropriately.

**Note**: Traits can be trains classes or type traits.
These can be user-defined or standard-libray ones.
Prefer the standard-libray ones.

**Enforcement**:

* The compiler flags ambiguous use of identical concepts.
* Flag the definition of identical concepts.


<a name="Rt-not"></a>
### T.25: Avoid negating constraints.

**Reason**: Clarity. Maintainability.
Functions with complementary requirements expressed using negation are brittle.

**Example**: Initially, people will try to define functions with complementary requirements:

	template<typename T>
  		requires !C<T> 		// bad
	void f();

	template<typename T>
  		requires C<T>
	void f();

This is better:

	template<typename T>	// general template
	void f();

	template<typename T>	// specialization by concept
  		requires C<T>
	void f();

The compiler will choose the unconstrained template only when `C<T>` is
unsatisfied. If you do not want to (or cannot) define an unconstrained
version of `f()`, then delete it.

	template<typename T>
	void f() = delete;

The compiler will select the overload and emit an appropriate error.

**Enforcement**:
* Flag pairs of functions with `C<T>` and `!C<T>` constraints
* Flag all constraint negation


<a name="Rt-use"></a>
### T.27: Prefer to define concepts in terms of use-patterns rather than simple syntax

**Reason**: The definition is more readable and corresponds directly to what a user has to write.
Conversions are taken into account. You don't have to remember the names of all the type traits.

**Example**:

	???

**Enforcement**: ???


<a name="SS-temp-interface"></a>
## Template interfaces

???

<a name="Rt-fo"></a>
### T.40: Use function objects to pass operations to algorithms

**Reason**: Function objects can carry more information through an interface than a "plain" pointer to function.
In general, passing function objects give better performance than passing pointers to functions.

**Example**:

	bool greater(double x, double y) { return x>y; }
	sort(v,greater);								// pointer to function: potentially slow
	sort(v,[](double x, double y) { return x>y; });	// function object
	sort(v,greater<>);								// function object
	
	bool greater_than_7(double x) { return x>7; }
	auto x = find(v,greater_than_7);				// pointer to function: inflexible
	auto y = find(v,[](double x) { return x>7; });	// function object: carries the needed data
	auto y = find(v,Greater_than<double>(7));		// function object: carries the needed data
	
	??? these lambdas are crying out for auto parameters -- any objection to making the change?
	
**Note**: Lambdas generate function objects.

**Note**: The performance argument depends on compiler and optimizer technology.

**Enforcement**:

* Flag pointer to function template arguments.
* Flag pointers to functions passed as arguments to a template (risk of false positives).


<a name="Rt-operations"></a>
### T.41: Require complete sets of operations for a concept

**Reason**: Ease of comprehension.
Improved interoperability.
Flexibility for template implementers.

**Note**: The issue here is whether to require the minimal set of operations for a template argument
(e.g., `==` but not `!=` or `+` but not `+=`).
The rule supports the view that a concept should reflect a (mathematically) coherent set of operations.

**Example**:

	???

**Enforcement**: ???


<a name="Rt-alias"></a>
### T.42: Use template aliases to simplify notation and hide implementation details

**Reason**: Improved readability. Implementation hiding. Note that template aliases replace many uses of traits to compute a type. They can also be used to wrap a trait.

**Example**:

	template<typename T, size_t N>
	class matrix {
		// ...
		using Iterator = typename std::vector<T>::iterator;
		// ...
	};

This saves the user of `Matrix` from having to know that its elements are stored in a `vector` and also saves the user from repeatedly typing `typename std::vector<T>::`.

**Example**:

	template<typename T>
	using Value_type<T> = container_traits<T>::value_type;

This saves the user of `Value_type` from having to know the technique used to implement `value_type`s.

**Enforcement**:

* Flag use of `typename` as a disambiguator outside `using` declarations.
* ???


<a name="Rt-using"></a>
### T.43: Prefer `using` over `typedef` for defining aliases

**Reason**: Improved readability: With `using`, the new name comes first rather than being embedded somewhere in a declaration.
Generality: `using` can be used for template aliases, whereas `typedef`s can't easily be templates.
Uniformity: `using` is syntactically similar to `auto`.

**Example**:

	typedef int (*PFI)(int);	// OK, but convoluted
	
	using PFI2 = int (*)(int);	// OK, preferred
	
	template<typename T>
	typedef int (*PFT)(T);		// error
		
	template<typename T>
	using PFT2 = int (*)(T);	// OK

**Enforcement**:

* Flag uses of `typedef`. This will give a lot of "hits" :-(


<a name="Rt-deduce"></a>
### T.44: Use function templates to deduce class template argument types (where feasible)

**Reason**: Writing the template argument types explicitly can be tedious and unnecessarily verbose.

**Example**:

	tuple<int,string,double> t1 = {1,"Hamlet",3.14};	// explicit type
	auto t2 = make_tuple(1,"Ophelia"s,3.14);			// better; deduced type

Note the use of the `s` suffix to ensure that the string is a `std::string`, rather than a C-style string.

**Note**: Since you can trivially write a `make_T` function, so could the compiler. Thus, `make_T` functions may become redundant in the future.

**Exception**: Sometimes there isn't a good way of getting the template arguments deduced and sometimes, you want to specify the arguments explicitly:

	vector<double> v = { 1, 2, 3, 7.9, 15.99 };
	list<Record*> lst;

**Enforcement**: Flag uses where an explicitly specialized type exactly matches the types of the arguments used.



<a name="Rt-regular"></a>
### T.46: Require template arguments to be at least `Regular` or `SemiRegular`

**Reason**: ???

**Example**:

	???

**Enforcement**: ???


<a name="Rt-visible"></a>
### T.47: Avoid highly visible unconstrained templates with common names

**Reason**: ???

**Example**:

	???

**Enforcement**: ???

<a name="Rt-concept-def"></a>
### T.48: If your compiler does not support concepts, fake them with `enable_if`

**Reason**: ???

**Example**:

	???

**Enforcement**: ???


<a name="Rt-erasure"></a>
### T.49: Where possible, avoid type-erasure

**Reason**: Type erasure incurs an extra level of indirection by hiding type information behind a separate compilation boundary.

**Example**:

	???

**Exceptions**: Type erasure is sometimes appropriate, such as for `std::function`.

**Enforcement**: ???


<a name="Rt-unconstrained-adl"></a>
### T.50: Avoid writing an unconstrained template in the same namespace as a type

**Reason**: ADL will find the template even when you think it shouldn't.

**Example**:

	???

**Note**: This rule should not be necessary; the committee cannot agree on how to fix ADL, but at least making it not consider unconstrained templates would solve many of the actual problems and remove the need for this rule.

**Enforcement**: ??? unfortunately this will get many false positives; the standard library violates this widely, by putting many unconstrained templates and types into the single namespace `std`



<a name="SS=temp-def"></a>
## TCP.def: Template definitions

???


<a name="Rt-depend"></a>
### T.60: Minimize a template's context dependencies

**Reason**: Eases understanding. Minimizes errors from unexpected dependencies. Eases tool creation.

**Example**:

	???
	
**Note**: Having a template operate only on its arguments would be one way of reducing the number of dependencies to a minimum,
but that would generally be unmaneageable. For example, an algorithm usually uses other algorithms.

**Enforcement**: ??? Tricky


<a name="Rt-scary"></a>
### T.61: Do not over-parameterize members (SCARY)

**Reason**: A member that does not depend on a template parameter cannot be used except for a specific template argument.
This limits use and typically increases code size.

**Example, bad**:

	template<typename T, typename A = std::allocator{}>
		// requires Regular<T> && Allocator<A>
	class List {
	public:
		struct Link {	// does not depend on A
			T elem;
			T* pre;
			T* suc;
		};
		
		using iterator = Link*;
		
		iterator first() const { return head; }
		
		// ...
	private:
		Node* head;
	};
	
	
	List<int> lst1;
	List<int,my_allocator> lst2;
	
	???

This looks innocent enough, but ???

	template<typename T>
	struct Link {
		T elem;
		T* pre;
		T* suc;
	};
		
	template<typename T, typename A = std::allocator{}>
		// requires Regular<T> && Allocator<A>
	class List2 {
	public:
		
		using iterator = Link<T>*;
		
		iterator first() const { return head; }
		
		// ...
	private:
		Node* head;
	};
	
	List<int> lst1;
	List<int,my_allocator> lst2;
	
	???

**Enforcement**:

* Flag member types that do not depend on every template argument
* Flag member functions that do not depend on every template argument


<a name="Rt-nondependent"></a>
### T.62: Place non-dependent template members in a non-templated base class

**Reason**: ???

**Example**:

	template<typename T>
	class Foo {
	public:
		enum { v1, v2 };
		// ...
	};
	
???

	struct Foo_base {
		enum { v1, v2 };
		// ...
	};
	
	template<typename T>
	class Foo : public Foo_base {
	public:
		// ...
	};
	
**Note**: A more general version of this rule would be
"If a template class member depends on only N template parameters out of M, place it in a base class with only N parameters."
For N==1, we have a choice of a base class of a class in the surrounding scope as in [T.41](#Rt-scary).

??? What about constants? class statics?

**Enforcement**:

* Flag ???


<a name="Rt-specialization"></a>
### T.64: Use specialization to provide alternative implementations of class templates

**Reason**: A template defines a general interface.
Specialization offers a powerful mechanism for providing alternative implementations of that interface.

**Example**:

	??? string specialization (==)
	
	??? representation specialization ?
	
**Note**: ???

**Enforcement**: ???


<a name="Rt-tag-dispatch"></a>
### T.65: Use tag dispatch to provide alternative implementations of a function

**Reason**: A template defines a general interface. ???

**Example**:

	??? that's how we get algorithms like `std::copy` which compiles into a `memmove` call if appropriate for the arguments.

**Note**: When `concept`s become available such alternatives can be distinguished directly.

**Enforcement**: ???


<a name="Rt-enable_if"></a>
### T.66: Use selection using `enable_if` to optionally define a function

**Reason**: ???

**Example**:

	???

**Enforcement**: ???


<a name="Rt-customization"></a>
### T.69: Inside a template, don't make an unqualified nonmember function call unless you intend it to be a customization point

**Reason**: To provide only intended flexibility, and avoid accidental environmental changes.

If you intend to call your own helper function `helper(t)` with a value `t` that depends on a template type parameter, put it in a `::detail` namespace and qualify the call as `detail::helper(t);`. Otherwise the call becomes a customization point where any function `helper` in the namespace of `t`'s type can be invoked instead -- falling into the second option below, and resulting in problems like [unintentionally invoking unconstrained function templates of that name that happen to be in the same namespace as `t`'s type](#Rt-unconstrained-adl).

There are three major ways to let calling code customize a template.

* Call a member function. Callers can provide any type with such a named member function.

	template<class T>
	void test(T t) {
		t.f();			// require T to provide f()
	}

* Call a nonmember function without qualification. Callers can provide any type for which there is such a function available in the caller's context or in the namespace of the type.

	template<class T>
	void test(T t) {
		f(t);			// require f(/*T*/) be available in caller's cope or in T's namespace
	}
	
* Invoke a "trait" -- usually a type alias to compute a type, or a `constexpr` function to compute a value, or in rarer cases a traditional traits template to be specialized on the user's type.

	template<class T>
	void test(T t) {
		test_traits<T>::f(t);	// require customizing test_traits<> to get non-default functions/types
		test_traits<T>::value_type x;
	}

**Enforcement**:
* In a template, flag an unqualified call to a nonmember function that passes a variable of dependent type when there is a nonmember function of the same name in the template's namespace.


<a name="SS-temp-hier"></a>
## T.temp-hier: Template and hierarchy rules:

Templates are the backbone of C++'s support for generic programming and class hierarchies the backbone of its support
for object-oriented programming.
The two language mechanisms can be use effectively in combination, but a few design pitfalls must be avoided.


<a name="Rt-hier"></a>
### T.80: Do not naively templatize a class hierarchy

**Reason**: Templatizing a class hierarchy that has many functions, especially many virtual functions, can lead to code bloat.

**Example, bad**:

	template<typename T>
	struct Container {			// an interface
		virtual T* get(int i);
		virtual T* first();
		virtial T* next();
		virtual void sort();
	};
	
	template<typename T>
	class Vector : public Container<T> {
	public:
		// ...
	};

	Vector<int> vi;
	Vector<string> vs;

It is probably a dumb idea to define a `sort` as a member function of a container,
but it is not unheard of and it makes a good example of what not to do.
 
Given this, the compiler cannot know if `Vector<int>::sort()` is called, so it must generate code for it.
Similar for `Vector<string>::sort()`.
Unless those two functions are called that's code bloat.
Imagine what this would do to a class hierarchy with dozens of member functions and dozens of derived classes with many instantiations.

**Note**: In many cases you can provide a stable interface by not parameterizing a base; see [???](#Rt-abi).

**Enforcement**:

* Flag virtual functions that depend on a template argument. ??? False positives


<a name="Rt-array"></a>
### T.81: Do not mix hierarchies and arrays

**Reason**: An array of derived classes can implicitly "decay" to a pointer to a base class with potential disastrous results.

**Example**: Assume that `Apple` and `Pear` are two kinds of `Fruit`s.

	void maul(Fruit* p)
	{
		*p = Pear{};	// put a Pear into *p
		p[1] = Pear{};	// put a Pear into p[2]
	}
	
	Apple aa [] = { an_apple, another_apple };	// aa contains Apples (obviously!)
	
	maul(aa);
	Apple& a0 = &aa[0];	// a Pear?
	Apple& a1 = &aa[1];	// a Pear?

Probably, `aa[0]` will be a `Pear` (without the use af a cast!).
If `sizeof(Apple)!=sizeof(Pear)` the access to `aa[1]` will not be aligned to the proper start of an object in the array.
We have a type violation and possibly (probably) a memory corruption.
Never write such code.

Note that `maul()` violates the a `T*` points to an individual object [Rule](#???).

**Alternative**: Use a proper container:

	void maul2(Fruit* p)
	{
		*p = Pear{};	// put a Pear into *p
	}
	
	vector<Apple> va = { an_apple, another_apple };	// aa contains Apples (obviously!)
	
	maul2(aa);		// error: cannot convert a vector<Apple> to a Fruit*
	maul2(&aa[0]);	// you asked for it
	
	Apple& a0 = &aa[0];	// a Pear?

Note that the assignment in `maul2()` violated the no-slicing [Rule](#???).

**Enforcement**:

* Detect this horror!


<a name="Rt-linear"></a>
### T.82: Linearize a hierarchy when virtual functions are undesirable

**Reason**: ???

**Example**:

	???

**Enforcement**: ???


<a name="Rt-virtual"></a>
### T.83: Do not declare a member function template virtual

**Reason** C++ does not support that.
If it did, vtbls could not be generated until link time.
And in general, implementations must deal with dynamic linking.

**Example; don't**:

	class Shape {
		// ...
		template<class T>
		virtual bool intersect(T* p);	// error: template cannot be virtual
	};

**Alternative**: ??? double dispatch, visitor, calculate which function to call

**Enforcement**: The compiler handles that.



<a name="Rt-abi"></a>
### T.84: Use a non-template core implementation to provide an ABI-stable interface

**Reason**: Improve stability of code. Avoids code bloat.

**Example**: It could be a base class:

	struct Link_base {		// stable
		Link* suc;
		Link* pre;
	};
	
	template<typename T>	// templated wrapper to add type safety
	struct Link : Link_base {
		T val;
	};
	
	struct List_base {
		Link_base* first;	// first element (if any)
		int sz;				// number of elements
		void add_front(Link_base* p);
		// ...
	};
	
	template<typename T>
	class List : List_base {
	public:
		void put_front(const T& e) { add_front(new Link<T>{e}); }	// implicit cast to Link_base
		T& front() { static_cast<Link<T>*>(first).val; }			// explicit cast back to Link<T>
		// ...
	};
	
	List<int> li;
	List<string> ls;

Now there is only one copy of the operations linking and unlinking elements of a `List`.
The `Link` and `List` classes does nothing but type manipulation.

Instead of using a separate "base" type, another common technique is to specialize for `void` or `void*` and have the general template for `T` be just the safely-encapsulated casts to and from the core `void` implementation.
	
**Alternative**: Use a [PIMPL](#???) implementation.

**Enforcement**: ???


<a name="SS-variadic"></a>
## T.var: Variadic template rules

???


<a name="Rt-variadic"></a>
### T.100: Use variadic templates when you need a function that takes a variable number of arguments of a variety of types

**Reason**: Variadic templates is the most general mechanism for that, and is both efficient and type-safe. Don't use C varargs.

**Example**:

	??? printf

**Enforcement**:

    * Flag uses of `va_arg` in user code.


<a name="Rt-variadic-pass"></a>
### T.101: ??? How to pass arguments to a variadic template ???

**Reason**: ???

**Example**:

	??? beware of move-only and reference arguments

**Enforcement**: ???


<a name="Rt-variadic-process"></a>
### T.102: How to process arguments to a variadic template

**Reason**: ???

**Example**:

	??? forwarding, type checking, references

**Enforcement**: ???


<a name="Rt-variadic-not"></a>
### T.103: Don't use variadic templates for homogeneous argument lists

**Reason** There are more precise ways of specifying a homogeneous sequence, such as an `initializer_list`.

**Example**:

	???

**Enforcement**: ???


<a name="SS-meta"></a>
## T.meta: Template metaprogramming (TMP)

Templates provide a general mechanism for compile-time programming.

Metaprogramming is programming where at least one input or one result is a type.
Templates offer Turing-complete (modulo memory capacity) duck typing at compile time.
The syntax and techniques needed are pretty horrendous.


<a name="Rt-metameta"></a>
### T.120: Use template metaprogramming only when you really need to

**Reason**: Template metaprogramming is hard to get right, slows down compilation, and is often very hard to maintain.
However, there are real-world examples where template metaprogramming provides better performance that any alternative short of expert-level assembly code.
Also, there are real-world examples where template metaprogramming expresses the fundamental ideas better than run-time code.
For example, if you really need AST manipulation at compile time (e.g., for optional matrix operation folding) there may be no other way in C++.

**Example, bad**:

	???
	
**Example, bad**:

	enable_if

Instead, use concepts. But see [How to emulate concepts if you don't have language support](#Rt-emulate).

**Example**:

	??? good

**Alternative**: If the result is a value, rather than a type, use a [`constexpr` function](#Rt-fct).

**Note**: If you feel the need to hide your template metaprogramming in macros, you have probably gone too far.


<a name="Rt-emulate"></a>
### T.121: Use template metaprogramming primarily to emulate concepts

**Reason**: Until concepts become generally available, we need to emulate them using TMP.
Use cases that require concepts (e.g. overloading based on concepts) are among the most common (and simple) uses of TMP.

**Example**:

	template<typename Iter>
	    /*requires*/ enable_if<random_access_iterator<Iter>,void>
	advance(Iter p, int n) { p += n; }

	template<typename Iter>
	    /*requires*/ enable_if<forward_iterator<Iter>,void>
	advance(Iter p, int n) { assert(n>=0); while (n--) ++p;}
	
**Note**: Such code is much simpler using concepts:

	void advance(RandomAccessIterator p, int n) { p += n; }

	void advance(ForwardIterator p, int n) { assert(n>=0); while (n--) ++p;}

**Enforcement**: ???


<a name="Rt-tmp"></a>
### T.122: Use templates (usually template aliases) to compute types at compile time

**Reason**: Template metaprogramming is the only directly supported and half-way principled way of generating types at compile time.

**Note**: "Traits" techniques are mostly replaced by template aliases to compute types and `constexpr` functions to compute values.

**Example**:

	??? big object / small object optimization

**Enforcement**: ???


<a name="Rt-fct"></a>
### T.123: Use `constexpr` functions to compute values at compile time

**Reason**: A function is the most obvious and conventional way of expressing the computation of a value.
Often a `constexpr` function implies less compile-time overhead than alternatives.

**Note**: "Traits" techniques are mostly replaced by template aliases to compute types and `constexpr` functions to compute values.

**Example**:

	template<typename T>
		// requires Number<T>
	constexpr T pow(T v, int n)	// power/exponential
	{
		T res = 1;
		while (n--) res *= v;
		return res;
	}
	
	constexpr auto f7 = pow(pi,7);

**Enforcement**:

    * Flag template metaprograms yielding a value. These should be replaced with `constexpr` functions.


<a name="Rt-std"></a>
### T.124: Prefer to use standard-library TMP facilities

**Reason**: Facilities defined in the standard, such as `conditional`, `enable_if`, and `tuple`, are portable and can be assumed to be known.

**Example**:

	???

**Enforcement**: ???


<a name="Rt-lib"></a>
### T.125: If you need to go beyond the standard-library TMP facilities, use an existing library

**Reason**: Getting advanced TMP facilities is not easy and using a library makes you part of a (hopefully supportive) community.
Write your own "advanced TMP support" only if you really have to.

**Example**:

	???

**Enforcement**: ???


<a name="SS-temp-other"></a>
## Other template rules


<a name="Rt-name"></a>
### T.140: Name all nontrivial operations

**Reason**: Documentation, readability, opportunity for reuse.

**Example**:

	???
	
**Example; good**:

	???

**Note**: whether functions, lambdas, or operators.

**Exceptions**:

* Lambdas logically used only locally, such as an argument to `for_each` and similar control flow algorithms.
* Lambdas as [initializers](#???)

**Enforcement**: ???


<a name="Rt-lambda"></a>
### T.141: Use an unnamed lambda if you need a simple function object in one place only

**Reason**: That makes the code concise and gives better locality than alternatives.

**Example**:

	??? for-loop equivalent

**Exception**: Naming a lambda can be useful for clarity even if it is used only once

**Enforcement**:

* Look for identical and near identical lambdas (to be replaced with named functions or named lambdas).


<a name="Rt-var"></a>
### T.142?: Use template variables to simplify notation

**Reason**: Improved readability.

**Example**:
	
	???
	
**Enforcement**: ???


<a name="Rt-nongeneric"></a>
### T.143: Don't write unintentionally nongeneric code

**Reason**: Generality. Reusability. Don't gratuitously commit to details; use the most general facilities available.

**Example**: Use `!=` instead of `<` to compare iterators; `!=` works for more objects because it doesn't rely on ordering.
	
	for(auto i = first; i < last; ++i) {	// less generic
		// ...
	}
	
	for(auto i = first; i != last; ++i) {	// good; more generic
		// ...
	}
	
Of course, range-for is better still where it does what you want.

**Example**: Use the least-derived class that has the functionality you need.

	class base {
	public:
		void f();
		void g();
	};
	
	class derived1 : public base {
	public:
		void h();
	};
	
	class derived2 : public base {
	public:
		void j();
	};
	
	void myfunc(derived& param) {	// bad, unless there is a specific reason for limiting to derived1 objects only
		use(param.f());
		use(param.g());
	}
	
	void myfunc(base& param) {		// good, uses only base interface so only commit to that
		use(param.f());
		use(param.g());
	}
	
**Enforcement**:
* Flag comparison of iterators using `<` instead of `!=`.
* Flag `x.size() == 0` when `x.empty()` or `x.is_empty()` is available. Emptiness works for more containers than size(), because some containers don't know their size or are conceptually of unbounded size.
* Flag functions that take a pointer or reference to a more-derived type but only use functions declared in a base type.


<a name="Rt-specialize-function"></a>
### T.144: Don't specialize function templates

**Reason**: You can't partially specialize a function template per language rules. You can fully specialize a function template but you almost certainly want to overload instead -- because function template specializations don't participate in overloading, they don't act as you probably wanted. Rarely, you should actually specialize by delegating to a class template that you can specialize properly.

**Example**:
	
	???

**Exceptions**: If you do have a valid reason to specialize a function template, just write a single function template that delegates to a class template, then specialize the class template (including the ability to write partial specializations).
	
**Enforcement**:
* Flag all specializations of a function template. Overload instead.





<a name="S-cpl"></a>
# CPL: C-style programming

C and C++ are closely related languages.
They both originate in "Classic C" from 1978 and have evolved in ISO committees since then.
Many attempts have been made to keep them compatible, but neither is a subset of the other.

C rule summary:

* [CPL.1: Prefer C++ to C](#Rcpl-C)
* [CPL.2: If you must use C, use the common subset of C and C++, and compile the C code as C++](#Rcpl-subset)
* [CPL.3: If you must use C for interfaces, use C++ in the code using such interfaces](#Rcpl-interface)


<a name="Rcpl-C"></a>
### CPL.1: Prefer C++ to C

**Reason**: C++ provides better type checking and more notational support.
It provides better support for high-level programming and often generates faster code.

**Example**:

	char ch = 7;
	void* pv = &ch;
	int* pi = pv;	// not C++
	*pi = 999;		// overwrite sizeof(int) bytes near &ch

**Enforcement**: Use a C++ compiler.


<a name="Rcpl-subset"></a>
### CPL.2: If you must use C, use the common subset of C and C++, and compile the C code as C++

**Reason**: That subset can be compiled with both C and C++ compilers, and when compiled as C++ is better type checked than "pure C."

**Example**:

	int* p1 = malloc(10*sizeof(int));                      // not C++
	int* p2 = static_cast<int*>(malloc(10*sizeof(int)));   // not C, C-style C++
	int* p3 = new int[10];                                 // not C
	int* p4 = (int*)malloc(10*sizeof(int));                // both C and C++

**Enforcement**:

    * Flag if using a build mode that compiles code as C.
	* The C++ compiler will enforce that the code is valid C++ unless you use C extension options.


<a name="Rcpl-interface"></a>
### CPL.3: If you must use C for interfaces, use C++ in the calling code using such interfaces

**Reason**: C++ is more expressive than C and offer better support for many types of programming.

**Example**: For example, to use a 3rd party C library or C systems interface, define the low-level interface in the common subset of C and C++ for better type checking.
Whenever possible encapsulate the low-level interface in an interface that follows the C++ guidelines (for better abstraction, memory safety, and resource safety) and use that C++ interface in C++ code.

**Example**: You can call C from C++:

    // in C:
    double sqrt(double);

    // in C++:
    extern "C" double sqrt(double);

    sqrt(2);

**Example**: You can call C++ from C:

    // in C:
    X call_f(struct Y*, int);

    // in C++:
    extern "C" X call_f(Y* p, int i)
    {
        return p->f(i);	// possibly a virtual function call
    }

**Enforcement**: None needed


<a name="S-source"></a>
#SF: Source files

Distinguish between declarations (used as interfaces) and definitions (used as implementations)
Use header files to represent interfaces and to emphasize logical structure.

Source file rule summary:

* [SF.1: Use a `.cpp` suffix for code files and `.h` for interface files](#Rs-suffix)
* [SF.2: A `.h` file may not contain object definitions or non-inline function definitions](#Rs-inline)
* [SF.3: Use `.h` files for all declarations used in multiple sourcefiles](#Rs-suffix)
* [SF.4: Include `.h` files before other declarations in a file](#Rs-include-order)
* [SF.5: A `.cpp` file must include the `.h` file(s) that defines its interface](#Rs-consistency)
* [SF.6: Use `using`-directives for transition, for foundation libraries (such as `std`), or within a local scope](#Rs-using)
* [SF.7: Don't put a `using`-directive in a header file](#Rs-using-directive)
* [SF.8: Use `#include` guards for all `.h` files](#Rs-guards)
* [SF.9: Avoid cyclic dependencies among source files](#Rs-cycles)

* [SF.20: Use `namespace`s to express logical structure](#Rs-namespace)
* [SF.21: Don't use an unnamed (anonymous) namespace in a header](#Rs-unnamed)
* [SF.22: Use an unnamed (anonymous) namespace for all internal/nonexported entities](#Rs-unnamed2)



<a name="Rs-suffix"></a>
### SF.1: Use a `.cpp` suffix for code files and `.h` for interface files

**Reason**: Convention

**Note**: The specific names `.h` and `.cpp` are not required (but recommended) and other names are in widespread use.
Examples are `.hh` and `.cxx`. Use such names equivalently.

**Example**:

	// foo.h:
	extern int a;	// a declaration
	extern void foo();
	
	// foo.cpp:
	int a;	// a definition
	void foo() { ++a; }

`foo.h` provides the interface to `foo.cpp`. Global variables are best avoided.

**Example**, bad:

	// foo.h:
	int a;	// a definition
	void foo() { ++a; }

`#include<foo.h>` twice in a program and you get a linker error for two one-definition-rule violations.
	

**Enforcement**:

* Flag non-conventional file names.
* Check that `.h` and `.cpp` (and equivalents) follow the rules below.


<a name="Rs-inline"></a>
### SF.2: A `.h` file may not contain object definitions or non-inline function definitions

**Reason**: Including entities subject to the one-definition rule leads to linkage errors.

**Example**:

	???
	
**Alternative formulation**: A `.h` file must contain only:

* `#include`s of other `.h` files (possibly with include guards
* templates
* class definitions
* function declarations
* `extern` declarations
* `inline` function definitions
* `constexpr` definitions
* `const` definitions
* `using` alias definitions
* ???

**Enforcement**: Check the positive list above.


<a name="Rs-suffix"></a>
### SF.3: Use `.h` files for all declarations used in multiple sourcefiles

**Reason**: Maintainability. Readability.

**Example, bad**:

	// bar.cpp:
	void bar() { cout << "bar\n"; }

	// foo.cpp:
	extern void bar();
	void foo() { bar(); }
	
A maintainer of `bar` cannot find all declarations of `bar` if its type needs changing.
The user of `bar` cannot know if the interface used is complete and correct. At best, error messages come (late) from the linker.

**Enforcement**:

* Flag declarations of entities in other source files not placed in a `.h`.


<a name="Rs-include-order"></a>
### SF.4: Include `.h` files before other declarations in a file

**Reason**: Minimize context dependencies and increase readability.

**Example**:

	#include<vector>
	#include<algorithm>
	#include<string>

	// ... my code here ...

**Example, bad**:

	#include<vector>

	// ... my code here ...

	#include<algorithm>
	#include<string>

**Note**: This applies to both `.h` and `.cpp` files.

**Exception**: Are there any in good code?

**Enforcement**: Easy.
	

<a name="Rs-consistency"></a>
### SF.5: A `.cpp` file must include the `.h` file(s) that defines its interface

**Reason** This enables the compiler to do an early consistency check.

**Example**, bad:

	// foo.h:
	void foo(int);
	int bar(long double);
	int foobar(int);
	
	// foo.cpp:
	void foo(int) { /* ... */ }
	int bar(double) { /* ... */ }
	double foobar(int);

The errors will not be caught until link time for a program calling `bar` or `foobar`.

**Example**:

	// foo.h:
	void foo(int);
	int bar(long double);
	int foobar(int);
	
	// foo.cpp:
	#include<foo.h>
		
	void foo(int) { /* ... */ }
	int bar(double) { /* ... */ }
	double foobar(int);		// error: wrong return type

The return-type error for `foobar` is now caught immediately when `foo.cpp` is compiled.
The argument-type error for `bar` cannot be caught until link time because of the possibility of overloading,
but systematic use of `.h` files increases the likelyhood that it is caught earlier by the programmer.

**Enforcement**: ???


<a name="Rs-using"></a>
### SF.6: Use `using`-directives for transition, for foundation libraries (such as `std`), or within a local scope

**Reason**: ???

**Example**:

	???

**Enforcement**: ???


<a name="Rs-using-directive"></a>
### SF.7: Don't put a `using`-directive in a header file

**Reason** Doing so takes away an `#include`r's ability to effectively disambiguate and to use alternatives.

**Example**:

	???

**Enforcement**: ???


<a name="Rs-guards"></a>
### SF.8: Use `#include` guards for all `.h` files

**Reason**: To avoid files being `#include`d several times.

**Example**:

	// file foobar.h:
	#ifndef FOOBAR_H
	#define FOOBAR_H
	// ... declarations ...
	#endif // FOOBAR_H

**Enforcement**: Flag `.h` files without `#include` guards


<a name="Rs-cycles"></a>
### SF.9: Avoid cyclic dependencies among source files

**Reason**: Cycles complicates comprehension and slows down compilation.
Complicates conversion to use language-supported modules (when they become available).

**Note**: Eliminate cycles; don't just break them with `#include` guards.

**Example, bad**:

	// file1.h:
	#include "file2.h"
	
	// file2.h:
	#include "file3.h"
	
	// file3.h:
	#include "file1.h"

**Enforcement: Flag all cycles.


<a name="Rs-namespace"></a>
### SF.20: Use `namespace`s to express logical structure

**Reason**: ???

**Example**:

	???

**Enforcement**: ???


<a name="Rs-unnamed"></a>
### SF.21: Don't use an unnamed (anonymous) namespace in a header

**Reason**: It is almost always a bug to mention an unnamed namespace in a header file.

**Example**:

	???

**Enforcement**:
* Flag any use of an anonymous namespace in a header file.



<a name="Rs-unnamed2"></a>
### SF.22: Use an unnamed (anonymous) namespace for all internal/nonexported entities

**Reason**:
nothing external can depend on an entity in a nested unnamed namespace.
Consider putting every definition in an implementation source file should be in an unnamed namespace unless that is defining an "external/exported" entity.

**Example**: An API class and its members can't live in an unnamed namespace; but any "helper" class or function that is defined in an implementation source file should be at an unnamed namespace scope.

	???

**Enforcement**:
* ???


<a name="S-stdlib"></a>
# SL: The Standard Library

Using only the bare language, every task is tedious (in any language).
Using a suitable library any task can be reasonably simple.

Standard-library rule summary:

* [SL.1: Use libraries wherever possible](#Rsl-lib)
* [SL.2: Prefer the standard library to other libraries](#Rsl-sl)
* ???


<a name="Rsl-lib"></a>
### SL.1:  Use libraries wherever possible

**Reason**: Save time. Don't re-invent the wheel.
Don't replicate the work of others.
Benefit from other people's work when they make improvements.
Help other people when you make improvements.

**References**: ???

<a name="Rsl-sl"></a>
### SL.2: Prefer the standard library to other libraries

**Reason**. More people know the standard library.
It is more likely to be stable, well-maintained, and widely available than your own code or most other libraries.

## SL.con: Containers

???

## SL.str: String

???

## SL.io: Iostream

???

### SL.???: Use character-level input only when you have to; _expr.low_.

### SL.???: When reading, always consider ill-formed input; _expr.low_.

## SL.regex: Regex

???

## SL:c: The C standard library

### SL.???: C-style strings

### SL.???: printf/scanf

<a name"S-A"></a>
# A: Architectural Ideas

This section contains ideas about ???

<a name"Ra-stable"></a>
### A.1 Separate stable from less stable part of code

???

<a name"Ra-reuse"></a>
### A.2 Express potentially reusable parts as a library

???

<a name"Ra-lib"></a>
### A.3 Express potentially separately maintained parts as a library

???


<a name="S-not"></a>
# Non-Rules and myths

This section contains rules and guidelines that are popular somewhere, but that we deliberately don't recommend.
In the context of the styles of programming we recommend and support with the guidelines, these "non-rules" would do harm.

Non-rule summary:

* all declarations on top of function
* single-return rule
* no exceptions
* one class per source file
* two-phase initialization
* goto exit


<a name="S-references"></a>
# RF: References

Many coding standards, rules, and guidelines have been written for C++, and especially for specialized uses of C++.
Many

* focus on lower-level issues, such as the spelling of identifiers
* are written by C++ novices
* see "stopping programmers from doing unusual things" as their primary aim
* aim at portability across many compilers (some 10 years old)
* are written to preserve decades old code bases
* aims at a single application domain
* are downright counterproductive
* are ignored (must be ignored for programmers to get their work done well)

A bad coding standard is worse than no coding standard.
However an appropriate set of guidelines are much better than no standards: "Form is liberating."

Why can't we just have a language that allows all we want and disallows all we don't want ("a perfect language")?
Fundamentally, because affordable languages (and their tool chains) also serve people with needs that differ from yours and serve more needs than you have today.
Also, your needs change over time and a general-purpose language is needed to allow you to adapt.
A language that is ideal for today would be overly restrictive tomorrow.

Coding guidelines adapt the use of a language to specific needs.
Thus, there cannot be a single coding style for everybody.
We expect different organizations to provide additions, typically with more restrictions and firmer style rules.

Reference sections:

* [RF.rules: Coding rules](#SS-rules)
* [RF.books: Books with coding guidelines](#SS-books)
* [RF.C++: C++ Programming (C++11/C++14)](#SS-C++)
* [RF.web: Websites](#SS-web)
* [RS.video: Videos about "modern C++"](#SS-vid)
* [RF.man: Manuals](#SS-man)


<a name="SS-rules"></a>
## RF.rules: Coding rules

* [Boost Library Requirements and Guidelines](http://www.boost.org/development/requirements.html).
???.
* [Bloomberg: BDE C++ Coding](https://github.com/bloomberg/bde/wiki/CodingStandards.pdf).
Has a stong emphasis on code organization and layout.
* Facebook: ???
* [GCC Coding Conventions](https://gcc.gnu.org/codingconventions.html).
C++03 and (reasonably) a bit backwards looking.
* [Google C++ Style Guide](http://google-styleguide.googlecode.com/svn/trunk/cppguide.html).
Too timid and reflects its 1990s origins.
[A critique from 2014](https://www.linkedin.com/pulse/20140503193653-3046051-why-google-style-guide-for-c-is-a-deal-breaker).
Google are busy updating their code base and we don't know how accurately the posted guideline reflects their actual code.
This set of recommendations is evolving.
* [JSF++: JOINT STRIKE FIGHTER AIR VEHICLE C++ CODING STANDARDS](http://www.stroustrup.com/JSF-AV-rules.pdf).
Document Number 2RDU00001 Rev C. December 2005.
For flight control software.
For hard real time.
This means that it is necessarily very restrictive ("if the program fails somebody dies").
For example, no free store allocation or deallocation may occur after the plane takes off (no memory overflow and no fragmentation allowed).
No exception may be used (because there was no available tool for guaranteeing that an exception would be handled within a fixed short time).
Libraries used have to have been approved for mission critical applications.
Any similarities to this set of guidelines are unsurprising because Bjarne Stroustrup was an author of JSF++.
Recommended, but note its very specific focus.
* [Mozilla Portability Guide](https://developer.mozilla.org/en-US/docs/Mozilla/C%2B%2B_Portability_Guide).
As the name indicate, this aims for portability across many (old) compilers.
As such, it is restrictive.
* [Geosoft.no: C++ Programming Style Guidelines](http://geosoft.no/development/cppstyle.html).
???.
* [Possibility.com: C++ Coding Standard](http://www.possibility.com/Cpp/CppCodingStandard.html).
???.
* [SEI CERT: Secure C++ Coding Standard](https://www.securecoding.cert.org/confluence/pages/viewpage.action?pageId=637).
A very nicely done set of rules (with examples and rationales) done for security-sensitive code.
Many of their rules apply generally.
* [High Integrity C++ Coding Standard](http://www.codingstandard.com/).
* [llvm](http://llvm.org/docs/CodingStandards.html).
Somewhat brief, pre-C++11, and (not unreasonably) adjusted to its domain.
* ???


<a name="SS-books"></a>
## RF.books: Books with coding guidelines

* Scott Meyers: Effective C++ (???). Addison-Wesley 2014. Beware of overly technical and overly definite rules.
* Sutter and Alexandrescu: C++ Coding Standards. Addison-Wesley 2005. More a set of meta-rules than a set of rules. Pre-C++11. Recommended.
* <a name="BS2005"></a>
Bjarne Stroustrup: [A rationale for semantically enhanced library languages](http://www.stroustrup.com/SELLrationale.pdf).
LCSD05. October 2005.
* Stroustrup: [A Tour of C++](http://www.stroustrup.com/Tour.html).
Addison Wesley 2014.
Each chapter ends with an advice section consisting of a set of recommendations.
* Stroustrup: [The C++ Programming Language (4th Edition)](http://www.stroustrup.com/4th.html).
Addison Wesley 2013.
Each chapter ends with an advice section consisting of a set of recommendations.
* Stroustrup: [Style Guide](http://www.stroustrup.com/Programming/PPP-style.pdf)
for [Programming: Principles and Practice using C++](http://www.stroustrup.com/programming.html).
Mostly low-level naming and layout rules.
Primarily a teaching tool.


<a name="SS-C++"></a>
## RF.C++: C++ Programming (C++11/C++14)

* TC++PL4
* Tour++
* Programming: Principles and Practice using C++


<a name="SS-web"></a>
## RF.web: Websites

* [isocpp.org](http://www.isocpp.com)
* [Bjarne Stroustrup's home pages](http://www.stroustrup.com)
* [WG21](http://www.open-std.org/jtc1/sc22/wg21/)
* [Boost](http://www.boost.org)
* [Adobe open source](http://www.adobe.com/open-source.html)
* [Poco libraries](http://pocoproject.org/)



<a name="SS-vid"></a>
## RS.video: Videos about "modern C++"

* Bjarne Stroustrup: [C++11 Style](http://channel9.msdn.com/Events/GoingNative/GoingNative-2012/Keynote-Bjarne-Stroustrup-Cpp11-Style). 2012.
* Bjarne Stroustrup: [The Essence of C++: With Examples in C++84, C++98, C++11, and C++14](http://channel9.msdn.com/Events/GoingNative/2013/Opening-Keynote-Bjarne-Stroustrup). 2013
* All the talks from [CppCon '14](https://isocpp.org/blog/2014/11/cppcon-videos-c9)
* Bjarne Stroustrup: [The essence of C++](https://www.youtube.com/watch?v=86xWVb4XIyE) at the University of Edinburgh. 2014.
* Sutter: ???
* ??? more ???


<a name="SS-man"></a>
## RF.man: Manuals

* ISO C++ Standard C++11
* ISO C++ Standard C++14
* Palo Alto "Concepts" TR
* ISO C++ Concepts TS
* WG21 Ranges report
 
 
<a name="SS-ack"></a>
## Acknowledgements

Thanks to the many people who contributed rules, suggestions, supporting information, references, etc.:

* Peter Juhl
* Neil MacIntosh
* Axel Naumann
* Andrew Pardoe
* Gabriel Dos Reis
* Zhuang, Jiangang (Jeff)
* Sergey Zubkov


<a name="S-profile"></a>
# Profiles

A "profile" is a set of deterministic and portably enforceable subset rules (i.e., restrictions) that are designed to achieve a specific guarantee. "Deterministic" means they require only local analysis and could be implemented in a compiler (though they don't need to be). "Portably enforceable" means they are like language rules, so programmers can count on enforcement tools giving the same answer for the same code.

Code written to be warning-free using such a language profile is considered to conform to the profile. Conforming code is considered to be safe by construction with regard to the safety properties targeted by that profile. Conforming code will not be the root cause of errors for that property, although such errors may be introduced into a program by other code, libraries or the external environment. A profile may also introduce additional library types to ease conformance and encourage correct code.

Profiles summary:

* [Pro.type: Type safety](#SS-type)
* [Pro.bounds: Bounds safety](#SS-bounds)
* [Pro.lifetime: Lifetime safety](#SS-lifetime)



<a name="SS-type"></a>
## Type safety profile

This profile makes it easier to construct code that uses types correctly and avoids inadvertent type punning. It does so by focusing on removing the primary sources of type violations, including unsafe uses of casts and unions.

For the purposes of this section, type-safety is defined to be the property that a program does not use a variable as a type it is not. Memory accessed as a type `T` should not be valid memory that actually contains an object of an unrelated type `U`. (Note that the safety is intended to be complete when combined also with [Bounds safety](#SS-bounds) and [Lifetime safety](#SS-lifetime).)

The following are under consideration but not yet in the rules below, and may be better in other profiles:

   - narrowing arithmetic promotions/conversions (likely part of a separate safe-arithmetic profile)
   - arithmetic cast from negative floating point to unsigned integral type (ditto)
   - selected undefined behavior: ??? this is a big bucket, start with Gaby's UB list
   - selected unspecified behavior: ??? would this really be about safety, or more a portability concern?
   - constness violations? if we rely on it for safety

An implementation of this profile shall recognize the following patterns in source code as non-conforming and issue a diagnostic.


<a name="Pro-type-reinterpretcast"></a>
### Type.1: Don't use `reinterpret_cast`.

**Reason**:
Use of these casts can violate type safety and cause the program to access a variable that is actually of type `X` to be accessed as if it were of an unrelated type `Z`.

**Example; bad**:

    std::string s = "hello world";
    double* p = reinterpret_cast<double*>(&s); // BAD

**Enforcement**: Issue a diagnostic for any use of `reinterpret_cast`. To fix: Consider using a `variant` instead.


<a name="Pro-type-downcast"></a>
### Type.2: Don't use `static_cast` downcasts. Use `dynamic_cast` instead.

**Reason**:
Use of these casts can violate type safety and cause the program to access a variable that is actually of type `X` to be accessed as if it were of an unrelated type `Z`.

**Example; bad**:

    class base { public: virtual ~base() =0; };

    class derived1 : public base { };

    class derived2 : public base {
        std::string s;
    public:
        std::string get_s() { return s; }
    };

    derived1 d1;
    base* p = &d1; // ok, implicit conversion to pointer to base is fine

    derived2* p2 = static_cast<derived2*>(p); // BAD, tries to treat d1 as a derived2, which it is not
    cout << p2.get_s(); // tries to access d1's nonexistent string member, instead sees arbitrary bytes near d1

**Enforcement**: Issue a diagnostic for any use of `static_cast` to downcast, meaning to cast from a pointer or reference to `X` to a pointer or reference to a type that is not `X` or an accessible base of `X`. To fix: If this is a downcast or cross-cast then use a `dynamic_cast` instead, otherwise consider using a `variant` instead.


<a name="Pro-type-constcast"></a>
### Type.3: Don't use `const_cast` to cast away `const` (i.e., at all).

**Reason**:
Casting away `const` is a lie. If the variable is actually declared `const`, it's a lie punishable by undefined behavior.

**Example; bad**:

    void f(const int& i) {
        const_cast<int&>(i) = 42;   // BAD
    }

    static int i = 0;
    static const int j = 0;

    f(i); // silent side effect
    f(j); // undefined behavior
    

**Exception**: You may need to cast away `const` when calling `const`-incorrect functions. Prefer to wrap such functions in inline `const`-correct wrappers to encapsulate the cast in one place.

**Enforcement**: Issue a diagnostic for any use of `const_cast`. To fix: Either don't use the variable in a non-`const` way, or don't make it `const`.


<a name="Pro-type-cstylecast"></a>
### Type.4: Don't use C-style `(T)expression` casts that would perform a `static_cast` downcast, `const_cast`, or `reinterpret_cast`.

**Reason**:
Use of these casts can violate type safety and cause the program to access a variable that is actually of type `X` to be accessed as if it were of an unrelated type `Z`.
Note that a C-style `(T)expression` cast means to perform the first of the following that is possible: a `const_cast`, a `static_cast`, a `static_cast` followed by a `const_cast`, a `reinterpret_cast`, or a `reinterpret_cast` followed by a `const_cast`. This rule bans `(T)expression` only when used to perform an unsafe cast.

**Example; bad**:

    std::string s = "hello world";
    double* p = (double*)(&s); // BAD

    class base { public: virtual ~base() = 0; };

    class derived1 : public base { };

    class derived2 : public base {
        std::string s;
    public:
        std::string get_s() { return s; }
    };

    derived1 d1;
    base* p = &d1; // ok, implicit conversion to pointer to base is fine

    derived2* p2 = (derived2*)(p); // BAD, tries to treat d1 as a derived2, which it is not
    cout << p2.get_s(); // tries to access d1's nonexistent string member, instead sees arbitrary bytes near d1

    void f(const int& i) {
        (int&)(i) = 42;   // BAD
    }

    static int i = 0;
    static const int j = 0;

    f(i); // silent side effect
    f(j); // undefined behavior

**Enforcement**: Issue a diagnostic for any use of a C-style `(T)expression` cast that would invoke a `static_cast` downcast, `const_cast`, or `reinterpret_cast`. To fix: Use a `dynamic_cast`, `const`-correct declaration, or `variant`, respectively.



<a name="Pro-type-init"></a>
### Type.5: Don't use a variable before it has been initialized.

[ES.20: Always initialize an object](#Res-always) is required.



<a name="Pro-type-memberinit"></a>
### Type.6: Always initialize a member variable.

**Reason**:
Before a variable has been initialized, it does not contain a deterministic valid value of its type. It could contain any arbitrary bit pattern, which could be different on each call.

**Example**:

    struct X { int i; };

    X x;
    use(x); // BAD, x hs not been initialized

    X x2{}; // GOOD
    use(x2);

**Enforcement**:
   - Issue a diagnostic for any constructor of a non-trivially-constructible type that does not initialize all member variables. To fix: Write a data member initializer, or mention it in the member initializer list.
   - Issue a diagnostic when constructing an object of a trivially constructible type without `()` or `{}` to initialize its members. To fix: Add `()` or `{}`.


<a name="Pro-type-unions"></a>
### Type.7: Avoid accessing members of raw unions. Prefer `variant` instead.

**Reason**:
Reading from a union member assumes that member was the last one written, and writing to a union member assumes another member with a nontrivial destructor had its destructor called. This is fragile because it cannot generally be enforced to be safe in the language and so relies on programmer discipline to get it right.

**Example**:

    union U { int i; double d; };
    
    U u;
    u.i = 42;
    use(u.d); // BAD, undefined
    
    variant<int,double> u;
    u = 42; // u  now contains int
    use(u.get<int>()); // ok
    use(u.get<double>()); // throws ??? update this when standardization finalizes the variant design
    
Note that just copying a union is not type-unsafe, so safe code can pass a union from one piece of unsafe code to another.

**Enforcement**:
   - Issue a diagnostic for accessing a member of a union. To fix: Use a `variant` instead.
   
 

<a name="Pro-type-varargs"></a>
### Type.8: Avoid reading from varargs or passing vararg arguments. Prefer variadic template parameters instead.

**Reason**:
Reading from a vararg assumes that the correct type was actually passed. Passing to varargs assumes the correct type will be read. This is fragile because it cannot generally be enforced to be safe in the language and so relies on programmer discipline to get it right.

**Example**:

    int sum(...) {
        // ...
        while( /*...*/ )
            result += va_arg(list, int); // BAD, assumes it will be passed ints
        // ...
    }
        
    sum( 3, 2 ); // ok
    sum( 3.14159, 2.71828 ); // BAD, undefined
    
    template<class ...Args>
    auto sum(Args... args) { // GOOD, and much more flexible
        return (... + args); // note: C++17 "fold expression"
    }
        
    sum( 3, 2 ); // ok: 5
    sum( 3.14159, 2.71828 ); // ok: ~5.85987

Note: Declaring a `...` parameter is sometimes useful for techniques that don't involve actual argument passing, notably to declare “take-anything” functions so as to disable "everything else" in an overload set or express a catchall case in a template metaprogram.
    
**Enforcement**:
   - Issue a diagnostic for using `va_list`, `va_start`, or `va_arg`. To fix: Use a variadic template parameter list instead.
   - Issue a diagnostic for passing an argument to a vararg parameter. To fix: Use a different function, or `[[suppress(types)]]`.



<a name="SS-bounds"></a>
## Bounds safety profile

This profile makes it easier to construct code that operates within the bounds of allocated blocks of memory. It does so by focusing on removing the primary sources of bounds violations: pointer arithmetic and array indexing. One of the core features of this profile is to restrict pointers to only refer to single objects, not arrays.

For the purposes of this document, bounds-safety is defined to be the property that a program does not use a variable to access memory outside of the range that was allocated and assigned to that variable. (Note that the safety is intended to be complete when combined also with [Type safety](#SS-type) and [Lifetime safety](#SS-lifetime), which cover other unsafe operations that allow bounds violations, such as type-unsafe casts that 'widen' pointers.)

The following are under consideration but not yet in the rules below, and may be better in other profiles:

   -

An implementation of this profile shall recognize the following patterns in source code as non-conforming and issue a diagnostic.


<a name="Pro-bounds-arithmetic"></a>
### Bounds.1: Don't use pointer arithmetic. Use `array_view` instead.

**Reason**:
Pointers should only refer to single objects, and pointer arithmetic is fragile and easy to get wrong. `array_view` is a bounds-checked, safe type for accessing arrays of data.

**Example; bad**:

    void f(int* p, int count)
    {
        if (count < 2) return;

        int* q = p + 1; // BAD

        ptrdiff_t d;
        int n;
        d = (p - &n); // OK
        d = (q - p); // OK

        int n = *p++; // BAD

        if (count < 6) return;

        p[4] = 1; // BAD

        p[count - 1] = 2; // BAD

        use(&p[0], 3); // BAD
    }

**Example; good**:

    void f(array_view<int> a) // BETTER: use array_view in the function declaration
    {
        if (a.length() < 2) return;

        int n = *a++; // OK

        array_view<int> q = a + 1; // OK

        if (a.length() < 6) return;

        a[4] = 1; // OK

        a[count – 1] = 2; // OK

        use(a.data(), 3); // OK
    }

**Enforcement**:
Issue a diagnostic for any arithmetic operation on an expression of pointer type that results in a value of pointer type.


<a name="Pro-bounds-arrayindex"></a>
### Bounds.2: Only index into arrays using constant expressions.

**Reason**:
Dynamic accesses into arrays are difficult for both tools and humans to validate as safe. `array_view` is a bounds-checked, safe type for accessing arrays of data. `at()` is another alternative that ensures single accesses are bounds-checked. If iterators are needed to access an array, use the iterators from an `array_view` constructed over the array.

**Example; bad**:

    void f(array<int,10> a, int pos)
    {
        a[pos/2] = 1; // BAD
        a[pos-1] = 2; // BAD
        a[-1] = 3;    // BAD - no replacement, just don't do this
        a[10] = 4;    // BAD - no replacement, just don't do this
    }

**Example; good**:

    // ALTERNATIVE A: Use an array_view

	// A1: Change parameter type to use array_view
    void f(array_view<int,10> a, int pos)
    {
        a[pos/2] = 1; // OK
        a[pos-1] = 2; // OK
    }

    // A2: Add local array_view and use that
    void f(array<int,10> arr, int pos)
    {
        array_view<int> a = arr, int pos)
        a[pos/2] = 1; // OK
        a[pos-1] = 2; // OK
    }

    // ALTERNATIVE B: Use at() for access
    void f()(array<int,10> a, int pos)
    {
        at(a, pos/2) = 1; // OK
        at(a, pos-1) = 2; // OK
    }

**Example; bad**:

    void f()
    {
        int arr[COUNT];
        for (int i = 0; i < COUNT; ++i)
            arr[i] = i; // BAD, cannot use non-constant indexer
    }

**Example; good**:

    // ALTERNATIVE A: Use an array_view
    void f()
    {
        int arr[COUNT];
		array_view<int> av = arr;
        for (int i = 0; i < COUNT; ++i)
            av[i] = i;
    }

    // ALTERNATIVE B: Use at() for access
    void f()
    {
        int arr[COUNT];
        for (int i = 0; i < COUNT; ++i)
            at(arr,i) = i;
    }


**Enforcement**:
Issue a diagnostic for any indexing expression on an expression or variable of array type (either static array or `std::array`) where the indexer is not a compile-time constant expression.

Issue a diagnostic for any indexing expression on an expression or variable of array type (either static array or `std::array`) where the indexer is not a value between `0` or and the upper bound of the array.

**Rewrite support**: Tooling can offer rewrites of array accesses that involve dynamic index expressions to use `at()` instead:

    static int a[10];

    void f(int i, int j)
    {
    	a[i + j] = 12; 		// BAD, could be rewritten as...
        at(a, i + j) = 12; 	// OK - bounds-checked
    }


<a name="Pro-bounds-decay"></a>
### Bounds.3: No array-to-pointer decay.

**Reason**:
Pointers should not be used as arrays. `array_view` is a bounds-checked, safe alternative to using pointers to access arrays.

**Example; bad**:

    void g(int* p, size_t length);

    void f()
    {
        int a[5];
        g(a, 5);        // BAD
        g(&a[0], 1);    // OK
    }

**Example; good**:

    void g(int* p, size_t length);
    void g1(array_view<int> av); // BETTER: get g() changed.

    void f()
    {
        int a[5];
        array_view av = a;

        g(a.data(), a.length());	// OK, if you have no choice
        g1(a);                      // OK - no decay here, instead use implicit array_view ctor
    }

**Enforcement**:
Issue a diagnostic for any expression that would rely on implicit conversion of an array type to a pointer type.


<a name="Pro-bounds-stdlib"></a>
### Bounds.4: Don't use standard library functions and types that are not bounds-checked.

**Reason**:
These functions all have bounds-safe overloads that take `array_view`. Standard types such as `vector` can be modified to perform bounds-checks under the bounds profile (in a compatible way, such as by adding contracts), or used with `at()`.

**Example; bad**:

    void f()
    {
        array<int,10> a, b;
        memset(a.data(), 0, 10); 		// BAD, and contains a length error
        memcmp(a.data(), b.data(), 10); // BAD, and contains a length error
    }

**Example; good**:

    void f()
    {
        array<int,10> a, b;
        memset(a, 0); 	// OK
        memcmp({a,b}); 	// OK
    }

**Example**: If code is using an unmodified standard library, then there are still workarounds that enable use of `std::array` and `std::vector` in a bounds-safe manner. Code can call the `.at()` member function on each class, which will result in an `std::out_of_range` exception being thrown. Alternatively, code can call the `at()` free function, which will result in fail-fast (or a customized action) on a bounds violation.

    void f(std::vector<int>& v, std::array<int, 12> a, int i)
    {
        v[0] = a[0];        // BAD
        v.at(0) = a[0];     // OK (alternative 1)
        at(v, 0) = a[0];    // OK (alternative 2)

        v.at(0) = a[i];     // BAD
        v.at(0) = a.at(i)   // OK (alternative 1)
        v.at(0) = at(a, i); // OK (alternative 2)
    }

**Enforcement**:
   - Issue a diagnostic for any call to a standard library function that is not bounds-checked. ??? insert link to a list of banned functions

**TODO Notes**:
   - Impact on the standard library will require close coordination with WG21, if only to ensure compatibility even if never standardized.
   - We are considering specifying bounds-safe overloads for stdlib (especially C stdlib) functions like `memcmp` and shipping them in the GSL.
   - For existing stdlib functions and types like `vector` that are not fully bounds-checked, the goal is for these features to be bounds-checked when called from code with the bounds profile on, and unchecked when called from legacy code, possibly using constracts (concurrently being proposed by several WG21 members).




<a name="SS-lifetime"></a>
## Lifetime safety profile






<a name="S-gsl"></a>
# GSL: Guideline support library

The GSL is a small library of facilities designed to support this set of guidelines.
Without these facilities, the guidelines would have to be far more restrictive on language details.

The Core Guidelines support library is defined in namespace `gsl` and the names may be aliases for standard library or other well-known library names. Using the (compile-time) indirection through the `gsl` namespace allows for experimentation and for local variants of the support facilities.

The support library facilities are designed to be extremely lightweight (zero-overhead) so that they impose no overhead compared to using conventional alternatives.
Where desirable, they can be "instrumented" with additional functionality (e.g., checks) for tasks such as debugging.

These Guidelines assume a `variant` type, but this is not currently in GSL because the design is being actively refined in the standards committee.


<a name="SS-views"></a>
## GSL.view: Views

These types allow the user to distinguish between owning and non-owning pointers and between pointers to a single object and pointers to the first element of a sequence.

These "views" are never owners.

References are never owners.

The names are mostly ISO standard-library style (lower case and underscore):

* `T*`			// The `T*` is not an owner, may be `nullptr` (Assumed to be pointing to a single element)
* `char*`		// A C-style string (a zero-terminated array of characters); can be `nullptr`
* `const char*`	// A C-style string; can be `nullptr`
* `T&`			// The `T&` is not an owner, may not be `&(T&)*nullptr` (language rule)

The "raw-pointer" notation (e.g. `int*`) is assumed to have its most common meaning; that is, a pointer points to an object, but does not own it.
Owners should be converted to resource handles (e.g., `unique_ptr` or `vector<T>`) or marked `owner<T*>`

* `owner<T*>`		// a `T*`that owns the object pointed/referred to; can be `nullptr`
* `owner<T&>`		// a `T&` that owns the object pointed/referred to

`owner` is used to mark owning pointers in code that cannot be upgraded to use proper resource handles.
Reasons for that include

* cost of conversion
* the pointer is used with an ABI
* the pointer is part of the implementation of a resource handle.

An `owner<T>` differs from a resource handle for a `T` by still requiring an explicit `delete`.

An `owner<T>` is assumed to refer to an object on the free store (heap).

If something is not supposed to be `nullptr`, say so:

* `not_null<T>`		// `T` is usually a pointer type (e.g., `not_null<int*>` and `not_null<owner<Foo*>>`) that may not be `nullptr`.
`T` can be any type for which `==nullptr` is meaningful.

* `array_view<T>`	// [`p`:`p+n`), constructor from `{p,q}` and `{p,n}`; `T` is the pointer type
* `array_view_p<T>`	// `{p,predicate}` [`p`:`q`) where `q` is the first element for which `predicate(*p)` is true
* `string_view`		// `array_view<char>`
* `cstring_view`	// `array_view<const char>`

A `*_view<T>` refers to zero or more mutable `T`s unless `T` is a `const` type.

"Pointer arithmetic" is best done within `array_view`s.
A `char*` that points to something that is not a C-style string (e.g., a pointer into an input buffer) should be represented by an `array_view`.
There is no really good way to say "pointer to a single `char` (`string_view{p,1}` can do that, and `T*` where `T` is a `char` in a template that has not been specialized for C-style strings).

* `zstring`		// a `char*` supposed to be a C-style string; that is, a zero-terminated sequence of `char` or `null_ptr`
* `czstring`	// a `const char*` supposed to be a C-style string; that is, a zero-terminated sequence of `const` `char` ort `null_ptr`

Logically, those last two aliases are not needed, but we are not always logical,
and they make the distinction between a pointer to one `char` and a pointer to a C-style string explicit.
A sequence of characters that is not assumed to be zero-terminated sould be a `char*`, rather than a `zstring`.
French accent optional.

Use `not_null<zstring>` for C-style strings that cannot be `nullptr`. ??? Do we need a name for `not_null<zstring>`? or is its ugliness a feature?


<a name="SS-ownership"></a>
## GSL.owner: Ownership pointers

* `unique_ptr<T>`	// unique ownership: `std::unique_ptr<T>`
* `shared_ptr<T>`	// shared ownership: `std::shared_ptr<T>` (a counted pointer)
* `stack_array<T>`	// A stack-allocated array. The number of elements are determined at construction and fixed thereafter. The elements are mutable unless `T` is a `const` type.
* `dyn_array<T>`	// ??? needed ??? A heap-allocated array. The number of elements are determined at construction and fixed thereafter.
The elements are mutable unless `T` is a `const` type. Basically an `array_view` that allocates and owns its elements.


<a name="SS-assertions"></a>
## GSL.assert: Assertions

* `Expects`		// precondition assertion. Currently placed in function bodies. Later, should be moved to declarations.
				// `Expects(p)` terminates the program unless `p==true`
				// ??? `Expect` in under control of some options (enforcement, error message, alternatives to terminate)
* `Ensures`		// postcondition assertion.	Currently placed in function bodies. Later, should be moved to declarations.


<a name="SS-utilities"></a>
## GSL.util: Utilities

* `finally`		// `finally(f)` makes a `Final_act{f}` with a destructor that invokes `f`
* `narrow_cast`	// `narrow_cast<T>(x)` is `static_cast<T>(x)`
* `narrow`		// `narrow<T>(x)` is `static_cast<T>(x)` if `static_cast<T>(x)==x` or it throws `narrowing_error`
* `implicit`	// "Marker" to put on single-argument constructors to explicitly make them non-explicit
(I don't know how to do that except with a macro: `#define implicit`).
* `move_owner`	// `p=move_owner(q)` means `p=q` but ???


<a name="SS-concepts"></a>
## GSL.concept: Concepts

These concepts (type predicates) are borrowed from Andrew Sutton's Origin library, the Range proposal, and the ISO WG21 Palo Alto TR.
They are likely to be very similar to what will become part of the ISO C++ standard.
The notation is that of the ISO WG21 Concepts TS (???ref???).

* `Range`
* `String`	// ???
* `Number`	// ???
* `Sortable`
* `Pointer`  // A type with `*`, `->`, `==`, and default construction (default construction is assumed to set the singular "null" value) [see smartptrconcepts](#Rr-smartptrconcepts)
* `Unique_ptr`  // A type that matches `Pointer`, has move (not copy), and matches the Lifetime profile criteria for a `unique` owner type [see smartptrconcepts](#Rr-smartptrconcepts)
* `Shared_ptr`   // A type that matches `Pointer`, has copy, and matches the Lifetime profile criteria for a `shared` owner type [see smartptrconcepts](#Rr-smartptrconcepts)
* `EqualityComparable`	// ???Must we suffer CaMelcAse???
* `Convertible`
* `Common`
* `Boolean`
* `Integral`
* `SignedIntegral`
* `SemiRegular`
* `Regular`
* `TotallyOrdered`
* `Function`
* `RegularFunction`
* `Predicate`
* `Relation`
* ...


<a name="S-naming"></a>
# NL: Naming and layout rules

Consistent naming and layout are helpful. If for no other reason because it minimizes "my style is better than your style" arguments.
However, there are many, many, different styles around and people are passionate about them (pro and con).
Also, most real-world projects includes code from many sources, so standardizing on a single style for all code is often impossible.
We present a set of rules that you might use if you have no better ideas, but the real aim is consistency, rather than any particular rule set.
IDEs and tools can help (as well as hinder).

Naming and layout rules:

* [NL 1: Don't say in comments what can be clearly stated in code](#Rl-comments)
* [NL.2: State intent in comments](#Rl-comments-intent)
* [NL.3: Keep comments crisp](#Rl-comments-crisp)
* [NL.4: Maintain a consistent indentation style](#Rl-indent)
* [NL.5: Don't encode type information in names](#Rl-name-type)
* [NL.6: Make the length of a name roughly proportional to the length of its scope](#Rl-name-length)
* [NL.7: Use a consistent naming style](#Rl-name)
* [NL 9: Use ALL_CAPS for macro names only](#Rl-space)
* [NL.10: Avoid CamelCase](#Rl-camel)
* [NL.15: Use spaces sparingly](#Rl-space)
* [NL.16: Use a conventional class member declaration order](#Rl-order)
* [NL.17: Use K&R-derived layout](#Rl-knr)
* [NL.18: Use C++-style declarator layout](#Rl-ptr)

Most of these rules are aesthetic and programmers hold strong opinions.
IDEs also tend to have defaults and a range of alternatives.These rules are suggested defaults to follow unless you have reasons not to.

More specific and detailed rules are easier to enforce.


<a name="Rl-comments"></a>
###  NL.1: Don't say in comments what can be clearly stated in code

**Reason**: Compilers do not read comments.
Comments are less precise than code.
Comments are not updates as consistently as code.

**Example, bad**:

	auto x = m*v1 + vv;	// multiply m with v1 and add the result to vv

**Enforcement**: Build an AI program that interprets colloquial English text and see if what is said could be better expressed in C++.


<a name="Rl-comments-intent"></a>
###  NL.2: State intent in comments

**Reason**: Code says what is done, not what is supposed to be done. Often intent can be stated more clearly and concisely than the implementation.

**Example**:

	void stable_sort(Sortable& c)
		// sort c in the order determined by <, keep equal elements (as defined by ==) in their original relative order
	{
		// ... quite a few lines of non-trivial code ...
	}

**Note**: If the comment and the code disagrees, both are likely to be wrong.


<a name="Rl-comments-crisp"></a>
### NL.3: Keep comments crisp

**Reason**: Verbosity slows down understanding and makes the code harder to read by spreading it around in the source file.

**Enforcement**: not possible.


<a name="Rl-indent"></a>
### NL.4: Maintain a consistent indentation style

**Reason**: Readability. Avoidance of "silly mistakes."

**Example, bad**:

	int i;
	for (i=0; i<max; ++i); // bug waiting to happen
	if (i==j)
		return i;

Enforcement: Use a tool.


<a name="Rl-name-type"></a>
### NL.5 Don't encode type information in names

**Rationale**: If names reflects type rather than functionality, it becomes hard to change the types used to provide that functionality.
Names with types encoded are either verbose or cryptic.
Hungarian notation is evil (at least in a strongly statically-typed language).

**Example**:

	???

**Note**: Some styles distinguishes members from local variable, and/or from global variable.

	struct S {
		int m_;
		S(int m) :m_{abs(m)} { }
	};

This is not evil.

**Note**: Some styles distinguishes types from non-types.

	typename<typename T>
	class Hash_tbl {	// maps string to T
		// ...
	};
	
	Hash_tbl<int> index;

This is not evil.


<a name="Rl-name-length"></a>
### NL.7: Make the length of a name roughly proportional to the length of its scope

**Rationale**: ???

**Example**:

	???

**Enforcement**: ???


<a name="Rl-name"></a>
### NL.8: Use a consistent naming style

**Rationale**: Consistence in naming and naming style increases readability.

**Note**: Where are many styles and when you use multiple libraries, you can't follow all their differences conventions.
Choose a "house style", but leave "imported" libraries with their original style.

**Example**, ISO Standard, use lower case only and digits, separate words with underscores:

	int
	vector
	my_map

Avoid double underscores `__`

**Example**: [Stroustrup](http://www.stroustrup.com/Programming/PPP-style.pdf):
ISO Standard, but with upper case used for your own types and concepts:

	int
	vector
	My_map

**Example**: CamelCase: capitalize each word in a multi-word identifier

	int
	vector
	MyMap
	myMap

Some conventions capitalize the first letter, some don't.

**Note**: Try to be consistent in your use of acronyms, lengths of identifiers:

	int mtbf {12};
	int mean_time_between_failor {12};		// make up your mind

**Enforcement**: Would be possible except for the use of libraries with varying conventions.
	


<a name="Rl-space"></a>
### NL 9: Use ALL_CAPS for macro names only

**Reason**: To avoid confusing macros from names that obeys scope and type rules

**Example**:

	???
	
**Note**: This rule applies to non-macro symbolic constants

	enum bad { BAD, WORSE, HORRIBLE }; // BAD
	
**Enforcement**:

* Flag macros with lower-case letters
* Flag ALL_CAPS non-macro names



<a name="Rl-camel"></a>
### NL.10: Avoid CamelCase

**Reason**: The use of underscores to separate parts of a name is the originale C and C++ style and used in the C++ standard library.
If you prefer CamelCase, you have to choose among different flavors of camelCase.

**Example**:

	???
	
**Enforcement**: Impossible.


<a name="Rl-space"></a>
### NL.15: Use spaces sparingly

**Reason**: Too much space makes the text larger and distracts.

**Example, bad**:

	#include < map >
	
	int main ( int argc , char * argv [ ] )
	{
		// ...
	}
	
**Example**:

	#include<map>
	
	int main(int argc, char* argv[])
	{
		// ...
	}

**Note**: Some IDEs have their own opinions and adds distracting space.

**Note**: We value well-placed whitespace as a significant help for readability. Just don't overdo it.



<a name="Rl-order"></a>
### NL.16: Use a conventional class member declaration order

**Reason**: A conventional order of members improves readability.

When declaring a class use the following order

* types: classes, enums, and aliases (`using`)
* constructors, assignments, destructor
* functions
* data

Used the `public` before `protected` before `private` order.

Private types and functions can be placed with private data.

**Example**:

	???

**Enforcement**: Flag departures from the suggested order. There will be a lot of old code that doesn't follow this rule.


<a name="Rl-knr"></a>
### NL.17: Use K&R-derived layout

**Reason**: This is the original C and C++ layout. It preserves vertical space well. It distinguishes different language constructs (such as functions and classes well).

**Note**: In the context of C++, this style is often called "Stroustrup".

**Example**:

	struct Cable {
		int x;
		// ...
	};
	
	double foo(int x)
	{
		if (0<x) {
			// ...
		}
		
		switch (x) {
		case 0:
			// ...
			break;
		case amazing:
			// ...
			break;
		default:
			// ...
			break;
		}
		
		if (0<x)
			++x;
			
		if (x<0)
			something();
		else
			something_else();
		
		return some_value;
	}

**Note**: a space between `if` and `(`

**Note**: Use separate lines for each statement, the branches of an `if`, and the body of a `for`.

**Note** the `{` for a `class` and a `struct` in *not* on a separate line, but the `{` for a function is.

**Note**: Capitalize the names of your user-defined types to distinguish them from standards-library types.

**Note**: Do not capitalize function names.
		
**Enforcement**: If you want enforcement, use an IDE to reformat.
 

<a name="Rl-ptr"></a>
### NL.18: Use C++-style declarator layout

**Reason**: The C-style layout emphasizes use in expressions and grammar, whereas the C++-style emphasizes types.
The use in expressions argument doesn't hold for references.

**Example**:

	T& operator[](size_t);	// OK
	T &operator[](size_t);	// just strange
	T & operator[](size_t);	// undecided

**Enforcement**: Impossible in the face of history.


<a name="S-libraries"></a>
# Appendix A: Libraries

This section lists recommended libraries, and explicitly recommends a few.

??? Suitable for the general guide? I think not ???


<a name="S-modernizing"></a>
# Appendix B: Modernizing code

Ideally, we follow all rules in all code.
Realistically, we have to deal with a lot of old code:

* application code written before the guidelines were formulated or known
* libraries written to older/different standards
* code that we just haven't gotten around to modernizing

If we have a million lines of new code, the idea of "just changing it all at once" is typically unrealistic.
Thus, we need a way of gradually modernizing a code base.

Upgrading older code to modern style can be a daunting task.
Often, the old code is both a mess (hard to understand) and working correctly (for the current range of uses).
Typically, the original programmer is not around and test cases incomplete.
The fact that the code is a mess dramatically increases to effort needed to make any change and the risk of introducing errors.
Often messy, old code runs unnecessarily slowly because it requires outdated compilers and cannot take advantage of modern hardware.
In many cases, programs support would be required for major upgrade efforts.

The purpose of modernizing code is to simplify adding new functionality, to ease maintenance, and to increase performance (throughput or latency), and to better utilize modern hardware.
Making code "look pretty" or "follow modern style" are not by themselves reasons for change.
There are risks implied by every change and costs (including the cost of lost opportunities) implied by having an outdated code base.
The cost reductions must outweigh the risks.

But how?

There is no one approach to modernizing code.
How best to do it depends on the code, the pressure for updates, the backgrounds of the developers, and the available tool.
Here are some (very general) ideas:

* The ideal is "just upgrade everything." That gives the most benefits for the shortest total time.
In most circumstances, it is also impossible.
* We could convert a code base module for module, but any rules that affects interfaces (especially ABIs), such as [use `array_view`](#SS-views), cannot be done on a per-module basis.
* We could convert code "bottom up" starting with the rules we estimate will give the greatest benefits and/or the least trouble in a given code base.
* We could start by focusing on the interfaces, e.g., make sure that no resources are lost and no pointer is misused.
This would be a set of changes across the whole code base, but would most likely have huge benefits.

Whichever way you choose, please note that the most advantages come with the highest conformance to the guidelines.
The guidelines are not a random set of unrelated rules where you can randomly pick and choose with an expectation of success.

We would dearly love to hear about experience and about tools used.
Modernization can be much faster, simpler, and safer when supported with analysis tools and even code transformation tools.


<a name="S-discussion"></a>
# Appendix C: Discussion

This section contains follow-up material on rules and sets of rules.
In particular, here we present further rationale, longer examples, and discussions of alternatives.

### <a name="Sd order">Discussion: Define and initialize member variables in the order of member declaration</a>

Member variables are always initialized in the order they are declared in the class definition, so write them in that order in the constructor initialization list. Writing them in a different order just makes the code confusing because it won't run in the order you see, and that can make it hard to see order-dependent bugs.

	class Employee {
	    string email, first, last;
	public:
	    Employee(const char* firstName, const char* lastName);
	    // ...
	};

	Employee::Employee(const char* firstName, const char* lastName)
		: first(firstName)
		, last(lastName)
		, email(first + "." + last + "@acme.com")  // BAD: first and last not yet constructed
	{}


In this example, `email` will be constructed before `first` and `last` because it is declared first. That means its constructor will attempt to use `first` and `last` too soon -- not just before they are set to the desired values, but before they are constructed at all.

If the class definition and the constructor body are in separate files, the long-distance influence that the order of member variable declarations has over the constructor's correctness will be even harder to spot.

**References**

[Cline99] Â§22.03-11 ï‚Ÿ [Dewhurst03] Â§52-53 ï‚Ÿ [Koenig97] Â§4 ï‚Ÿ [Lakos96] Â§10.3.5 ï‚Ÿ [Meyers97] Â§13 ï‚Ÿ [Murray93] Â§2.1.3 ï‚Ÿ [Sutter00] Â§47

### <a name="???">Use of `=`, `{}`, and `()` as initializers</a>

???

### <a name="Sd factory">Discussion: Use a factory function if you need "virtual behavior" during initialization</a>

If your design wants virtual dispatch into a derived class from a base class constructor or destructor for functions like `f` and `g`, you need other techniques, such as a post-constructor -- a separate member function the caller must invoke to complete initialization, which can safely call `f` and `g` because in member functions virtual calls behave normally. Some techniques for this are shown in the References. Here's a non-exhaustive list of options:

   * *Pass the buck:* Just document that user code must call the post-initialization function right after constructing an object.
   * *Post-initialize lazily:* Do it during the first call of a member function. A Boolean flag in the base class tells whether or not post-construction has taken place yet.
   * *Use virtual base class semantics:* Language rules dictate that the constructor most-derived class decides which base constructor will be invoked; you can use that to your advantage. (See [Taligent94].)
   * *Use a factory function:* This way, you can easily force a mandatory invocation of a post-constructor function.

Here is an example of the last option:

	class B {
	public:
	    B() { /* ... */ f(); /*...*/ }		// BAD: see Item 49.1

 	   virtual void f() = 0;

 	   // ...
	};

	class B {
	protected:
  	  B() { /* ... */ }
  	  virtual void PostInitialize()       // called right after construction
  	      { /* ... */ f(); /*...*/ }		// GOOD: virtual dispatch is safe
	public:
   		virtual void f() = 0;

    	template<class T>
    	static shared_ptr<T> Create() {		// interface for creating objects
        	auto p = make_shared<T>();
        	p->PostInitialize();
        	return p;
    	}
	};

	class D : public B { /* "¦ */ };			// some derived class

	shared_ptr<D> p = D::Create<D>();		// creating a D object

This design requires the following discipline:

* Derived classes such as `D` must not expose a public constructor. Otherwise, `D`'s users could create `D` objects that don't invoke `PostInitialize`.
* Allocation is limited to `operator new`. `B` can, however, override `new` (see Items 45 and 46).
* `D` must define a constructor with the same parameters that `B` selected. Defining several overloads of `Create` can assuage this problem, however; and the overloads can even be templated on the argument types.

If the requirements above are met, the design guarantees that `PostInitialize` has been called for any fully constructed `B`-derived object. `PostInitialize` doesn't need to be virtual; it can, however, invoke virtual functions freely.

In summary, no post-construction technique is perfect. The worst techniques dodge the whole issue by simply asking the caller to invoke the post-constructor manually. Even the best require a different syntax for constructing objects (easy to check at compile time) and/or cooperation from derived class authors (impossible to check at compile time).

**References**: [Alexandrescu01] Â§3 ï‚Ÿ [Boost] ï‚Ÿ [Dewhurst03] Â§75 ï‚Ÿ [Meyers97] Â§46 ï‚Ÿ [Stroustrup00] Â§15.4.3 ï‚Ÿ [Taligent94]

 

 

###<a name="Sd dtor">Discussion: Make base class destructors public and virtual, or protected and nonvirtual</a>

Should destruction behave virtually? That is, should destruction through a pointer to a `base` class should be allowed? If yes, then `base`'s destructor must be public in order to be callable, and virtual otherwise calling it results in undefined behavior. Otherwise, it should be protected so that only derived classes can invoke it in their own destructors, and nonvirtual since it doesn't need to behave virtually virtual.

**Example**: The common case for a base class is that it's intended to have publicly derived classes, and so calling code is just about sure to use something like a `shared_ptr<base>`:
    
```
class base {
public:
    ~base();                   // BAD, not virtual
    virtual ~base();           // GOOD
    // ...
};

class derived : public base { /*...*/ };

{
    shared_ptr<base> pb = make_shared<derived>();
    // ...
} // ~pb invokes correct destructor only when ~base is virtual
```

In rarer cases, such as policy classes, the class is used as a base class for convenience, not for polymorphic behavior. It is recommended to make those destructors protected and nonvirtual:

```
class my_policy {
public:
    virtual ~my_policy();      // BAD, public and virtual
protected:
    ~my_policy();              // GOOD
    // ...
};

template<class Policy>
class customizable : Policy { /*...*/ }; // note: private inheritance
```

**Note**: This simple guideline illustrates a subtle issue and reflects modern uses of inheritance and object-oriented design principles.

For a base class `Base`, calling code might try to destroy derived objects through pointers to `Base`, such as when using a `shared_ptr<Base>`. If `Base`'s destructor is public and nonvirtual (the default), it can be accidentally called on a pointer that actually points to a derived object, in which case the behavior of the attempted deletion is undefined. This state of affairs has led older coding standards to impose a blanket requirement that all base class destructors must be virtual. This is overkill (even if it is the common case); instead, the rule should be to make base class destructors virtual if and only if they are public.

To write a base class is to define an abstraction (see Items 35 through 37). Recall that for each member function participating in that abstraction, you need to decide:

* Whether it should behave virtually or not.
* Whether it should be publicly available to all callers using a pointer to Base or else be a hidden internal implementation detail.

As described in Item 39, for a normal member function, the choice is between allowing it to be called via a pointer to `Base` nonvirtually (but possibly with virtual behavior if it invokes virtual functions, such as in the NVI or Template Method patterns), virtually, or not at all. The NVI pattern is a technique to avoid public virtual functions.

Destruction can be viewed as just another operation, albeit with special semantics that make nonvirtual calls dangerous or wrong. For a base class destructor, therefore, the choice is between allowing it to be called via a pointer to `Base` virtually or not at all; "nonvirtually" is not an option. Hence, a base class destructor is virtual if it can be called (i.e., is public), and nonvirtual otherwise.

Note that the NVI pattern cannot be applied to the destructor because constructors and destructors cannot make deep virtual calls. (See Items 39 and 55.)

Corollary: When writing a base class, always write a destructor explicitly, because the implicitly generated one is public and nonvirtual. You can always `=default` the implementation if the default body is fine and you're just writing the function to give it the proper visibility and virtuality.

**Exception**: Some component architectures (e.g., COM and CORBA) don't use a standard deletion mechanism, and foster different protocols for object disposal. Follow the local patterns and idioms, and adapt this guideline as appropriate.

Consider also this rare case:

* `B` is both a base class and a concrete class that can be instantiated by itself, and so the destructor must be public for `B` objects to be created and destroyed.
* Yet `B` also has no virtual functions and is not meant to be used polymorphically, and so although the destructor is public it does not need to be virtual.

Then, even though the destructor has to be public, there can be great pressure to not make it virtual because as the first virtual function it would incur all the run-time type overhead when the added functionality should never be needed.

In this rare case, you could make the destructor public and nonvirtual but clearly document that further-derived objects must not be used polymorphically as `B`'s. This is what was done with `std::unary_function`.

In general, however, avoid concrete base classes (see Item 35). For example, `unary_function` is a bundle-of-typedefs that was never intended to be instantiated standalone. It really makes no sense to give it a public destructor; a better design would be to follow this Item's advice and give it a protected nonvirtual destructor.


**References**: [C++CS Item 50]; [Cargill92] pp. 77-79, 207 Å¸ [Cline99] Â§21.06, 21.12-13 Å¸ [Henricson97] pp. 110-114 Å¸ [Koenig97] Chapters 4, 11 Å¸ [Meyers97] Â§14 Å¸ [Stroustrup00] Â§12.4.2 Å¸ [Sutter02] Â§27 Å¸ [Sutter04] Â§18





<a name="Sd-never-fail"></a>
### Dicussion: Destructors, deallocation, and swap must never fail

Never allow an error to be reported from a destructor, a resource deallocation function (e.g., `operator delete`), or a `swap` function using `throw`. It is nearly impossible to write useful code if these operations can fail, and even if something does go wrong it nearly never makes any sense to retry. Specifically, types whose destructors may throw an exception are flatly forbidden from use with the C++ standard library. Most destructors are now implicitly `noexcept` by default.

**Example**:

```
class nefarious {
public:
    nefarious()  { /* code that could throw */ }   // ok
    ~nefarious() { /* code that could throw */ }   // BAD, should not throw
    // ...
};
```

* 1. `nefarious` objects are hard to use safely even as local variables:

```
void test(string& s) {
    nefarious n;          // trouble brewing
    string copy = s;      // copy the string
} // destroy copy and then n
```
Here, copying `s` could throw, and if that throws and if `n`'s destructor then also throws, the program will exit via `std::terminate` because two exceptions can't be propagated simultaneously.

* 2. Classes with `nefarious` members or bases are also hard to use safely, because their destructors must invoke `nefarious`' destructor, and are similarly poisoned by its poor behavior:

```
class innocent_bystander {
    nefarious member;     // oops, poisons the enclosing class's destructor
    // ...
};

void test(string& s) {
    innocent_bystander i; // more trouble brewing
    string copy = s;      // copy the string
} // destroy copy and then i
```

Here, if constructing `copy2` throws, we have the same problem because `i`'s destructor now also can throw, and if so we'll invoke `std::terminate`.

* 3. You can't reliably create global or static `nefarious` objects either:

```
static nefarious n;       // oops, any destructor exception can't be caught
```

* 4. You can't reliably create arrays of `nefarious`:

```
void test() {
    std::array<nefarious,10> arr; // this line can std::terminate(!)
```

The behavior of arrays is undefined in the presence of destructors that throw because there is no reasonable rollback behavior that could ever be devised. Just think: What code can the compiler generate for constructing an `arr` where, if the fourth object's constructor throws, the code has to give up and in its cleanup mode tries to call the destructors of the already-constructed objects... and one or more of those destructors throws? There is no satisfactory answer.

* 5. You can't use `Nefarious` objects in standard containers:

```
std::vector<nefarious> vec(10);   // this is line can std::terminate()
```

The standard library forbids all destructors used with it from throwing. You can't store `nefarious` objects in standard containers or use them with any other part of the standard library.


**Note**: These are key functions that must not fail because they are necessary for the two key operations in transactional programming: to back out work if problems are encountered during processing, and to commit work if no problems occur. If there's no way to safely back out using no-fail operations, then no-fail rollback is impossible to implement. If there's no way to safely commit state changes using a no-fail operation (notably, but not limited to, `swap`), then no-fail commit is impossible to implement.

Consider the following advice and requirements found in the C++ Standard:

> If a destructor called during stack unwinding exits with an exception, terminate is called (15.5.1). So destructors should generally catch exceptions and not let them propagate out of the destructor. --[C++03] Â§15.2(3)
>
> No destructor operation defined in the C++ Standard Library [including the destructor of any type that is used to instantiate a standard library template] will throw an exception. --[C++03] Â§17.4.4.8(3)

Deallocation functions, including specifically overloaded `operator delete` and `operator delete[]`, fall into the same category, because they too are used during cleanup in general, and during exception handling in particular, to back out of partial work that needs to be undone.
Besides destructors and deallocation functions, common error-safety techniques rely also on `swap` operations never failing--in this case, not because they are used to implement a guaranteed rollback, but because they are used to implement a guaranteed commit. For example, here is an idiomatic implementation of `operator=` for a type `T` that performs copy construction followed by a call to a no-fail `swap`:

```
T& T::operator=( const T& other ) {
    auto temp = other;
    swap(temp);
}
```

(See also Item 56. ???)

Fortunately, when releasing a resource, the scope for failure is definitely smaller. If using exceptions as the error reporting mechanism, make sure such functions handle all exceptions and other errors that their internal processing might generate. (For exceptions, simply wrap everything sensitive that your destructor does in a `try/catch(...)` block.) This is particularly important because a destructor might be called in a crisis situation, such as failure to allocate a system resource (e.g., memory, files, locks, ports, windows, or other system objects).

When using exceptions as your error handling mechanism, always document this behavior by declaring these functions `noexcept`. (See Item 75.)


**References**: C++CS Item 51; [C++03] Â§15.2(3), Â§17.4.4.8(3) Å¸ [Meyers96] Â§11 Å¸ [Stroustrup00] Â§14.4.7, Â§E.2-4 Å¸ [Sutter00] Â§8, Â§16 Å¸ [Sutter02] Â§18-19




## <a name ="Sd consistent">Define Copy, move, and destroy consistently</a>

**Reason**: ???


**Note**: If you define a copy constructor, you must also define a copy assignment operator.

**Note**: If you define a move constructor, you must also define a move assignment operator.


**Example**:

    class x {
        // ...
    public:
        x(const x&) { /* stuff */ }

        // BAD: failed to also define a copy assignment operator

        x(x&&) { /* stuff */ }

        // BAD: failed to also define a move assignment operator
    };

    x x1;
    x x2 = x1; // ok
    x2 = x1;   // pitfall: either fails to compile, or does something suspicious


If you define a destructor, you should not use the compiler-generated copy or move operation; you probably need to define or suppress copy and/or move.


	class X {
	    HANDLE hnd;
	    // ...
	public:
 	   ~X() { /* custom stuff, such as closing hnd */ }

		// suspicious: no mention of copying or moving -- what happens to hnd?
	};

	X x1;
	X x2 = x1; // pitfall: either fails to compile, or does something suspicious
	x2 = x1;   // pitfall: either fails to compile, or does something suspicious

If you define copying, and any base or member has a type that defines a move operation, you should also define a move operation.

	class x {
	    string s; // defines more efficient move operations
	    // ... other data members ...
	public:
    	x(const x&) { /* stuff */ }
    	x& operator=(const x&) { /* stuff */ }

    	// BAD: failed to also define a move construction and move assignment
   	 //      (why wasn't the custom "stuff" repeated here?)
	};

	x test()
	{
	    x local;
    	// ...
    	return local;  // pitfall: will be inefficient and/or do the wrong thing
	}

If you define any of the copy constructor, copy assignment operator, or destructor, you probably should define the others.

**Note**:  If you need to define any of these five functions, it means you need it to do more than its default behavior--and the five are asymmetrically interrelated. Here's how:

* If you write/disable either of the copy constructor or the copy assignment operator, you probably need to do the same for the other: If one does "special" work, probably so should the other because the two functions should have similar effects. (See Item 53, which expands on this point in isolation.)
* If you explicitly write the copying functions, you probably need to write the destructor: If the "special" work in the copy constructor is to allocate or duplicate some resource (e.g., memory, file, socket), you need to deallocate it in the destructor.
* If you explicitly write the destructor, you probably need to explicitly write or disable copying: If you have to write a nontrivial destructor, it's often because you need to manually release a resource that the object held. If so, it is likely that those resources require careful duplication, and then you need to pay attention to the way objects are copied and assigned, or disable copying completely.

In many cases, holding properly encapsulated resources using RAII "owning" objects can eliminate the need to write these operations yourself. (See Item 13.)

Prefer compiler-generated (including `=default`) special members; only these can be classified as "trivial," and at least one major standard library vendor heavily optimizes for classes having trivial special members. This is likely to become common practice.

**Exceptions**: When any of the special functions are declared only to make them nonpublic or virtual, but without special semantics, it doesn't imply that the others are needed.
In rare cases, classes that have members of strange types (such as reference members) are an exception because they have peculiar copy semantics.
In a class holding a reference, you likely need to write the copy constructor and the assignment operator, but the default destructor already does the right thing. (Note that using a reference member is almost always wrong.)

**References**: C++CS Item 52; [Cline99] Â§30.01-14 Å¸ [Koenig97] Â§4 Å¸ [Stroustrup00] Â§5.5, Â§10.4 Å¸ [SuttHysl04b]



Resource management rule summary:

* [Provide strong resource safety; that is, never leak anything that you think of as a resource](#Cr safety)
* [Never throw while holding a resource not owned by a handle](#Cr never)
* [A "raw" pointer or reference is never a resource handle](#Cr raw)
* [Never let a pointer outlive the object it points to](#Cr outlive)
* [Use templates to express containers (and other resource handles)](#Cr templates)
* [Return containers by value (relying on move for efficiency)](#Cr value return)
* [If a class is a resource handle, it needs a constructor, a destructor, and copy and/or move operations](#Cr handle)
* [If a class is a container, give it an initializer-list constructor](#Cr list)



### <a name="Cr safety">Provide strong resource safety; that is, never leak anything that you think of as a resource</a>

**Reason**: Prevent leaks. Leaks can lead to performance degradation, mysterious error, system crashes, and security violations.

**Alternative formulation**: Have every resource represented as an object of some class managing its lifetime.

**Example**:

	template<class T>
	class Vector {
	// ...
	private:
		T* elem;	// sz elements on the free store, owned by the class object
		int sz;
	};

This class is a resource handle. It manages the lifetime of the `T`s. To do so, `Vector` must define or delete <a ref="???">the set of special operations</a> (constructors, a destructor, etc.).

**Example**:

	??? "odd" non-memory resource ???

**Enforcement**: The basic technique for preventing leaks is to have every resource owned by a resource handle with a suitable destructor. A checker can find "naked `new`s". Given a list of C-style allocation functions (e.g., `fopen()`), a checker can also find uses that are not managed by a resource handle. In general, "naked pointers" can be viewed with suspicion, flagged, and/or analyzed. A a complete list of resources cannot be generated without human input (the definition of "a resource" is necessarily too general), but a tool can be "parameterized" with a resource list.


### <a name="Cr never">Never throw while holding a resource not owned by a handle</a>

**Reason**: That would be a leak.

**Example**:
	
	void f(int i)
	{
		FILE* f = fopen("a file","r");
		ifstream is { "another file" };
		// ...
		if (i==0) return;
		// ...
		fclose(f);
	}

If `i==0` the file handle for `a file` is leaked. On the other hand, the `ifstream` for `another file` will correctly close its file (upon destruction). If you must use an explicit pointer, rather than a resource handle with specific semantics, use a `unique_ptr` or a `shared_ptr`:

void f(int i)
	{
		unique_ptr<FILE> f = fopen("a file","r");
		// ...
		if (i==0) return;
		// ...
	}

The code is simpler as well as correct.

**Enforcement**: A checker must consider all "naked pointers" suspicious.
A checker probably must rely on a human-provided list of resources.
For starters, we know about the standard-library containers, `string`, and smart pointers.
The use of `array_view` and `string_view` should help a lot (they are not resource handles).


### <a name="Cr raw">A "raw" pointer or reference is never a resource handle</a>

**Reason** To be able to distinguish owners from views.

**Note**: This is independent of how you "spell" pointer: `T*`, `T&`, `Ptr<T>` and `Range<T>` are not owners.


### <a name="Cr outlive">Never let a pointer outlive the object it points to</a>

**Reason**: To avoid extremely hard-to-find errors. Dereferencing such a pointer is undefined behavior and could lead to violations of the type system.

**Example**:

	string* bad()	// really bad
	{
		vector<string> v = { "this", "will", "cause" "trouble" };
		return &v[0];	// leaking a pointer into a destroyed member of a destroyed object (v)
	}

	void use()
	{
		string* p = bad();
		vector<int> xx = {7,8,9};
		string x = *p;		// undefined behavior: x may not be 1
		*p = "Evil!";		// undefined behavior: we don't know what (if anytihng) is allocated a location p
	}

The `string`s of `v` are destroyed upon exit from `bad()` and so is `v` itself. This the returned pointer points to unallocated memory on the free store. This memory (pointed into by `p`) may have been reallocated by the time `*p` is executed. There may be no `string` to read and a write through `p` could easily corrupt objects of unrelated types.

**Enforcement**: Most compilers already warn about simple cases and has the information to do more. Consider any pointer returned from a function suspect. Use containers, resource handles, and views (e.g., `array_view` known not to be resource handles) to lower the number of cases to be examined. For starters, consider every class with a destructor a resource handle.


### <a name="Cr templates">Use templates to express containers (and other resource handles)</a>

**Reason**: To provide statically type-safe manipulation of elements.

**Example**:

	template<typename T> class Vvector {
		// ...
		T* elem;	// point to sz elements of type T
		int sz;
	};

### <a name="Cr value return">Return containers by value (relying on move for efficiency)</a>

**Reason**: To simplify code and eliminate a need for explicit memory management. To bring an object into a surrounding scope, thereby extending its lifetime.

**Example**:

	vector

**Example**:

	factory

**Enforcement**: Check for pointers and references returned from functions and see if they are assigned to resource handles (e.g., to a `unique_ptr`).


### <a name="Cr handle">If a class is a resource handle, it needs a constructor, a destructor, and copy and/or move operations</a>

**Reason**: To provide complete control of the lifetime of the resource. To provide a coherent set of operations on the resource.

**Example**:

	messing with pointers

**Note**: If all members are resource handles, rely on the default special operations where possible.

	template<typename T> struct Named {
		string name;
		T value;
	};

Now `Named` has a default constructor, a destructor, and efficient copy and move operations, provided `T` has.

**Enforcement**: In general, a tool cannot know if a class is a resource handle. However, if a class has some of [the default operations](???), it should have all, and if a class has a member that is a resource handle, it should be considered a resource handle.


### <a name="Cr list">If a class is a container, give it an initializer-list constructor</a>

**Reason**: It is common to need an initial set of elements.

**Example**:

	template<typename T> class Vector {
	public:
		Vector<std::initializer_list<T>);
		// ...
	};

	Vector<string> vs = { "Nygaard", "Ritchie" };

**Enforcement**: When is a class a container?


<a name="S-unclassified"></a>
# To-do: Unclassified proto-rules

This is our to-do list.
Eventually, the entries will become rules or parts of rules.
Aternatively, we will decide that no change is needed and delete the entry.

* No long-distance friendship
* Should physical design (what's in a file) and large-scale design (libraries, groups of libraries) be addressed?
* Namespaces
* How granular should namespaces be? All classes/functions designed to work together and released together (as defined in Sutter/Alexandrescu) or something narrower or wider?
* Should there be inline namespaces (a-la `std::literals::*_literals`)?
* Avoid implicit conversions
* Const member functions should be thread safe "¦ aka, but I don't really change the variable, just assign it a value the first time its called "¦ argh
* Always initialize variables, use initialization lists for member variables.
* Anyone writing a public interface which takes or returns void* should have their toes set on fire. Â Â That one has been a personal favourite of mine for a number of years. :)
* Use `const`'ness wherever possible: member functions, variables and (yippee) const_iterators
* Use `auto`
* `(size)` vs. `{initializers}` vs. `{Extent{size}}`
* Don't overabstract
* Never pass a pointer down the call stack
* falling through a function bottom
* Should there be guidelines to choose between polymorphisms? YES. classic (virtual functions, reference semantics) vs. Sean Parent style (value semantics, type-erased, kind of like std::function)  vs. CRTP/static? YES Perhaps even vs. tag dispatch?
* Speaking of virtual functions, should non-virtual interface be promoted? NO. (public non-virtual foo() calling private/protected do_foo())? Not a new thing, seeing as locales/streams use it, but it seems to be under-emphasized.
* should virtual calls be banned from ctors/dtors in your guidelines? YES. A lot of people ban them, even though I think it's a big strength of C++ that they are ??? -preserving (D disappointed me so much when it went the Java way). WHAT WOULD BE A GOOD EXAMPLE?
* Speaking of lambdas, what would weigh in on the decision between lambdas and (local?) classes in algorithm calls and other callback scenarios?
* And speaking of std::bind, Stephen T. Lavavej criticizes it so much I'm starting to wonder if it is indeed going to fade away in future. Should lambdas be recommended instead?
* What to do with leaks out of temporaries? : `p = (s1+s2).c_str();`
* pointer/iterator invalidation leading to dangling pointers

	void bad()
	{
		int* p = new int[700];
		    int* q = &p[7];
    	delete p;

    	vector<int> v(700);
    	int* q2 = &v[7];
    	v.resize(900);

  	  // ... use q and q2 ...
	}
	
* LSP
* private inheritance vs/and membership
* avoid static class members variables (race conditions, almost-global variables)

* Use RAII lock guards (`lock_guard`, `unique_lock`, `shared_lock`), never call `mutex.lock` and `mutex.unlock` directly (RAII)
* Prefer non-recursive locks (often used to work around bad reasoning, overhead)
* Join your threads! (because of `std::terminate` in destructor if not joined or detached... is there a good reason to detach threads?) -- ??? could support library provide a RAII wrapper for `std::thread`?
* If two or more mutexes must be acquired at the same time, use `std::lock` (or another deadlock avoidance algorithm?)
* When using a `condition_variable`, always protect the condition by a mutex (atomic bool whose value is set outside of the mutex is wrong!), and use the same mutex for the condition variable itself.
* Never use `atomic_compare_exchange_strong` with `std::atomic<user-defined-struct>` (differences in padding matter, while `compare_exchange_weak` in a loop converges to stable padding)
* individual `shared_future` objects are not thread-safe: two threads cannot wait on the same `shared_future` object (they can wait on copies of a `shared_future` that refer to the same shared state)
* individual `shared_ptr` objects are not thread-safe: a thread cannot call a non-const member function of `shared_ptr` while another thread accesses (but different threads can call non-const member functions on copies of a `shared_ptr` that refer to the same shared object)

* rules for arithmetic
