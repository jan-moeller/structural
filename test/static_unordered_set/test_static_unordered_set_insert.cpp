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

TEST_CASE("static_unordered_set - insert", "[container]")
{
    using namespace structural;

    static_unordered_set<int, 15> ss;
    std::array const              data{1, 2, 3, 4, 5, -1, -2, -3, 42, 90, -12345};

    SECTION("const&")
    {
        for (std::size_t i = 0; i < data.size(); ++i)
        {
            ss.insert(data[i]);
            CHECK(ss.size() == i + 1);
            for (std::size_t j = 0; j <= i; ++j)
                CHECK(ss.contains(data[i]));
        }
    }
    SECTION("&&")
    {
        auto datacp = data;
        for (std::size_t i = 0; i < data.size(); ++i)
        {
            ss.insert(std::move(datacp[i]));
            CHECK(ss.size() == i + 1);
            for (std::size_t j = 0; j <= i; ++j)
                CHECK(ss.contains(data[i]));
        }
    }
    SECTION("range")
    {
        ss.insert(data.begin(), data.end());
        CHECK(std::ranges::equal(ss, static_unordered_set<int, 15>{data.begin(), data.end()}));
    }
    SECTION("initializer_list")
    {
        ss.insert({1, 2, 3});
        CHECK(ss.contains(1));
        CHECK(ss.contains(2));
        CHECK(ss.contains(3));
    }
}
EVAL_TEST_CASE("static_unordered_set - insert");