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

#ifndef STRUCTURAL_SERIALIZE_HELPER_HPP
#define STRUCTURAL_SERIALIZE_HELPER_HPP

#include "structural/serialization.hpp"

#include <vector>

#include <cassert>
#include <cstdint>

template<typename T>
constexpr auto serialize(T const& value) -> std::vector<std::byte>
{
    std::vector<std::byte> buffer;

    auto in_iter = std::back_inserter(buffer);
    structural::serialize(value, in_iter);

    return buffer;
}

template<typename T>
constexpr auto deserialize(std::vector<std::byte> const& value) -> T
{
    auto out_iter = value.begin();
    auto result   = structural::deserialize(std::in_place_type<T>, out_iter);
    assert(out_iter == value.end());
    return result;
}

#endif // STRUCTURAL_SERIALIZE_HELPER_HPP
