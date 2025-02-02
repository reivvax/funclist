# Funclist
Some basic functional programming.

Implemented a functional list representation using callable objects, where a list `l` of type `L` supports operations via the method:
```cpp
template <typename F, typename A>  
A operator()(F f, A a);
```
with `f` being a callable of the form:
```cpp
template <typename X>  
A operator()(X x, A a);
```
Lists implicitly store elements $[x_0, ..., x_{n-1}]$ and operate as:
$$l(f, a) = f(x_0, f(x_1, ..., f(x_{n-1}, a)...))$$

Implemented operations:
- `empty`: represents an empty list.
- `size(l)`: returns the size of list l.
- `cons(x, l)`: prepends x to list l.
- `create(...)`: constructs a list from given elements.
- `of_range(r)`: creates a list from a bidirectional range `r`.
- `concat(l, k)`: concatenates lists `l` and `k`.
- `rev(l)`: reverses the list `l`.
- `map(m, l)`: applies `m(x)` to each element `x` in `l`.
- `filter(p, l)`: keeps elements `x` in `l` where `p(x)` is true.
- `flatten(l)`: concatenates nested lists in `l`.
- `as_string(l)`: converts `l` to `std::string`, assuming `operator<<` is defined for elements.
