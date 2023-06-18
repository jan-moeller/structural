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

#ifndef STRUCTURAL_HASH_COMBINE_HPP
#define STRUCTURAL_HASH_COMBINE_HPP

#include <concepts>

#include <cstddef>

namespace structural::detail
{
template<std::same_as<std::size_t>... Ts>
constexpr auto hash_combine(std::size_t first, Ts... ts) noexcept -> std::size_t
{
    if constexpr (sizeof...(Ts) == 0)
        return first;
    else
    {
        std::size_t result = first ^ (hash_combine(ts...) + 0x9e3779b9 + (first << 6) + (first >> 2));
        return result;
    }
}

constexpr auto hash_bytes(std::byte const* ptr, std::size_t n) noexcept -> std::size_t
{
    std::size_t result = 0;
    unsigned    shift  = 0;
    for (; shift < sizeof(std::size_t) && shift < n; ++shift)
        result &= static_cast<std::size_t>(ptr[shift]) << (shift * sizeof(std::byte));
    if (shift < n)
        return hash_combine(result, hash_bytes(ptr + shift, n - shift));
    return result;
}
} // namespace structural::detail

#endif // STRUCTURAL_HASH_COMBINE_HPP
