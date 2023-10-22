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
#include "structural/inplace_vector.hpp"

#include <bugspray/bugspray.hpp>

TEST_CASE("inplace_vector - structurality", "[container][inplace_vector]")
{
    using namespace structural;

    constexpr std::size_t C = 10;
    CHECK(structural_type<inplace_vector<int, C>>);
    CHECK(structural_type<inplace_vector<inplace_vector<int, C>, C>>);
    CHECK(!structural_type<inplace_vector<std::string, C>>);
    CHECK(!structural_type<inplace_vector<inplace_vector<std::string, C>, C>>);
}
EVAL_TEST_CASE("inplace_vector - structurality");