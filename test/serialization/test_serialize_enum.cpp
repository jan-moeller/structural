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

#include <bugspray/bugspray.hpp>

#include <cstddef>

TEST_CASE("serialize - enum", "[utility]")
{
    using namespace structural;

    enum foo
    {
        a,
        b,
    };
    enum class bar : std::uint8_t
    {
        a,
        b,
    };

    CHECK(deserialize<foo>(serialize(foo::a)) == foo::a);
    CHECK(deserialize<foo>(serialize(foo::b)) == foo::b);
    CHECK(deserialize<bar>(serialize(bar::a)) == bar::a);
    CHECK(deserialize<bar>(serialize(bar::b)) == bar::b);

    CHECK(deserialize<std::byte>(serialize(std::byte{0})) == std::byte{0});
    CHECK(deserialize<std::byte>(serialize(std::byte{42})) == std::byte{42});
    CHECK(deserialize<std::byte>(serialize(std::byte{255})) == std::byte{255});
}
EVAL_TEST_CASE("serialize - enum");