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
#include "structural/inplace_map.hpp"

#include <bugspray/bugspray.hpp>

#include <string_view>

TEST_CASE("inplace_map - assignment", "[container]")
{
    using namespace structural;
    using namespace std::string_view_literals;

    inplace_map<std::string_view, int, 15> sm{{"foo"sv, 1},
                                             {""sv, 5},
                                             {"bar"sv, 4},
                                             {"baz"sv, 8},
                                             {"bam"sv, 6},
                                             {"baz"sv, 8}};
    inplace_map<std::string_view, int, 15> sm2{{"wub"sv, 0}, {"gnarf"sv, 35}};

    SECTION("copy")
    {
        sm = sm2;
        CHECK(sm.size() == sm2.size());
        CHECK(sm == sm2);
    }
    SECTION("move")
    {
        auto const ss2cp = sm2;
        sm               = std::move(sm2);
        CHECK(sm.size() == ss2cp.size());
        CHECK(sm == ss2cp);
    }
}
EVAL_TEST_CASE("inplace_map - assignment");