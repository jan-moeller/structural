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

TEST_CASE("inplace_map - lower_bound, upper_bound, equal_range", "[container]")
{
    using namespace structural;

    std::array<pair<const int, const char*>, 11> const data = {{{1, ""},
                                                                {2, "foo"},
                                                                {3, "bar"},
                                                                {4, "baz"},
                                                                {5, "boom"},
                                                                {6, ""},
                                                                {7, ""},
                                                                {8, ""},
                                                                {9, ""},
                                                                {10, ""},
                                                                {11, ""}}};
    inplace_map<int, char const*, 15>                   sm{data.begin(), data.end()};

    SECTION("lower_bound")
    {
        CHECK(sm.lower_bound(0)->first == 1);
        for (auto&& e : data)
            CHECK(*sm.lower_bound(e) == e);
        CHECK(sm.lower_bound(12) == sm.end());
    }
    SECTION("upper_bound")
    {
        CHECK(sm.upper_bound(0)->first == 1);
        for (auto iter = data.begin(); iter != data.end() - 1; ++iter)
            CHECK(*sm.upper_bound(*iter) == *(iter + 1));
        CHECK(sm.upper_bound(11) == sm.end());
    }
    SECTION("equal_range")
    {
        SECTION("nonexistent element")
        {
            CHECK(get<0>(sm.equal_range(0)) == sm.lower_bound(0));
            CHECK(get<1>(sm.equal_range(0)) == sm.upper_bound(0));
            CHECK(get<0>(sm.equal_range(0)) == get<1>(sm.equal_range(0)));
        }
        SECTION("existing element")
        {
            CHECK(get<0>(sm.equal_range(5)) == sm.lower_bound(5));
            CHECK(get<1>(sm.equal_range(5)) == sm.upper_bound(5));
            CHECK(get<0>(sm.equal_range(5)) != get<1>(sm.equal_range(5)));
        }
    }
}
EVAL_TEST_CASE("inplace_map - lower_bound, upper_bound, equal_range");