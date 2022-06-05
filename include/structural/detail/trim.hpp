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

#ifndef STRUCTURAL_TRIM_HPP
#define STRUCTURAL_TRIM_HPP

#include <string_view>

namespace structural::detail
{
constexpr auto trim_left(std::string_view sv, char c = ' ') noexcept -> std::string_view
{
    while (sv.starts_with(c))
        sv.remove_prefix(1);
    return sv;
}

constexpr auto trim_right(std::string_view sv, char c = ' ') noexcept -> std::string_view
{
    while (sv.ends_with(c))
        sv.remove_suffix(1);
    return sv;
}

constexpr auto trim(std::string_view sv, char c = ' ') noexcept -> std::string_view
{
    return trim_left(trim_right(sv, c), c);
}
} // namespace structural::detail

#endif // STRUCTURAL_TRIM_HPP
