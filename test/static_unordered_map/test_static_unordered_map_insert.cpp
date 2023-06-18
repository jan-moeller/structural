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

#include <string_view>

TEST_CASE("static_unordered_map - insert", "[container]")
{
    using namespace structural;

    static_unordered_map<int, std::string_view, 15>         sm;
    std::array<pair<const int, std::string_view>, 11> const data{
        {{1, ""}, {2, ""}, {3, ""}, {4, ""}, {5, ""}, {-1, ""}, {-2, ""}, {-3, ""}, {42, ""}, {90, ""}, {-12345, ""}}};

    SECTION("insert")
    {
        SECTION("const&")
        {
            for (std::size_t i = 0; i < data.size(); ++i)
            {
                sm.insert(data[i]);
                CHECK(sm.size() == i + 1);
                for (std::size_t j = 0; j <= i; ++j)
                    CHECK(sm.contains(data[i]));
            }
            auto const size = sm.size();
            sm.insert({data.front().first, "foobar"});
            CHECK(size == sm.size());
            CHECK(sm.find(data.front())->second == data.front().second);
        }
        SECTION("&&")
        {
            auto datacp = data;
            for (std::size_t i = 0; i < data.size(); ++i)
            {
                sm.insert(std::move(datacp[i]));
                CHECK(sm.size() == i + 1);
                for (std::size_t j = 0; j <= i; ++j)
                    CHECK(sm.contains(data[i]));
            }
        }
        SECTION("range")
        {
            sm.insert(data.begin(), data.end());
            CHECK(std::ranges::equal(sm, static_unordered_map<int, std::string_view, 15>{data.begin(), data.end()}));
        }
        SECTION("initializer_list")
        {
            sm.insert({{1, ""}, {2, ""}, {3, ""}});
            CHECK(sm.contains(1));
            CHECK(sm.contains(2));
            CHECK(sm.contains(3));
        }
    }
    SECTION("insert_or_assign")
    {
        SECTION("key const&")
        {
            for (std::size_t i = 0; i < data.size(); ++i)
            {
                sm.insert_or_assign(data[i].first, data[i].second);
                CHECK(sm.size() == i + 1);
                for (std::size_t j = 0; j <= i; ++j)
                    CHECK(sm.contains(data[i]));
            }
            auto const size = sm.size();
            sm.insert_or_assign(data.front().first, "foobar");
            CHECK(size == sm.size());
            CHECK(sm.find(data.front())->second == "foobar");
        }
        SECTION("key&&")
        {
            for (std::size_t i = 0; i < data.size(); ++i)
            {
                int              key   = data[i].first;
                std::string_view value = data[i].second;
                sm.insert_or_assign(std::move(key), std::move(value));
                CHECK(sm.size() == i + 1);
                for (std::size_t j = 0; j <= i; ++j)
                    CHECK(sm.contains(data[i]));
            }
            auto const       size  = sm.size();
            int              key   = data.front().first;
            sm.insert_or_assign(std::move(key), "foobar");
            CHECK(size == sm.size());
            CHECK(sm.find(data.front())->second == "foobar");
        }
        SECTION("operator[]")
        {
            for (std::size_t i = 0; i < data.size(); ++i)
            {
                int              key   = data[i].first;
                std::string_view value = data[i].second;
                sm[key]                = value;
                CHECK(sm.size() == i + 1);
                for (std::size_t j = 0; j <= i; ++j)
                    CHECK(sm.contains(data[i]));
            }
            auto const       size  = sm.size();
            int              key   = data.front().first;
            sm[key]                = "foobar";
            CHECK(size == sm.size());
            CHECK(sm.find(data.front())->second == "foobar");
        }
    }
}
EVAL_TEST_CASE("static_unordered_map - insert");