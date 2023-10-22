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
#include "../concept_structural_type_value.hpp"
#include "structural/basic_inplace_string.hpp"

#include <bugspray/bugspray.hpp>

TEST_CASE("basic_inplace_string - structurality", "[container][inplace_string]")
{
    using namespace structural;

    constexpr std::size_t C = 10;
    CHECK(structural_type<basic_inplace_string<char, C>>);
    CHECK(structural_type<basic_inplace_string<wchar_t, C>>);
    CHECK(structural_type<basic_inplace_string<char8_t, C>>);
    CHECK(structural_type<basic_inplace_string<char16_t, C>>);
    CHECK(structural_type<basic_inplace_string<char32_t, C>>);
}
EVAL_TEST_CASE("basic_inplace_string - structurality");