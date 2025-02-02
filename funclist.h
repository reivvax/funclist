#ifndef FUNC
#define FUNC

#include <functional>
#include <string>
#include <sstream>
#include <iostream>
#include <ranges>

namespace detail {

    auto create();

    template<typename T, typename ...Args>
    auto create(T first, Args... args);

    template <typename T>
    constexpr auto maybe_unwrap(T t) {
        if constexpr (std::is_same_v<T, std::reference_wrapper<decltype(t)>>)
            return t.get();
        else
            return t;
    }

    template<typename It, typename F, typename A>
    A of_range(It current, It end, F f, A a) {
        if (current == end)
            return a;
        It tmp = current;
        ++current;
        return f(*tmp, of_range(current, end, f, a));
    }

} // namespace detail

namespace flist {

    const auto empty = [](auto _, auto a) {
        return a;
    };

    const auto cons = [](auto x, auto l) {
        return [=](auto f, auto a) {
            return f(x, l(f, a));
        };
    };

    auto create = []<typename ...Args>(Args... args) {
        return detail::create(args...);
    };

    const auto of_range = [](auto r) {
        return [=](auto f, auto a) {
            if constexpr (std::ranges::bidirectional_range<decltype(r)>)
                return detail::of_range(r.begin(), r.end(), f, a);
            else
                return detail::of_range(r.get().begin(), r.get().end(), f, a);
        };
    };

    const auto concat = [](auto l, auto k) {
        return [=](auto f, auto a) {
            return l(f, k(f, a));
        };
    };

    const auto rev = [](auto l) {
        return [=](auto f, auto a) {
            using A = decltype(a);
            using F = decltype(f);

            const auto push_back = [=](auto x, auto l) {
                return std::function<A(F, A)>([=](auto f, auto a) {
                    return l(f, f(x, a));
                });
            };

            std::function<A(F, A)> start = empty;
            return l(push_back, start)(f, a);
        };
    };

    const auto map = [](auto m, auto l) {
        return [=](auto f, auto a) {
            return l([=](auto x, auto _a) {
                return f(m(x), _a);
            }, a);
        };
    };

    const auto filter = [](auto p, auto l) {
        return [=](auto f, auto a) {
            return l([=](auto x, auto _a) {
                return p(x) ? f(x, _a) : _a;
            }, a);
        };
    };

    const auto flatten = [](auto l) {
        return [=](auto f, auto a) {
            return l([=](auto x, auto _a) {
                return x(f, _a);
            }, a);
        };
    };

    const auto as_string = [](const auto& l) -> std::string {
        std::ostringstream os;
        auto stream = [&os](auto x, auto a) {
            if (!a)
                os << ";"; 
            os << x;
            return false;
        };

        rev(std::ref(l))(stream, true);

        return "[" + os.str() + "]";
    };

} // namespace flist

namespace detail {

    auto create() {
        return flist::empty;
    }

    template<typename T, typename ...Args>
    auto create(T first, Args... args) {
        return flist::cons(first, create(args...));
    }

} // namespace detail

#endif