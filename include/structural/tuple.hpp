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

#include <tuple>
#include <type_traits>

namespace structural
{
template<typename... Ts>
using tuple = detail::tuple_impl<std::make_index_sequence<sizeof...(Ts)>, Ts...>;

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
constexpr auto make_tuple(Types&&... args) -> tuple<std::unwrap_ref_decay_t<Types>...>
{
    return tuple<std::unwrap_ref_decay_t<Types>...>(std::forward<Types>(args)...);
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
