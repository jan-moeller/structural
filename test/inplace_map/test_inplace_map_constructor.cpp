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

TEST_CASE("inplace_map - constructors", "[container]")
{
    using namespace structural;

    SECTION("default")
    {
        inplace_map<int, char const*, 15> sm;
        CHECK(sm.empty());
    }
    SECTION("range")
    {
        std::array<pair<int const, char const*>, 9> data{
            {{1, "foo"}, {5, "bar"}, {4, ""}, {8, "a"}, {6, "b"}, {8, "c"}, {9, "d"}, {-1, "e"}, {42, "f"}}};
        inplace_map<int, char const*, 15> sm{data.begin(), data.end()};
        CHECK(sm.size() == 8);
        for (auto&& e : data)
            CHECK(sm.contains(e));
    }
    SECTION("initializer_list")
    {
        inplace_map<int, char const*, 15>
            sm{{1, "foo"}, {5, "bar"}, {4, ""}, {8, "a"}, {6, "b"}, {8, "c"}, {9, "d"}, {-1, "e"}, {42, "f"}};
        CHECK(sm.size() == 8);
        CHECK(sm.contains(1));
        CHECK(sm.contains(5));
        CHECK(sm.contains(4));
        CHECK(sm.contains(8));
        CHECK(sm.contains(6));
        CHECK(sm.contains(9));
        CHECK(sm.contains(-1));
        CHECK(sm.contains(42));
    }
    SECTION("copy")
    {
        inplace_map<int, char const*, 15>
                   sm{{1, "foo"}, {5, "bar"}, {4, ""}, {8, "a"}, {6, "b"}, {8, "c"}, {9, "d"}, {-1, "e"}, {42, "f"}};
        auto const cp = sm;
        CHECK(sm == cp);
    }
    SECTION("move")
    {
        inplace_map<int, char const*, 15>
                   sm{{1, "foo"}, {5, "bar"}, {4, ""}, {8, "a"}, {6, "b"}, {8, "c"}, {9, "d"}, {-1, "e"}, {42, "f"}};
        auto const cp   = sm;
        auto const move = std::move(sm);
        CHECK(move == cp);
    }
}
EVAL_TEST_CASE("inplace_map - constructors");