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
    SECTION("constructors")
    {
        SECTION("default constructor")
        {
            CHECK(!bs.all());
            CHECK(!bs.any());
            CHECK(bs.none());
        }
        SECTION("from integer")
        {
            CHECK(bitset<9>{0b110110101} == bitset<9>{0b110110101u});
        }
        SECTION("from string")
        {
            CHECK(bitset<9>{"110110101"} == bitset<9>{0b110110101u});
        }
    }
    SECTION("literal")
    {
        CHECK(110110101_bits == bitset<9>{0b110110101u});
        CHECK(1'1011'0101_bits == bitset<9>{0b110110101u});
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
        CHECK((1'1011'0111_bits & 1'0101'0101_bits) == 1'0001'0101_bits);
    }
    SECTION("operator|")
    {
        CHECK((1'1011'0111_bits | 1'0101'0101_bits) == 1'1111'0111_bits);
    }
    SECTION("operator^")
    {
        CHECK((1'1011'0111_bits ^ 1'0101'0101_bits) == 0'1110'0010_bits);
    }
    SECTION("operator<<")
    {
        CHECK((1'1011'0111_bits << 3u) == 1'1011'1000_bits);
        CHECK((110'1101'1100'1001_bits << 3u) == 110'1110'0100'1000_bits);
    }
    SECTION("operator>>")
    {
        CHECK((1'1011'0111_bits >> 3u) == 0'0011'0110_bits);
        CHECK((110'1101'1100'1001_bits >> 3u) == 000'1101'1011'1001_bits);
    }

    SECTION("structural")
    {
        CHECK(test_structurality<1101_bits>::value == 1101_bits);
    }
}

EVAL_TEST_CASE("bitset");