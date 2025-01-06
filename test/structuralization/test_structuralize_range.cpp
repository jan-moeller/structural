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

#include "structural/inplace_vector.hpp"
#include "structural/structuralize.hpp"

#include <bugspray/bugspray.hpp>

TEST_CASE("structuralize - range", "[utility]")
{
    using namespace structural;

    constexpr auto arr0 = STRUCTURALIZE(std::vector<int>{});
    constexpr auto arr1 = STRUCTURALIZE(std::vector<int>{42});
    constexpr auto arr2 = STRUCTURALIZE(std::vector<int>{42, 9});
    CHECK(arr0 == std::array<int, 0>{});
    CHECK(arr1 == std::array{42});
    CHECK(arr2 == std::array{42, 9});

    constexpr auto arr10 = STRUCTURALIZE(std::vector{std::vector<int>{1}});
    CHECK(arr10 == std::array{std::array{1}});

    constexpr auto tuparr = STRUCTURALIZE(std::vector{std::vector<int>{1}, std::vector<int>{42, 43, 44}});
    CHECK(tuparr == structural::tuple{std::array{1}, std::array{42, 43, 44}});

    // check that structural range stays the same
    constexpr auto test = STRUCTURALIZE(structural::inplace_vector<int, 3>{1, 2, 3});
    CHECK(test == structural::inplace_vector<int, 3>{1, 2, 3});
}
EVAL_TEST_CASE("structuralize - range");