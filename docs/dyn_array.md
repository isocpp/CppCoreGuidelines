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

### Container Named Requirements
In order to allow element access using iterators and to align with various container
idioms for `std::` algorithms, `gsl::dyn_array` should satisfy the following named
requirements:

* Container ([link](https://en.cppreference.com/w/cpp/named_req/Container))
* ReversibleContainer ([link](https://en.cppreference.com/w/cpp/named_req/ReversibleContainer))
* ContiguousContainer ([link](https://en.cppreference.com/w/cpp/named_req/ContiguousContainer))
* SequenceContainer ([link](https://en.cppreference.com/w/cpp/named_req/SequenceContainer))
* AllocatorAwareContainer ([link](https://en.cppreference.com/w/cpp/named_req/AllocatorAwareContainer))

### Construction
In addition to the constructors required by the named requirements (default, copy, and
move), `gsl::dyn_array` will support the following constructors:

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
    requires (std::input_iterator<InputIt>)
constexpr dyn_array(InputIt first, InputIt last, const Allocator & alloc = Allocator());
```

* Construct a `dyn_array` with elements from the initializer list:
```c++
constexpr dyn_array(std::initializer_list<T>, const Allocator & alloc = Allocator());
```

* Construct a `dyn_array` with elements from the range `R`:
```c++
template <typename R>
    requires (std::input_range<R>)
constexpr dyn_array(R&&, const Allocator & alloc = Allocator());
```

### Operations
In addition to the operations required by the named requirements, `gsl::dyn_array` will
support the following operations:

* Access the specified element **_with bounds checking_**:
```c++
constexpr T& operator[](size_t);
constexpr const T& operator[](size_t) const
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

#### Note: Why no push_back (and friends)?
`gsl::dyn_array` is intended to be a fixed-size array and all objects should be
constructed at creation. Moreover, the memory overhead of storing another member
variable to track where to push the next item is not desired.

### Bounds Checking Semantics
If an out-of-bounds access (read or write) is attempted, `gsl::dyn_array` should follow
the contract violation strategy outlined in [GSL.assert: Assertions](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#gslassert-assertions).

### References
* [C++ Named Requirements](https://en.cppreference.com/w/cpp/named_req)
* [Microsoft/GSL #1169](https://github.com/microsoft/GSL/issues/1169)
* [isocpp/CppCoreGuidelines #2244](https://github.com/isocpp/CppCoreGuidelines/issues/2244)
* [n3662](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3662.html)
