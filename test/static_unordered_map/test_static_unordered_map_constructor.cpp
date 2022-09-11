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
#include "structural/static_unordered_map.hpp"

#include <bugspray/bugspray.hpp>

TEST_CASE("static_unordered_map - constructors", "[container]")
{
    using namespace structural;

    SECTION("default")
    {
        static_unordered_map<int, int, 15> ss;
        CHECK(ss.empty());
    }
    SECTION("range")
    {
        std::array<pair<int, int>, 9> data{
            {{1, 2}, {5, -3}, {4, 42}, {8, 8}, {6, 2}, {8, 9}, {9, 24}, {-1, 0}, {42, 1337}}};
        static_unordered_map<int, int, 15> ss{data.begin(), data.end()};
        CHECK(ss.size() == 8);
        for (auto&& e : data)
            CHECK(ss.contains(e));
    }
    SECTION("initializer_list")
    {
        static_unordered_map<int, int, 15>
            ss{{1, 2}, {5, -3}, {4, 42}, {8, 8}, {6, 2}, {8, 9}, {9, 24}, {-1, 0}, {42, 1337}};
        CHECK(ss.size() == 8);
        CHECK(ss.contains(1));
        CHECK(ss.contains(5));
        CHECK(ss.contains(4));
        CHECK(ss.contains(8));
        CHECK(ss.contains(6));
        CHECK(ss.contains(9));
        CHECK(ss.contains(-1));
        CHECK(ss.contains(42));
    }
    SECTION("copy")
    {
        static_unordered_map<int, int, 15>
                   ss{{1, 2}, {5, -3}, {4, 42}, {8, 8}, {6, 2}, {8, 9}, {9, 24}, {-1, 0}, {42, 1337}};
        auto const cp = ss;
        CHECK(ss == cp);
    }
    SECTION("move")
    {
        static_unordered_map<int, int, 15>
                   ss{{1, 2}, {5, -3}, {4, 42}, {8, 8}, {6, 2}, {8, 9}, {9, 24}, {-1, 0}, {42, 1337}};
        auto const cp   = ss;
        auto const move = std::move(ss);
        CHECK(move == cp);
    }
}
EVAL_TEST_CASE("static_unordered_map - constructors");