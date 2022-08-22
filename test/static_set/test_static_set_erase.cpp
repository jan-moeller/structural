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

TEST_CASE("static_set - erase", "[container]")
{
    using namespace structural;

    std::array const    data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    static_set<int, 15> ss{data.begin(), data.end()};

    SECTION("iterator")
    {
        CHECK((*ss.erase(ss.find(3))) == 4);
        CHECK(ss.size() == 10);
        for (auto&& e : data)
            CHECK(ss.contains(e) == (e != 3));
    }
    SECTION("iterator pair")
    {
        CHECK((*ss.erase(ss.find(3), ss.find(6))) == 6);
        CHECK(ss.size() == 8);
        for (auto&& e : data)
            CHECK(ss.contains(e) == (e < 3 || e >= 6));
    }
    SECTION("erase_if")
    {
        auto const pred = [](int const& i)
        {
            return i > 3 && i < 6;
        };
        erase_if(ss, pred);
        for (auto&& e : ss)
            CHECK(!pred(e));
    }
}
EVAL_TEST_CASE("static_set - erase");