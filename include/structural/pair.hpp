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

#ifndef STRUCTURAL_PAIR_HPP
#define STRUCTURAL_PAIR_HPP

#include "structural/tuple.hpp"

#include <tuple>
#include <type_traits>

namespace structural
{
namespace detail::pair
{
struct internal_tag
{
};
} // namespace detail::pair

template<typename First, typename Second>
struct pair
{
    First  first;
    Second second;

    constexpr pair() = default;
    constexpr pair(First const& x, Second const& y)
        : first(x)
        , second(y)
    {
    }
    template<typename T1 = First, typename T2 = Second>
        requires std::is_constructible_v<First, T1> && std::is_constructible_v<Second, T2>
    constexpr pair(T1&& x, T2&& y)
        : first(std::forward<T1>(x))
        , second(std::forward<T2>(y))
    {
    }
    template<typename T1 = First, typename T2 = Second>
        requires std::is_constructible_v<First, T1 const&> && std::is_constructible_v<Second, T2 const&>
    constexpr explicit(!std::is_convertible_v<T1 const&, First> || !std::is_convertible_v<T2 const&, Second>)
        pair(pair<T1, T2> const& p)
        : first(p.first)
        , second(p.second)
    {
    }
    template<typename T1 = First, typename T2 = Second>
        requires std::is_constructible_v<First, T1 const&> && std::is_constructible_v<Second, T2 const&>
    constexpr explicit(!std::is_convertible_v<T1 const&, First> || !std::is_convertible_v<T2 const&, Second>)
        pair(pair<T1, T2>&& p)
        : first(std::forward<T1>(p.first))
        , second(std::forward<T2>(p.second))
    {
    }
    template<class... Args1, class... Args2>
    constexpr pair(std::piecewise_construct_t, tuple<Args1...>&& first_args, tuple<Args2...>&& second_args)
        : pair(detail::pair::internal_tag{},
               std::move(first_args),
               std::move(second_args),
               std::make_index_sequence<sizeof...(Args1)>(),
               std::make_index_sequence<sizeof...(Args2)>())
    {
    }
    template<class Tuple1, class Tuple2, std::size_t... Is, std::size_t... Js>
    constexpr pair(detail::pair::internal_tag,
                   Tuple1&& tuple1,
                   Tuple2&& tuple2,
                   std::index_sequence<Is...>,
                   std::index_sequence<Js...>)
        : first(get<Is>(std::forward<Tuple1>(tuple1))...)
        , second(get<Js>(std::forward<Tuple2>(tuple2))...)
    {
    }

    friend constexpr auto operator==(pair const& lhs, pair const& rhs) -> bool = default;

    friend constexpr auto operator<=>(pair const& lhs, pair const& rhs)
    {
        using result_t  = std::common_comparison_category_t<decltype(detail::synth_three_way(std::declval<First>(),
                                                                                            std::declval<Second>()))>;
        result_t result = result_t::equivalent;
        detail::static_for<0, 2>(
            [&](auto I)
            {
                auto const r = detail::synth_three_way(get<I>(lhs), get<I>(rhs));
                result       = r;
                if (r != 0)
                    return detail::static_for_continuation::break_;
                return detail::static_for_continuation::continue_;
            });

        return result;
    }
};

template<std::size_t I, class First, class Second>
constexpr auto get(pair<First, Second>& t) noexcept -> std::tuple_element_t<I, pair<First, Second>>&
{
    if constexpr (I == 0)
        return t.first;
    else
        return t.second;
}

template<std::size_t I, class First, class Second>
constexpr auto get(pair<First, Second>&& t) noexcept -> std::tuple_element_t<I, pair<First, Second>>&&
{
    if constexpr (I == 0)
        return std::move(t.first);
    else
        return std::move(t.second);
}

template<std::size_t I, class First, class Second>
constexpr auto get(pair<First, Second> const& t) noexcept -> std::tuple_element_t<I, pair<First, Second>> const&
{
    if constexpr (I == 0)
        return t.first;
    else
        return t.second;
}

template<std::size_t I, class First, class Second>
constexpr auto get(pair<First, Second> const&& t) noexcept -> std::tuple_element_t<I, pair<First, Second>> const&&
{
    if constexpr (I == 0)
        return std::move(t.first);
    else
        return std::move(t.second);
}

template<class T, class First, class Second>
constexpr auto get(pair<First, Second>& t) noexcept -> T&
{
    if constexpr (std::same_as<T, First>)
        return t.first;
    else
        return t.second;
}

template<class T, class First, class Second>
constexpr auto get(pair<First, Second>&& t) noexcept -> T&&
{
    if constexpr (std::same_as<T, First>)
        return std::move(t.first);
    else
        return std::move(t.second);
}

template<class T, class First, class Second>
constexpr auto get(pair<First, Second> const& t) noexcept -> T const&
{
    if constexpr (std::same_as<T, First>)
        return t.first;
    else
        return t.second;
}

template<class T, class First, class Second>
constexpr auto get(pair<First, Second> const&& t) noexcept -> T const&&
{
    if constexpr (std::same_as<T, First>)
        return std::move(t.first);
    else
        return std::move(t.second);
}

template<class First, class Second>
constexpr auto make_pair(First&& first, Second&& second)
    -> pair<std::unwrap_ref_decay_t<First>, std::unwrap_ref_decay_t<Second>>
{
    return pair<std::unwrap_ref_decay_t<First>, std::unwrap_ref_decay_t<Second>>(std::forward<First>(first),
                                                                                 std::forward<Second>(second));
}
} // namespace structural

namespace std
{
template<class First, class Second>
struct tuple_size<structural::pair<First, Second>> : std::integral_constant<std::size_t, 2>
{
};

template<std::size_t I, class First, class Second>
struct tuple_element<I, structural::pair<First, Second>>
{
    using type = std::conditional_t<I == 0, First, Second>;
};
} // namespace std

#endif // STRUCTURAL_PAIR_HPP
