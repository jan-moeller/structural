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
#include "structural/static_map.hpp"

#include <bugspray/bugspray.hpp>

TEST_CASE("static_map - erase", "[container]")
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
    static_map<int, const char*, 15>                   sm{data.begin(), data.end()};

    SECTION("iterator")
    {
        CHECK((sm.erase(sm.find(3)))->first == 4);
        CHECK(sm.size() == 10);
        for (auto&& e : data)
            CHECK(sm.contains(e) == (e.first != 3));
    }
    SECTION("iterator pair")
    {
        CHECK((sm.erase(sm.find(3), sm.find(6)))->first == 6);
        CHECK(sm.size() == 8);
        for (auto&& e : data)
            CHECK(sm.contains(e) == (e.first < 3 || e.first >= 6));
    }
    SECTION("erase_if")
    {
        auto const pred = [](pair<const int, char const*> const& i)
        {
            return i.first > 3 && i.first < 6;
        };
        erase_if(sm, pred);
        for (auto&& e : sm)
            CHECK(!pred(e));
    }
}
EVAL_TEST_CASE("static_map - erase");