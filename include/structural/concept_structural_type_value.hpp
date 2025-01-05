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

#ifndef STRUCTURAL_CONCEPT_STRUCTURAL_TYPE_VALUE_HPP
#define STRUCTURAL_CONCEPT_STRUCTURAL_TYPE_VALUE_HPP

#include <type_traits>

namespace structural
{
namespace detail
{
template<auto Value>
struct test_structurality
{
};
} // namespace detail

template<typename T>
concept structural_type = requires { typename detail::test_structurality<T{}>; };

template<auto Value>
concept structural_value = requires { typename detail::test_structurality<Value>; };
} // namespace structural

#endif // STRUCTURAL_CONCEPT_STRUCTURAL_TYPE_VALUE_HPP
