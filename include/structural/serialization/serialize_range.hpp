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

#ifndef STRUCTURAL_SERIALIZE_RANGE_HPP
#define STRUCTURAL_SERIALIZE_RANGE_HPP

#include "structural/serialization/serialize_tuple_like.hpp"

#include <iterator>
#include <ranges>
#include <utility>
#include <vector>

#include <cstddef>

namespace structural
{
template<std::ranges::forward_range T>
    requires(!tuple_like<T> && !std::ranges::view<T>)
struct serializer<T>
{
    static constexpr void do_serialize(T const& range, std::output_iterator<std::byte> auto& out_iter)
    {
        std::size_t const size = std::ranges::distance(range);
        serialize(size, out_iter);
        for (auto&& e : range)
            serialize(e, out_iter);
    }

    static constexpr auto do_deserialize(std::in_place_type_t<T>, std::input_iterator auto& in_iter) -> T
    {
        std::size_t const size = deserialize(std::in_place_type<std::size_t>, in_iter);
        using value_type       = std::ranges::range_value_t<T>;

        // Temporarily deserialize into a vector, then create the correct type
        std::vector<value_type> temp;
        temp.reserve(size);
        for (std::size_t i = 0; i < size; ++i)
            temp.push_back(deserialize(std::in_place_type<value_type>, in_iter));

        return T{temp.begin(), temp.end()};
    }
};
} // namespace structural

#endif // STRUCTURAL_SERIALIZE_RANGE_HPP
