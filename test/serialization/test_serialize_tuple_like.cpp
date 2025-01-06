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
#include "structural/tuple.hpp"

#include <bugspray/bugspray.hpp>

#include <array>
#include <tuple>
#include <utility>

TEST_CASE("serialize - tuple-like", "[utility]")
{
    using namespace structural;

    CHECK(deserialize<std::tuple<>>(serialize(std::make_tuple())) == std::make_tuple());
    CHECK(deserialize<std::pair<int, float>>(serialize(std::make_pair(42, 3.12f))) == std::make_pair(42, 3.12f));
    CHECK(deserialize<std::array<int, 3>>(serialize(std::array{1, 2, 3})) == std::array{1, 2, 3});
    CHECK(deserialize<structural::tuple<int, bool, float>>(serialize(structural::tuple{1, true, 42.42f}))
          == structural::tuple{1, true, 42.42f});
    CHECK(deserialize<structural::tuple<std::variant<int>>>(serialize(structural::tuple{std::variant<int>(42)}))
          == structural::tuple{std::variant<int>(42)});
}
EVAL_TEST_CASE("serialize - tuple-like");