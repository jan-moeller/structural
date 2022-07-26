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

#ifndef STRUCTURAL_TEST_BASIC_STATIC_STRING_OPERATOR_PLUS_HPP
#define STRUCTURAL_TEST_BASIC_STATIC_STRING_OPERATOR_PLUS_HPP

#include <bugspray/bugspray.hpp>

template<typename test_type>
ASSERTING_FUNCTION(test_basic_static_string_operator_plus,
                   (std::basic_string_view<typename test_type::value_type> data, test_type& out))
{
    using value_type = test_type::value_type;
    test_type ss(data.begin(), data.end());
    SECTION("operator+")
    {
        SECTION("static_string + static_string")
        {
            ss = ss + ss;
            CAPTURE(ss);
            CHECK(ss.starts_with(data));
            CHECK(ss.ends_with(data));
            CHECK(ss.size() == data.size() * 2);
        }
        SECTION("static_string + char")
        {
            ss = ss + ss.front();
            CAPTURE(ss);
            CHECK(ss.starts_with(data));
            CHECK(ss.ends_with(ss.front()));
            CHECK(ss.size() == data.size() + 1);
        }
        SECTION("char + static_string")
        {
            ss = ss.front() + ss;
            CAPTURE(ss);
            CHECK(ss.starts_with(ss.front()));
            CHECK(ss.ends_with(ss));
            CHECK(ss.size() == data.size() + 1);
        }
    }
    out = ss;
}

#endif // STRUCTURAL_TEST_BASIC_STATIC_STRING_OPERATOR_PLUS_HPP
