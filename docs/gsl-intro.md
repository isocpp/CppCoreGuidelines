
# Using the Guidelines Support Library (GSL): A Tutorial and FAQ

by Herb Sutter

updated 2018-01-08


## Overview: "Is this document a tutorial or a FAQ?"

It aims to be both:

- a tutorial you can read in order, following a similar style as the introduction of [K&R](https://en.wikipedia.org/wiki/The_C_Programming_Language) by building up examples of increasing complexity; and

- a FAQ you can use as a reference, with each section showing the answer to a specific question.


## Motivation: "Why would I use GSL, and where can I get it?"

First look at the [C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines); this is a support library for that document. Select a set of guidelines you want to adopt, then bring in the GSL as directed by those guidelines.

You can try out the examples in this document on all major compilers and platforms using [this GSL reference implementation](https://github.com/microsoft/gsl).


# gsl::span: "What is gsl::span, and what is it for?"

`gsl::span` is a replacement for `(pointer, length)` pairs to refer to a sequence of contiguous objects. It can be thought of as a pointer to an array, but that knows its bounds.

For example, a `span<int,7>` refers to a sequence of seven contiguous integers.

A `span` does not own the elements it points to. It is not a container like an `array` or a `vector`, it is a view into the contents of such a container.


## span parameters: "How should I choose between span and traditional (ptr, length) parameters?"

In new code, prefer the bounds-checkable `span<T>` instead of separate pointer and length parameters. In older code, adopt `span` where reasonable as you maintain the code.

A function that takes a pointer to an array and a separate length, such as:

~~~cpp
// Error-prone: Process n contiguous ints starting at *p
void dangerous_process_ints(const int* p, size_t n);
~~~

is error-prone and difficult to use correctly:

~~~cpp
int a[100];
dangerous_process_ints(a, 1000); // oops: buffer overflow

vector<int> v(200);
dangerous_process_ints(v.data(), 1000); // oops: buffer overflow

auto remainder = find(v.begin(), v.end(), some_value);
    // now call dangerous_process_ints() to fill the rest of the container from *remainder to the end
dangerous_process_ints(&*remainder, v.end() - remainder); // correct but convoluted
~~~

Instead, using `span` encapsulates the pointer and the length:

~~~cpp
// BETTER: Read s.size() contiguous ints starting at s[0]
void process_ints(span<const int> s);
~~~

which makes `process_ints` easier to use correctly because it conveniently deduces from common types:

~~~cpp
int a[100];
process_ints(a); // deduces correct length: 100 (constructs the span from a container)

vector<int> v(200);
process_ints(v); // deduces correct length: 200 (constructs the span from a container)
~~~

and conveniently supports modern C++ argument initialization when the calling code does have distinct pointer and length arguments:

~~~cpp
auto remainder = find(v.begin(), v.end(), some_value);
    // now call process_ints() to fill the rest of the container from *remainder to the end
process_ints({remainder, v.end()}); // correct and clear (constructs the span from an iterator pair)
~~~

> Things to remember
> - Prefer `span` instead of (pointer, length) pairs.
> - Pass a `span` like a pointer (i.e., by value for "in" parameters). Treat it like a pointer range.


## span and const: "What's the difference between `span<const T>` and `const span<T>`?"

`span<const T>` means that the `T` objects are read-only. Prefer this by default, especially as a parameter, if you don't need to modify the `T`s.

`const span<T>` means that the `span` itself can't be made to point at a different target.

`const span<const T>` means both.

> Things to remember
> - Prefer a `span<const T>` by default to denote that the contents are read-only, unless you do need read-write access.


## Iteration: "How do I iterate over a span?"

A `span` is an encapsulated range, and so can be visited using a range-based `for` loop.

Consider the implementation of a function like the `process_ints` that we saw in an earlier example. Visiting every object using a (pointer, length) pair requires an explicit index:

~~~cpp
void dangerous_process_ints(int* p, size_t n) {
    for (auto i = 0; i < n; ++i) {
        p[i] = next_character();
    }
}
~~~

A `span` supports range-`for` -- note this is zero-overhead and does not need to perform any range check, because the range-`for` loop is is known by construction not to exceed the range's bounds:

~~~cpp
void process_ints(span<int> s) {
    for (auto& c : s) {
        c = next_character();
    }
}
~~~

A `span` also supports normal iteration using `.begin()` and `.end()`.

Note that you cannot compare iterators from different spans, even if they refer to the same array.

An iterator is valid as long as the `span` that it is iterating over exists.


## Element access: "How do I access a single element in a span?"

Use `myspan[offset]` to subscript, or equivalently use `iter + offset` wheren `iter` is a `span<T>::iterator`. Both are range-checked.



## Sub-spans: "What if I need a subrange of a span?"

To refer to a sub-span, use `first`, `last`, or `subspan`.

~~~cpp
void process_ints(span<widget> s) {
    if (s.length() > 10) {
        read_header(s.first(10));   // first 10 entries
        read_rest(s.subspan(10));   // remaining entries
        // ...
    }
}
~~~

In rarer cases, when you know the number of elements at compile time and want to enable `constexpr` use of `span`, you can pass the length of the sub-span as a template argument:

~~~cpp
constexpr int process_ints(span<widget> s) {
    if (s.length() > 10) {
        read_header(s.first<10>());   // first 10 entries
        read_rest(s.subspan<10>());   // remaining entries
        // ...
    }
    return s.size();
}
~~~


## span and STL: "How do I pass a span to an STL-style [begin,end) function?"

Use `span::iterator`s. A `span` is iterable like any STL range.

To call an STL `[begin,end)`-style interface, use `begin` and `end` by default, or other valid iterators if you don't want to pass the whole range:

~~~cpp
void f(span<widget> s) {
    // ... 
    auto found = find_if(s.begin(), s.end(), some_value);
    // ... 
}
~~~

If you are using a range-based algorithm such as from [Range-V3](https://github.com/ericniebler/range-v3), you can use a `span` as a range directly:

~~~cpp
void f(span<widget> s) {
    // ... 
    auto found = find_if(s, some_value); 
    // ... 
}
~~~


## Comparison: "When I compare `span<T>`s, do I compare the `T` values or the underlying pointers?"

Comparing two `span<T>`s compares the `T` values. To compare two spans for identity, to see if they're pointing to the same thing, use `.data()`.

~~~cpp
int a[] = { 1, 2, 3};
span<int> sa{a};

vector<int> v = { 1, 2, 3 };
span<int> sv{v};

assert(sa == sv); // sa and sv both point to contiguous ints with values 1, 2, 3
assert(sa.data() != sv.data()); // but sa and sv point to different memory areas
~~~  

> Things to remember
> - Comparing spans compares their contents, not whether they point to the same location.


## Empty vs null: "Do I have to explicitly check whether a span is null?"

Usually not, because the thing you usually want to check for is that the `span` is not empty, which means its size is not zero. It's safe to test the size of a span even if it's null. 

Remember that the following all have identical meaning for a `span s`:

- `!s.empty()`
- `s.size() != 0`
- `s.data() != nullptr && s.size() != 0` (the first condition is actually redundant)

The following is also functionally equivalent as it just tests whether there are zero elements:

- `s != nullptr` (compares `s` against a null-constructed empty `span`)

For example:

~~~cpp
void f(span<const int> s) {
    if (s != nullptr && s.size() > 0) { // bad: redundant, overkill
        // ...
    }

    if (s.size() > 0) { // good: not redundant
        // ...
    }

    if (!s.empty()) { // good: same as "s.size() > 0"
        // ...
    }
}

~~~

> Things to remember
> - Usually you shouldn't check for a null `span`. For a `span s`, if you're comparing `s != nullptr` or `s.data() != nullptr`, check to make sure you shouldn't just be asking `!s.empty()`.


## as_bytes: "Why would I convert a span to `span<const byte>`?"

Because it's a type-safe way to get a read-only view of the objects' bytes.  

Without `span`, to view the bytes of an object requires writing a brittle cast:

~~~cpp
void serialize(char* p, int length); // bad: forgot const

void f(widget* p, int length) {
    // serialize one object's bytes (incl. padding)
    serialize(p, 1); // bad: copies just the first byte, forgot sizeof(widget)
}
~~~

With `span` the code is safer and cleaner:

~~~cpp
void serialize(span<const byte>); // can't forget const, the first test call site won't compile

void f(span<widget> s) {
    // ...
    // serialize one object's bytes (incl. padding)
    serialize(as_bytes(s)); // ok
}
~~~

Also, `span<T>` lets you distinguish between `.size()` and `.size_bytes()`; make use of that distinction instead of multiplying by `sizeof(T)`.

> Things to remember
> - Prefer `span<T>`'s `.size_bytes()` instead of `.size() * sizeof(T)`.


## And a few `span`-related hints

These are not directly related to `span` but can often come up while using `span`.

   * Use `byte` everywhere you are handling memory (as opposed to characters or integers). That is, when accessing a chunk of raw memory, use `gsl::span<std::byte>`.

   * Use `narrow()` when you cannot afford to be surprised by a value change during conversion to a smaller range. This includes going between a signed `span` size or index and an unsigned today's-STL-container `.size()`, though the `span` constructors from containers nicely encapsulate many of these conversions.

   * Similarly, use `narrow_cast()` when you are *sure* you won’t be surprised by a value change during conversion to a smaller range
