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

#ifndef STRUCTURALIZE_TUPLE_LIKE_HPP
#define STRUCTURALIZE_TUPLE_LIKE_HPP

#include "structural/structuralization/structuralizer.hpp"
#include "structural/tuple.hpp"

#include <tuple>

namespace structural
{
// Specifically force std::tuple to be converted to structural::tuple, since otherwise std::tuple<> stays the same (it
// happens to be structural in some implementations), while any other std::tuple instantiation gets converted to
// structural::tuple. That would be weird.
template<typename... Ts, wrapper WrappedValue>
struct structuralizer<std::tuple<Ts...>, WrappedValue>
{
    static consteval auto do_structuralize()
    {
        return []<std::size_t... Is>(std::index_sequence<Is...>)
        {
            return structural::tuple{get<Is>(WrappedValue.unwrap())...};
        }(std::make_index_sequence<sizeof...(Ts)>{});
    }
};

template<tuple_like T, wrapper WrappedValue>
    requires(!structural_type<T>)
struct structuralizer<T, WrappedValue>
{
    static consteval auto do_structuralize()
    {
        return []<std::size_t... Is>(std::index_sequence<Is...>)
        {
            return structural::tuple{get<Is>(WrappedValue.unwrap())...};
        }(std::make_index_sequence<std::tuple_size_v<T>>{});
    }
};
} // namespace structural

#endif // STRUCTURALIZE_TUPLE_LIKE_HPP
