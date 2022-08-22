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
#include "structural/static_set.hpp"

#include <bugspray/bugspray.hpp>

TEST_CASE("static_set - lower_bound, upper_bound, equal_range", "[container]")
{
    using namespace structural;

    std::array const    data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    static_set<int, 15> ss{data.begin(), data.end()};

    SECTION("lower_bound")
    {
        CHECK(*ss.lower_bound(0) == 1);
        for (auto&& e : data)
            CHECK(*ss.lower_bound(e) == e);
        CHECK(ss.lower_bound(12) == ss.end());
    }
    SECTION("upper_bound")
    {
        CHECK(*ss.upper_bound(0) == 1);
        for (auto iter = data.begin(); iter != data.end() - 1; ++iter)
            CHECK(*ss.upper_bound(*iter) == *(iter + 1));
        CHECK(ss.upper_bound(11) == ss.end());
    }
    SECTION("equal_range")
    {
        SECTION("nonexistent element")
        {
            CHECK(get<0>(ss.equal_range(0)) == ss.lower_bound(0));
            CHECK(get<1>(ss.equal_range(0)) == ss.upper_bound(0));
            CHECK(get<0>(ss.equal_range(0)) == get<1>(ss.equal_range(0)));
        }
        SECTION("existing element")
        {
            CHECK(get<0>(ss.equal_range(5)) == ss.lower_bound(5));
            CHECK(get<1>(ss.equal_range(5)) == ss.upper_bound(5));
            CHECK(get<0>(ss.equal_range(5)) != get<1>(ss.equal_range(5)));
        }
    }
}
EVAL_TEST_CASE("static_set - lower_bound, upper_bound, equal_range");