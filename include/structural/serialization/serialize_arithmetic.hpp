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

#ifndef STRUCTURAL_SERIALIZE_ARITHMETIC_HPP
#define STRUCTURAL_SERIALIZE_ARITHMETIC_HPP

#include "structural/serialization/serializer.hpp"

#include <bit>
#include <iterator>
#include <type_traits>
#include <utility>

#include <cstddef>

namespace structural
{
template<typename T>
    requires std::is_arithmetic_v<T>
struct serializer<T>
{
    static constexpr void do_serialize(T value, std::output_iterator<std::byte> auto& out_iter)
    {
        auto const a = std::bit_cast<std::array<std::byte, sizeof(T)>>(value);
        for (auto b : a)
            *out_iter++ = b;
    }

    static constexpr auto do_deserialize(std::in_place_type_t<T>, std::input_iterator auto& in_iter) -> T
    {
        std::array<std::byte, sizeof(T)> a;
        for (auto& b : a)
            b = *in_iter++;
        return std::bit_cast<T>(a);
    }
};
} // namespace structural

#endif // STRUCTURAL_SERIALIZE_ARITHMETIC_HPP
