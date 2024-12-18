//
// MIT License
//
// Copyright (c) 2022 Jan Möller
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef STRUCTURAL_TUPLE_HPP
#define STRUCTURAL_TUPLE_HPP

#include "structural/detail/tuple_impl.hpp"

#include <array>
#include <tuple>
#include <type_traits>

namespace structural
{
template<typename... Ts>
using tuple = detail::tuple_impl<std::make_index_sequence<sizeof...(Ts)>, Ts...>;

namespace detail
{
template<typename... Tuples>
struct tuple_cat_t;

template<typename... Ts, typename... Us, typename... Tuples>
struct tuple_cat_t<tuple<Ts...>, tuple<Us...>, Tuples...>
{
    using type = typename tuple_cat_t<tuple<Ts..., Us...>, Tuples...>::type;
};
template<typename... Ts, typename... Us>
struct tuple_cat_t<tuple<Ts...>, tuple<Us...>>
{
    using type = tuple<Ts..., Us...>;
};
template<typename... Ts>
struct tuple_cat_t<tuple<Ts...>>
{
    using type = tuple<Ts...>;
};

template<typename... Tuples>
inline constexpr auto tuple_sizes = std::array{std::tuple_size_v<std::remove_cvref_t<Tuples>>...};

template<typename... Tuples>
consteval auto tuple_cat_inner_indices()
{
    return []<std::size_t... is>(std::index_sequence<is...>) constexpr
    {
        std::size_t idx = 0;
        std::size_t sum = 0;
        auto        l   = [&]([[maybe_unused]] std::size_t)
        {
            if (tuple_sizes<Tuples...>[idx] == sum)
            {
                ++idx;
                sum = 0;
            }
            ++sum;
            return idx;
        };
        return std::array{l(is)...};
    }(std::make_index_sequence<(std::tuple_size_v<std::remove_cvref_t<Tuples>> + ...)>{});
}

template<typename... Tuples>
consteval auto tuple_cat_outer_indices()
{
    return []<std::size_t... is>(std::index_sequence<is...>) constexpr
    {
        std::size_t idx = 0;
        std::size_t sum = 0;
        auto        l   = [&]([[maybe_unused]] std::size_t)
        {
            if (tuple_sizes<Tuples...>[idx] == sum)
            {
                ++idx;
                sum = 0;
            }
            ++sum;
            return sum - 1;
        };
        return std::array{l(is)...};
    }(std::make_index_sequence<(std::tuple_size_v<std::remove_cvref_t<Tuples>> + ...)>{});
}

template<std::array Indices>
consteval auto tuple_cat_make_integral_constants()
{
    return []<std::size_t... is>(std::index_sequence<is...>) constexpr
    {
        return tuple{std::integral_constant<std::size_t, Indices[is]>{}...};
    }(std::make_index_sequence<Indices.size()>{});
}

} // namespace detail

template<std::size_t I, class... Types>
constexpr auto get(tuple<Types...>& t) noexcept -> std::tuple_element_t<I, tuple<Types...>>&
{
    return detail::get<I>(t);
}

template<std::size_t I, class... Types>
constexpr auto get(tuple<Types...>&& t) noexcept -> std::tuple_element_t<I, tuple<Types...>>&&
{
    return detail::get<I>(std::move(t));
}

template<std::size_t I, class... Types>
constexpr auto get(tuple<Types...> const& t) noexcept -> std::tuple_element_t<I, tuple<Types...>> const&
{
    return detail::get<I>(t);
}

template<std::size_t I, class... Types>
constexpr auto get(tuple<Types...> const&& t) noexcept -> std::tuple_element_t<I, tuple<Types...>> const&&
{
    return detail::get<I>(std::move(t));
}

template<class T, class... Types>
constexpr auto get(tuple<Types...>& t) noexcept -> T&
{
    return detail::get<T>(t);
}

template<class T, class... Types>
constexpr auto get(tuple<Types...>&& t) noexcept -> T&&
{
    return detail::get<T>(std::move(t));
}

template<class T, class... Types>
constexpr auto get(tuple<Types...> const& t) noexcept -> T const&
{
    return detail::get<T>(t);
}

template<class T, class... Types>
constexpr auto get(tuple<Types...> const&& t) noexcept -> T const&&
{
    return detail::get<T>(std::move(t));
}

template<typename... Types>
constexpr auto make_tuple(Types&&... args) -> decltype(auto)
{
    return tuple<std::unwrap_ref_decay_t<Types>...>(std::forward<Types>(args)...);
}

template<typename... Types>
constexpr auto forward_as_tuple(Types&&... args) noexcept -> decltype(auto)
{
    return tuple<Types&&...>(std::forward<Types>(args)...);
}

template<typename... Tuples>
constexpr auto tuple_cat(Tuples&&... args) -> typename detail::tuple_cat_t<std::remove_cvref_t<Tuples>...>::type
{
    using Ret = typename detail::tuple_cat_t<std::remove_cvref_t<Tuples>...>::type;

    constexpr auto inner_indices = detail::tuple_cat_inner_indices<Tuples...>();
    constexpr auto outer_indices = detail::tuple_cat_outer_indices<Tuples...>();

    return []<typename... Is, typename... Js, typename TT>(tuple<Is...>, tuple<Js...>, TT&& tuples) -> Ret
    {
        return Ret{structural::get<Js::value>(structural::get<Is::value>(std::forward<TT>(tuples)))...};
    }(detail::tuple_cat_make_integral_constants<inner_indices>(),
      detail::tuple_cat_make_integral_constants<outer_indices>(),
      structural::forward_as_tuple(std::forward<Tuples>(args)...));
}
} // namespace structural

namespace std
{
template<class... Types>
struct tuple_size<structural::tuple<Types...>> : std::integral_constant<std::size_t, sizeof...(Types)>
{
};

template<std::size_t I, typename... Types>
struct tuple_element<I, structural::tuple<Types...>> : structural::detail::tuple_element<I, structural::tuple<Types...>>
{
};
} // namespace std

#endif // STRUCTURAL_TUPLE_HPP
