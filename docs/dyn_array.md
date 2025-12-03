<!-- $ pandoc -V geometry:margin=1in -V colorlinks=true -o dyn_array.pdf dyn_array.md --> 

# `gsl::dyn_array<T, Allocator>`

`gsl::dyn_array` is a dynamic array that is intended to be a replacement for slinging
around raw pointers and sizes. Notably, it _owns_ all of its elements. It should replace
the following idioms:

* Replace `new T[n]` with `gsl::dyn_array<T>(n)`.
* Replace both `foo(T*, size_t)` and `foo(unique_ptr<T[]>&, size_t)` with
`foo(gsl::dyn_array<T>&)`.

It can be thought of like a...

* `std::array` except the size is specified at runtime.
* `std::vector` except it can neither shrink nor grow.

By design, `gsl::dyn_array` is not a `Container` as defined by the C++ Named
Requirements because we want to avoid the invalidation of iterators or references to
`gsl::dyn_array` objects.  Furthermore, by design, there is no support for operations
(other than destruction) that can invalidate iterators or pointers into the sequence
owned by a `gsl::dyna_array` object. An `gsl::dyn_array` object cannot be moved,
nor can it be copied.

### Construction
`gsl::dyn_array`s can be constructed in the following ways:

* Default construct a `dyn_array` with no elements:
```c++
constexpr dyn_array();
```

* Move construct a `dyn_array` from `other`: not possible
```c++
dyn_array(dyn_array&& other) = delete;
```

* Construct a `dyn_array` with `n` default constructed elements:
```c++
constexpr explicit dyn_array(size_t n, const Allocator & alloc = Allocator());
```

* Construct a `dyn_array` with `n` elements, each copy constructed from `arg`:
```c++
constexpr dyn_array(size_t n, const T& arg, const Allocator & alloc = Allocator());
``` 

* Construct a `dyn_array` with elements from the range `[first, last)`:
```c++
template <typename InputIt>
#ifdef __cpp_lib_concepts
    requires(std::input_iterator<InputIt>)
#endif /* __cpp_lib_concepts */
constexpr dyn_array(InputIt first, InputIt last, const Allocator & alloc = Allocator());
```

* Construct a `dyn_array` from a range:
```c++
#ifdef __cpp_lib_containers_range
template <typename R>
    requires(std::ranges::range<R>)
constexpr dyn_array(std::from_range_t, R&& r, const Allocator & alloc = Allocator());
#endif /* __cpp_lib_containers_range */
```

* Construct a `dyn_array` with elements from the initializer list:
```c++
constexpr dyn_array(std::initializer_list<T>, const Allocator & alloc = Allocator());
```

### Operations
In addition to the operations required by the named requirements, `gsl::dyn_array` will
support the following operations:

* Access the specified element **_with bounds checking_**:
```c++
constexpr T& operator[](size_t);
constexpr const T& operator[](size_t) const;
```

* Access the underlying array:
```c++
constexpr T* data() noexcept;
constexpr const T* data() const noexcept;
```
 
* Return the number of elements in the `dyn_array`:
```c++
constexpr size_t size() const noexcept;
```

### FAQ

#### Why no push_back (and friends)?
`gsl::dyn_array` is intended to be a fixed-size array and all objects should be
constructed at creation.  It supports no iterator/pointer-invalidating operation.

#### Why does `gsl::dyn_array` not conform to the `Container` Named Requirements?
`gsl::dyn_array` is intended to be a safer replacement for raw pointers and sizes. We
don't want users to accidentally use it in a way that would be unsafe. For example,
`gsl::dyn_array` does not have copy or move operations. This is because it
would be possible to invalidate existing iterators and references.

### Bounds Checking Semantics
If an out-of-bounds access (read or write) is attempted, `gsl::dyn_array` should follow
the contract violation strategy outlined in [GSL.assert: Assertions](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#gslassert-assertions).

### References
* [C++ Named Requirements](https://en.cppreference.com/w/cpp/named_req)
* [Microsoft/GSL #1169](https://github.com/microsoft/GSL/issues/1169)
* [isocpp/CppCoreGuidelines #2244](https://github.com/isocpp/CppCoreGuidelines/issues/2244)
* [n3662](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3662.html)
