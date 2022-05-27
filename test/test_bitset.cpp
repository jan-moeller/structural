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

#include "structural/bitset.hpp"

#include <bugspray/bugspray.hpp>

template<auto Value>
struct test_structurality
{
    static constexpr auto const& value = Value;
};

TEST_CASE("bitset")
{
    using namespace structural;

    constexpr auto size = 65;
    bitset<size>   bs;
    SECTION("after construction, all bits are unset")
    {
        CHECK(!bs.all());
        CHECK(!bs.any());
        CHECK(bs.none());
    }

    SECTION("set")
    {
        SECTION("setting all")
        {
            bs.set();
            CHECK(bs.all());
        }
        SECTION("setting individual bit")
        {
            bs.set(34);
            CHECK(bs.any());
            CHECK(!bs.none());
            CHECK(bs.test(34));
        }
        SECTION("unsetting individual bit")
        {
            bs.set(); // First set all
            bs.set(34, false);
            CHECK(!bs.test(34));
        }
    }

    SECTION("reset")
    {
        bs.set(); // Set all so we have something to reset
        SECTION("reset all")
        {
            bs.reset();
            CHECK(bs.none());
        }
        SECTION("reset some")
        {
            bs.reset(34);
            CHECK(bs.any());
            CHECK(!bs.test(34));
        }
    }

    SECTION("flip")
    {
        SECTION("flip all")
        {
            // First set some bits so we have something to flip
            bs.set(3);
            bs.set(34);

            bs.flip();
            CHECK(bs.any());
            CHECK(!bs.none());
            CHECK(!bs.all());
            CHECK(!bs.test(3) && !bs.test(34));
        }
        SECTION("flip some")
        {
            bs.flip(34);
            CHECK(bs.test(34));
            bs.flip(34);
            CHECK(!bs.test(34));
        }
    }
    SECTION("operator&")
    {
        CHECK((110110111_bits & 101010101_bits) == 100010101_bits);
    }
    SECTION("operator|")
    {
        CHECK((110110111_bits | 101010101_bits) == 111110111_bits);
    }
    SECTION("operator^")
    {
        CHECK((110110111_bits ^ 101010101_bits) == 011100010_bits);
    }
    SECTION("operator<<")
    {
        CHECK((110110111_bits << 3u) == 110111000_bits);
        CHECK((110110111001001_bits << 3u) == 110111001001000_bits);
    }
    SECTION("operator>>")
    {
        CHECK((110110111_bits >> 3u) == 000110110_bits);
        CHECK((110110111001001_bits >> 3u) == 000110110111001_bits);
    }

    SECTION("structural")
    {
        CHECK(test_structurality<1101_bits>::value == 1101_bits);
    }
}

EVAL_TEST_CASE("bitset");