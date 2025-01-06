//
// MIT License
//
// Copyright (c) 2025 Jan Möller
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
#include "serialize_helper.hpp"
#include "structural/inplace_map.hpp"
#include "structural/inplace_set.hpp"
#include "structural/inplace_string.hpp"
#include "structural/inplace_unordered_map.hpp"
#include "structural/inplace_unordered_set.hpp"
#include "structural/inplace_vector.hpp"

#include <bugspray/bugspray.hpp>

#include <deque>
#include <forward_list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

TEST_CASE("serialize - range", "[utility]")
{
    using namespace structural;

    CHECK(deserialize<std::vector<int>>(serialize(std::vector<int>{})) == std::vector<int>{});
    CHECK(deserialize<std::vector<int>>(serialize(std::vector{1, 2, 3})) == std::vector{1, 2, 3});
    CHECK(deserialize<std::string>(serialize(std::string{"foo"})) == std::string{"foo"};);
    CHECK(deserialize<std::u8string>(serialize(std::u8string{u8"foo"})) == std::u8string{u8"foo"});
    CHECK(deserialize<std::u16string>(serialize(std::u16string{u"foo"})) == std::u16string{u"foo"});
    CHECK(deserialize<std::u32string>(serialize(std::u32string{U"foo"})) == std::u32string{U"foo"});
    CHECK(deserialize<structural::inplace_vector<int, 3>>(serialize(structural::inplace_vector<int, 3>{1, 2, 3}))
          == structural::inplace_vector<int, 3>{1, 2, 3});
    CHECK(deserialize<structural::inplace_string<3>>(serialize(structural::inplace_string<3>{"asd"}))
          == structural::inplace_string<3>{"asd"});
    CHECK(deserialize<structural::inplace_map<int, float, 2>>(
              serialize(structural::inplace_map<int, float, 2>{{1, 1.f}, {2, 42.3f}}))
          == structural::inplace_map<int, float, 2>{{1, 1.f}, {2, 42.3f}});
    CHECK(deserialize<structural::inplace_unordered_map<int, float, 2>>(
              serialize(structural::inplace_unordered_map<int, float, 2>{{1, 1.f}, {2, 42.3f}}))
          == structural::inplace_unordered_map<int, float, 2>{{1, 1.f}, {2, 42.3f}});
    CHECK(deserialize<structural::inplace_set<int, 3>>(serialize(structural::inplace_set<int, 3>{1, 2, 3}))
          == structural::inplace_set<int, 3>{1, 2, 3});
    CHECK(deserialize<structural::inplace_unordered_set<int, 3>>(
              serialize(structural::inplace_unordered_set<int, 3>{1, 2, 3}))
          == structural::inplace_unordered_set<int, 3>{1, 2, 3});

    CHECK(deserialize<std::vector<std::vector<int>>>(serialize(std::vector<std::vector<int>>{std::vector{1, 2, 3}}))
          == std::vector<std::vector<int>>{std::vector{1, 2, 3}});

    SECTION("runtime-only", runtime)
    {
        CHECK(deserialize<std::forward_list<int>>(serialize(std::forward_list{1, 2, 3})) == std::forward_list{1, 2, 3});
        CHECK(deserialize<std::deque<int>>(serialize(std::deque{1, 2, 3})) == std::deque{1, 2, 3});
        CHECK(deserialize<std::map<int, float>>(serialize(std::map<int, float>{{1, 1.f}, {2, 42.3f}}))
              == std::map<int, float>{{1, 1.f}, {2, 42.3f}});
        CHECK(
            deserialize<std::unordered_map<int, float>>(serialize(std::unordered_map<int, float>{{1, 1.f}, {2, 42.3f}}))
            == std::unordered_map<int, float>{{1, 1.f}, {2, 42.3f}});
        CHECK(deserialize<std::set<int>>(serialize(std::set{1, 2, 3})) == std::set{1, 2, 3});
        CHECK(deserialize<std::unordered_set<int>>(serialize(std::unordered_set{1, 2, 3}))
              == std::unordered_set{1, 2, 3});
    }
}
EVAL_TEST_CASE("serialize - range");