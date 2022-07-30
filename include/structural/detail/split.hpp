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

#ifndef STRUCTURAL_SPLIT_HPP
#define STRUCTURAL_SPLIT_HPP

#include <algorithm>
#include <ranges>
#include <string_view>
#include <vector>

namespace structural::detail
{
template<std::size_t N>
constexpr auto split(std::string_view sv, char delimiter) -> std::array<std::string_view, N>
{
    auto r = sv | std::views::split(delimiter)
             | std::views::transform(
                 [](auto&& in) -> std::string_view
                 {
                     // &*in.begin() is safe here because we know that the original string was contiguous
                     return std::string_view{&*in.begin(), static_cast<std::size_t>(std::ranges::distance(in))};
                 });
    std::array<std::string_view, N> results;
    std::ranges::copy(r, results.begin());
    return results;
}
} // namespace structural::detail

#endif // STRUCTURAL_SPLIT_HPP
