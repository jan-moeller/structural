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
#include "structural/static_unordered_set.hpp"

#include <bugspray/bugspray.hpp>

TEST_CASE("static_unordered_set - assignment", "[container]")
{
    using namespace structural;

    static_unordered_set<int, 15> ss{1, 5, 4, 8, 6, 8, 9, -1, 42};
    static_unordered_set<int, 15> ss2{-3, 5, -15};

    SECTION("copy")
    {
        ss = ss2;
        CHECK(ss.size() == ss2.size());
        CHECK(ss == ss2);
    }
    SECTION("move")
    {
        auto const ss2cp = ss2;
        ss               = std::move(ss2);
        CHECK(ss.size() == ss2cp.size());
        CHECK(ss == ss2cp);
    }
}
EVAL_TEST_CASE("static_unordered_set - assignment");