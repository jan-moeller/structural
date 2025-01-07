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

TEST_CASE("serialize - aggregate", "[utility]")
{
    using namespace structural;

    struct foo_0
    {
        constexpr auto operator==(foo_0 const&) const -> bool = default;
    };
    struct foo_1
    {
        int            i                                      = 0;
        constexpr auto operator==(foo_1 const&) const -> bool = default;
    };
    struct foo_2
    {
        int            i                                      = 0;
        float          j                                      = 42.f;
        constexpr auto operator==(foo_2 const&) const -> bool = default;
    };
    struct foo_5
    {
        int            i                                      = 0;
        float          j                                      = 42.f;
        bool           b                                      = false;
        double         d                                      = 1.23;
        char           c                                      = 'c';
        constexpr auto operator==(foo_5 const&) const -> bool = default;
    };
    struct foo_nonliteral
    {
        int                  a;
        std::optional<float> b;
        constexpr auto       operator==(foo_nonliteral const&) const -> bool = default;
    };

    CHECK(deserialize<foo_0>(serialize(foo_0{})) == foo_0{});
    CHECK(deserialize<foo_1>(serialize(foo_1{42})) == foo_1{42});
    CHECK(deserialize<foo_2>(serialize(foo_2{42, 0.1f})) == foo_2{42, 0.1f});
    CHECK(deserialize<foo_5>(serialize(foo_5{42, 0.1f, true, 5.43, 'a'})) == foo_5{42, 0.1f, true, 5.43, 'a'});
    CHECK(deserialize<foo_nonliteral>(serialize(foo_nonliteral{42, std::nullopt})) == foo_nonliteral{42, std::nullopt});
}
EVAL_TEST_CASE("serialize - aggregate");