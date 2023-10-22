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
#include "structural/inplace_string.hpp"

#include <bugspray/bugspray.hpp>

TEST_CASE("basic_inplace_string - capacity deduction", "[container][inplace_string]")
{
    using namespace structural;

    SECTION("basic_inplace_string")
    {
        basic_inplace_string ss = "foobar";
        CHECK(ss.capacity() == 7);
        CHECK(ss.size() == 6);
        CHECK(ss == "foobar");
        CHECK(*ss.end() == '\0');
    }
    SECTION("inplace_string")
    {
        inplace_string ss = "foobar";
        CHECK(ss.capacity() == 7);
        CHECK(ss.size() == 6);
        CHECK(ss == "foobar");
        CHECK(*ss.end() == '\0');
    }
    SECTION("inplace_wstring")
    {
        inplace_wstring ss = L"foobar";
        CHECK(ss.capacity() == 7);
        CHECK(ss.size() == 6);
        CHECK(ss == L"foobar");
        CHECK(*ss.end() == '\0');
    }
    SECTION("inplace_u8string")
    {
        inplace_u8string ss = u8"foobar";
        CHECK(ss.capacity() == 7);
        CHECK(ss.size() == 6);
        CHECK(ss == u8"foobar");
        CHECK(*ss.end() == '\0');
    }
    SECTION("inplace_u16string")
    {
        inplace_u16string ss = u"foobar";
        CHECK(ss.capacity() == 7);
        CHECK(ss.size() == 6);
        CHECK(ss == u"foobar");
        CHECK(*ss.end() == '\0');
    }
    SECTION("inplace_u32string")
    {
        inplace_u32string ss = U"foobar";
        CHECK(ss.capacity() == 7);
        CHECK(ss.size() == 6);
        CHECK(ss == U"foobar");
        CHECK(*ss.end() == '\0');
    }
}
EVAL_TEST_CASE("basic_inplace_string - capacity deduction");