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

#ifndef STRUCTURAL_TEST_BASIC_STATIC_STRING_STARTS_WITH_ENDS_WITH_HPP
#define STRUCTURAL_TEST_BASIC_STATIC_STRING_STARTS_WITH_ENDS_WITH_HPP

#include <bugspray/bugspray.hpp>

template<typename test_type>
ASSERTING_FUNCTION(test_basic_static_string_starts_with_ends_with,
                   (std::basic_string_view<typename test_type::value_type> data))
{
    using value_type = test_type::value_type;
    test_type                          ss(data.begin(), data.end());
    test_type                          begin_ss(data.begin(), data.begin() + 2);
    test_type                          end_ss(data.end() - 2, data.end());
    std::basic_string_view<value_type> total_sv{ss};
    std::basic_string_view<value_type> begin_sv{begin_ss};
    std::basic_string_view<value_type> end_sv{end_ss};

    SECTION("starts_with")
    {
        SECTION("string_view")
        {
            REQUIRE(ss.starts_with(total_sv));
            REQUIRE(ss.starts_with(begin_sv));
            REQUIRE(!ss.starts_with(end_sv));
        }
        SECTION("char")
        {
            REQUIRE(ss.starts_with(total_sv.front()));
            REQUIRE(ss.starts_with(begin_sv.front()));
            REQUIRE(!ss.starts_with(end_sv.front()));
        }
        SECTION("pointer")
        {
            REQUIRE(ss.starts_with(total_sv.data()));
            REQUIRE(ss.starts_with(begin_sv.data()));
            REQUIRE(!ss.starts_with(end_sv.data()));
        }
    }
    SECTION("ends_with")
    {
        SECTION("string_view")
        {
            REQUIRE(ss.ends_with(total_sv));
            REQUIRE(!ss.ends_with(begin_sv));
            REQUIRE(ss.ends_with(end_sv));
        }
        SECTION("char")
        {
            REQUIRE(ss.ends_with(total_sv.back()));
            REQUIRE(!ss.ends_with(begin_sv.back()));
            REQUIRE(ss.ends_with(end_sv.back()));
        }
        SECTION("pointer")
        {
            REQUIRE(ss.ends_with(total_sv.data()));
            REQUIRE(!ss.ends_with(begin_sv.data()));
            REQUIRE(ss.ends_with(end_sv.data()));
        }
    }
}

#endif // STRUCTURAL_TEST_BASIC_STATIC_STRING_STARTS_WITH_ENDS_WITH_HPP
