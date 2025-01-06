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

#ifndef STRUCTURAL_SERIALIZE_EXPECTED_HPP
#define STRUCTURAL_SERIALIZE_EXPECTED_HPP

#include <version>
#if __cpp_lib_expected >= 202202L

#include "structural/serialization/serializer.hpp"

#include <expected>

#include <iterator>
#include <utility>

#include <cstddef>

namespace structural
{
template<typename T, typename E>
struct serializer<std::expected<T, E>>
{
    static constexpr void do_serialize(std::expected<T, E> const& value, std::output_iterator<std::byte> auto& out_iter)
    {
        serialize(value.has_value(), out_iter);
        if (value.has_value())
            serialize(value.value(), out_iter);
        else
            serialize(value.error(), out_iter);
    }

    static constexpr auto do_deserialize(std::in_place_type_t<std::expected<T, E>>, std::input_iterator auto& in_iter)
        -> std::expected<T, E>
    {
        bool const has_value = deserialize(std::in_place_type<bool>, in_iter);
        if (has_value)
            return std::expected<T, E>(std::in_place, deserialize(std::in_place_type<T>, in_iter));
        return std::expected<T, E>(std::unexpect, deserialize(std::in_place_type<E>, in_iter));
    }
};
} // namespace structural

#endif

#endif // STRUCTURAL_SERIALIZE_EXPECTED_HPP
