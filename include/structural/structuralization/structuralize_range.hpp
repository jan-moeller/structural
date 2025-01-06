//
// MIT License
//
// Copyright (c) 2025 Jan Möller
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

#ifndef STRUCTURALIZE_RANGE_HPP
#define STRUCTURALIZE_RANGE_HPP

#include "structural/structuralization/structuralizer.hpp"
#include "structural/tuple.hpp"

#include <ranges>

namespace structural
{
namespace detail
{
template<typename Head, typename... Tail>
constexpr auto homogenous(tuple<Head, Tail...> const&) -> bool
{
    return (std::is_same_v<Head, Tail> && ...);
}

constexpr auto homogenous(tuple<> const&) -> bool
{
    return true;
}
} // namespace detail

template<std::ranges::forward_range T, wrapper WrappedValue>
    requires(!structural_type<T> && !tuple_like<T> && !std::ranges::view<T>)
struct structuralizer<T, WrappedValue>
{
    static consteval auto do_structuralize()
    {
        using value_type                  = std::ranges::range_value_t<T>;
        constexpr std::size_t size        = std::ranges::distance(WrappedValue.unwrap());
        constexpr auto        tuple_value = []<std::size_t... Is>(std::index_sequence<Is...>)
        {
            return structural::tuple(STRUCTURALIZE(WrappedValue.unwrap()[Is])...);
        }(std::make_index_sequence<size>{});

        return [&tuple_value]<std::size_t... Is>(std::index_sequence<Is...>)
        {
            if constexpr (homogenous(tuple_value))
                return std::array<value_type, size>{get<Is>(tuple_value)...};
            else
                return tuple_value;
        }(std::make_index_sequence<size>{});
    }
};
} // namespace structural

#endif // STRUCTURALIZE_RANGE_HPP
