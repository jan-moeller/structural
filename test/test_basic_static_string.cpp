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
#include "structural/basic_static_string.hpp"

#include <bugspray/bugspray.hpp>
#include <bugspray/utility/static_for_each_type.hpp>
#include <bugspray/utility/stringify_typename.hpp>

#include <algorithm>

using namespace structural;

#include "basic_static_string/test_basic_static_string_append.hpp"
#include "basic_static_string/test_basic_static_string_assign.hpp"
#include "basic_static_string/test_basic_static_string_at.hpp"
#include "basic_static_string/test_basic_static_string_c_str.hpp"
#include "basic_static_string/test_basic_static_string_clear.hpp"
#include "basic_static_string/test_basic_static_string_compare.hpp"
#include "basic_static_string/test_basic_static_string_constructors.hpp"
#include "basic_static_string/test_basic_static_string_conversion_string_view.hpp"
#include "basic_static_string/test_basic_static_string_empty_size.hpp"
#include "basic_static_string/test_basic_static_string_erase.hpp"
#include "basic_static_string/test_basic_static_string_front_back.hpp"
#include "basic_static_string/test_basic_static_string_hash.hpp"
#include "basic_static_string/test_basic_static_string_insert.hpp"
#include "basic_static_string/test_basic_static_string_iterators.hpp"
#include "basic_static_string/test_basic_static_string_operator+.hpp"
#include "basic_static_string/test_basic_static_string_operator+=.hpp"
#include "basic_static_string/test_basic_static_string_operator==.hpp"
#include "basic_static_string/test_basic_static_string_operator[].hpp"
#include "basic_static_string/test_basic_static_string_push_pop_back.hpp"
#include "basic_static_string/test_basic_static_string_replace.hpp"
#include "basic_static_string/test_basic_static_string_starts_with_ends_with.hpp"
#include "basic_static_string/test_basic_static_string_substr.hpp"

#include "basic_static_string/test_basic_static_string_operator<=>.hpp"

TEST_CASE("basic_static_string", "[container]")
{
    using namespace bs;

    SECTION("deduce capacity from literal")
    {
        SECTION("basic_static_string")
        {
            basic_static_string ss = "foobar";
            CHECK(ss.capacity() == 7);
            CHECK(ss.size() == 6);
            CHECK(ss == "foobar");
            CHECK(*ss.end() == '\0');
        }
        SECTION("static_string")
        {
            static_string ss = "foobar";
            CHECK(ss.capacity() == 7);
            CHECK(ss.size() == 6);
            CHECK(ss == "foobar");
            CHECK(*ss.end() == '\0');
        }
        SECTION("static_wstring")
        {
            static_wstring ss = L"foobar";
            CHECK(ss.capacity() == 7);
            CHECK(ss.size() == 6);
            CHECK(ss == L"foobar");
            CHECK(*ss.end() == '\0');
        }
        SECTION("static_u8string")
        {
            static_u8string ss = u8"foobar";
            CHECK(ss.capacity() == 7);
            CHECK(ss.size() == 6);
            CHECK(ss == u8"foobar");
            CHECK(*ss.end() == '\0');
        }
        SECTION("static_u16string")
        {
            static_u16string ss = u"foobar";
            CHECK(ss.capacity() == 7);
            CHECK(ss.size() == 6);
            CHECK(ss == u"foobar");
            CHECK(*ss.end() == '\0');
        }
        SECTION("static_u32string")
        {
            static_u32string ss = U"foobar";
            CHECK(ss.capacity() == 7);
            CHECK(ss.size() == 6);
            CHECK(ss == U"foobar");
            CHECK(*ss.end() == '\0');
        }
    }

    constexpr std::size_t test_capacity = 16;
    static_for_each_type<char, wchar_t, char8_t, char16_t, char32_t>( //
        [&]<typename value_type>
        {
            using test_type = basic_static_string<value_type, test_capacity>;
            DYNAMIC_SECTION(stringify_typename<test_type>())
            {
                SECTION("structurality", only_if(structural_type<value_type>))
                {
                    CHECK(structural_type<test_type>);
                }

                std::array<value_type, 5> const data{value_type('f'),
                                                     value_type('o'),
                                                     value_type('o'),
                                                     value_type('b'),
                                                     value_type('\0')};
                std::basic_string_view const    data_sv(data.data());
                CAPTURE(data_sv);

                test_type ss;

                CALL(test_basic_static_string_constructors<test_type>, data_sv, ss);
                CALL(test_basic_static_string_assign<test_type>, data_sv, ss);
                CALL(test_basic_static_string_at<test_type>, data_sv);
                CALL(test_basic_static_string_operator_square_brackets<test_type>, data_sv);
                CALL(test_basic_static_string_front_back<test_type>, data_sv);
                CALL(test_basic_static_string_c_str<test_type>, data_sv);
                CALL(test_basic_static_string_conversion_string_view<test_type>, data_sv);
                CALL(test_basic_static_string_iterators<test_type>, data_sv);
                CALL(test_basic_static_string_empty_size<test_type>, data_sv);
                CALL(test_basic_static_string_clear<test_type>, data_sv, ss);
                CALL(test_basic_static_string_insert<test_type>, data_sv, ss);
                CALL(test_basic_static_string_erase<test_type>, data_sv, ss);
                CALL(test_basic_static_string_push_pop_back<test_type>, data_sv, ss);
                CALL(test_basic_static_string_append<test_type>, data_sv, ss);
                CALL(test_basic_static_string_operator_plus_equals<test_type>, data_sv, ss);
                CALL(test_basic_static_string_compare<test_type>, data_sv);
                CALL(test_basic_static_string_starts_with_ends_with<test_type>, data_sv);
                CALL(test_basic_static_string_replace<test_type>, data_sv, ss);
                CALL(test_basic_static_string_substr<test_type>, data_sv);
                CALL(test_basic_static_string_operator_plus<test_type>, data_sv, ss);
                CALL(test_basic_static_string_operator_equals<test_type>, data_sv);
                CALL(test_basic_static_string_operator_spaceship<test_type>, data_sv);
                CALL(test_basic_static_string_hash<test_type>, data_sv);

                CHECK(*ss.end() == value_type('\0'));
            }
        });
}
EVAL_TEST_CASE("basic_static_string");