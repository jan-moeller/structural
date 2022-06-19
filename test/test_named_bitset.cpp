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
#include "concept_structural_type_value.hpp"
#include "structural/named_bitset.hpp"

#include <bugspray/bugspray.hpp>

STRUCTURAL_MAKE_NAMED_BITSET(empty_bits, empty)
STRUCTURAL_MAKE_NAMED_BITSET(color_bits, colors, red, green, blue, yellow)

TEST_CASE("named_bitset")
{
    using namespace structural;
    using enum color_bits;

    auto c = red | blue | green;
    REQUIRE(colors::size() == 4);

    SECTION("test")
    {
        REQUIRE(c.test(red));
        REQUIRE(c.test(blue));
        REQUIRE(c.test(green));
        REQUIRE(!c.test(yellow));

        SECTION("test_all")
        {
            REQUIRE(c.test_all(red));
            REQUIRE(c.test_all(red | green));
            REQUIRE(!c.test_all(red | yellow));
        }
        SECTION("test_any")
        {
            REQUIRE(c.test_any(red));
            REQUIRE(c.test_any(red | yellow));
            REQUIRE(!c.test_any(yellow));
        }
        SECTION("test_none")
        {
            REQUIRE(c.test_none(yellow));
            REQUIRE(!c.test_none(red | green));
            REQUIRE(!c.test_none(red | yellow));
        }
    }

    SECTION("set")
    {
        c.set(yellow);
        REQUIRE(c.all());

        c.set(yellow, false);
        REQUIRE(!c.test(yellow));
    }
    SECTION("reset")
    {
        c.reset(red);
        REQUIRE(!c.test(red));
    }
    SECTION("flip")
    {
        c.flip(red);
        REQUIRE(!c.test(red));

        c.flip(red);
        REQUIRE(c.test(red));
    }

    SECTION("stringification", runtime)
    {
        REQUIRE(c.to_string() == "red | green | blue");
    }

    SECTION("operator&")
    {
        REQUIRE((c & c) == c);
        REQUIRE((c & yellow).none());
    }
    SECTION("operator|")
    {
        REQUIRE((c | c) == c);
        REQUIRE((c | yellow).all());
    }
    SECTION("operator^")
    {
        REQUIRE((c ^ c) == colors{});
        REQUIRE((c ^ yellow).all());
    }

    SECTION("structural")
    {
        CHECK(structural_value<yellow | red>);
    }
}
EVAL_TEST_CASE("named_bitset");