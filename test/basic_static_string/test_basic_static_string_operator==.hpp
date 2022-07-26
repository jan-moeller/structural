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

#ifndef TSTRUCTURAL_TEST_BASIC_STATIC_STRING_OPERATOR_EQUALS_HPP
#define TSTRUCTURAL_TEST_BASIC_STATIC_STRING_OPERATOR_EQUALS_HPP

#include <bugspray/bugspray.hpp>

template<typename test_type>
ASSERTING_FUNCTION(test_basic_static_string_operator_equals,
                   (std::basic_string_view<typename test_type::value_type> data))
{
    using value_type = test_type::value_type;
    SECTION("operator==")
    {
        test_type ss(data.begin(), data.end());
        SECTION("static_string == static_string")
        {
            test_type same = ss;
            test_type not_same(value_type('a'));
            CHECK(ss == same);
            CHECK(ss != not_same);
        }
        SECTION("static_string == pointer")
        {
            CHECK(ss == data.begin());
            CHECK(data.begin() == ss);
            CHECK(ss != (data.begin() + 1));
            CHECK((data.begin() + 1) != ss);
        }
        SECTION("static_string == string_view")
        {
            CHECK(ss == data);
            CHECK(data == ss);
            CHECK(ss != data.substr(1));
            CHECK(data.substr(1) != ss);
        }
    }
}

#endif // TSTRUCTURAL_TEST_BASIC_STATIC_STRING_OPERATOR_EQUALS_HPP
