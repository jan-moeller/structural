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

#include "structural/structuralize.hpp"

#include <bugspray/bugspray.hpp>

TEST_CASE("structuralize - tuple-like", "[utility]")
{
    using namespace structural;

    constexpr auto tup0 = STRUCTURALIZE(std::tuple<>{});
    constexpr auto tup1 = STRUCTURALIZE(std::tuple{42});
    constexpr auto tup2 = STRUCTURALIZE(std::tuple{42, 9.3});
    CHECK(tup0 == structural::tuple{});
    CHECK(tup1 == structural::tuple{42});
    CHECK(tup2 == structural::tuple{42, 9.3});
}
EVAL_TEST_CASE("structuralize - tuple-like");