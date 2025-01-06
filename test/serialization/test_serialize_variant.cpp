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

#include <variant>

struct foo
{
    foo() = delete;
    constexpr explicit foo(int i)
        : bar(i)
    {
    }

    int bar;

    constexpr auto operator==(foo const&) const -> bool = default;
};

constexpr void serialize(foo const& value, std::output_iterator<std::byte> auto& out_iter)
{
    using structural::serialize;
    serialize(value.bar, out_iter);
}
constexpr auto deserialize(std::in_place_type_t<foo>, std::input_iterator auto& in_iter) -> foo
{
    using structural::deserialize;
    return foo{deserialize(std::in_place_type<int>, in_iter)};
}

TEST_CASE("serialize - variant", "[utility]")
{
    using namespace structural;

    CHECK(deserialize<std::variant<std::monostate>>(serialize(std::variant<std::monostate>{}))
          == std::variant<std::monostate>{});
    CHECK(deserialize<std::variant<int, float>>(serialize(std::variant<int, float>{42}))
          == std::variant<int, float>{42});
    CHECK(deserialize<std::variant<int, float>>(serialize(std::variant<int, float>{1.23f}))
          == std::variant<int, float>{1.23f});
    CHECK(deserialize<std::variant<int, int>>(serialize(std::variant<int, int>{std::in_place_index<1>, 42}))
          == std::variant<int, int>{std::in_place_index<1>, 42});
    CHECK(deserialize<std::variant<foo>>(serialize(std::variant<foo>{foo(42)})) == std::variant<foo>{foo(42)});
}
EVAL_TEST_CASE("serialize - variant");