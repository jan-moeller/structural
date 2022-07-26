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

#ifndef STRUCTURAL_TEST_BASIC_STATIC_STRING_ASSIGN_HPP
#define STRUCTURAL_TEST_BASIC_STATIC_STRING_ASSIGN_HPP

#include <bugspray/bugspray.hpp>

template<typename test_type>
ASSERTING_FUNCTION(test_basic_static_string_assign,
                   (std::basic_string_view<typename test_type::value_type> data, test_type& out))
{
    using value_type = test_type::value_type;
    SECTION("assign")
    {
        test_type ss(data.begin(), data.end());
        SECTION("basic_static_string copy")
        {
            test_type const ss2(data.begin() + 1, data.end());
            ss.assign(ss2);
            CHECK(ss.size() == ss2.size());
            CHECK(ss == ss2);
        }
        SECTION("basic_static_string move")
        {
            test_type       ss2(data.begin() + 1, data.end());
            test_type const ss2_copy(data.begin() + 1, data.end());
            ss.assign(std::move(ss2));
            CHECK(ss.size() == ss2_copy.size());
            CHECK(ss == ss2_copy);
        }
        SECTION("pointer + size")
        {
            ss.assign(data.data(), data.size());
            CHECK(ss.size() == data.size());
            CHECK(std::ranges::equal(ss, data));
        }
        SECTION("pointer")
        {
            ss.assign(data.data());
            CHECK(ss.size() == data.size());
            CHECK(std::ranges::equal(ss.begin(), ss.end(), data.begin(), data.end()));
        }
        SECTION("iterators")
        {
            ss.assign(data.begin() + 1, data.end() - 1);
            CHECK(ss.size() == data.size() - 2);
            CHECK(std::ranges::equal(ss.begin(), ss.end(), data.begin() + 1, data.end() - 1));
        }
        SECTION("initializer_list")
        {
            ss.assign({value_type('a'), value_type('b')});
            CHECK(ss.size() == 2);
            CHECK(ss[0] == value_type('a'));
            CHECK(ss[1] == value_type('b'));
        }
        SECTION("string_view")
        {
            ss.assign(data);
            CHECK(ss.size() == data.size());
            CHECK(std::ranges::equal(ss, data));
        }
        out = ss;
    }
}

#endif // STRUCTURAL_TEST_BASIC_STATIC_STRING_ASSIGN_HPP
