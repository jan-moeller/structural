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

#ifndef STRUCTURAL_TEST_BASIC_STATIC_STRING_REPLACE_HPP
#define STRUCTURAL_TEST_BASIC_STATIC_STRING_REPLACE_HPP

#include <bugspray/bugspray.hpp>

template<typename test_type>
ASSERTING_FUNCTION(test_basic_static_string_replace,
                   (std::basic_string_view<typename test_type::value_type> data, test_type& out))
{
    using value_type = test_type::value_type;
    test_type  ss(data.begin(), data.end());
    auto const first = ss.begin() + 1;
    auto const last  = ss.end() - 1;
    SECTION("replace")
    {
        SECTION("begin, end, static_string")
        {
            test_type ss2 = ss;
            ss.replace(first, last, ss2);
        }
        SECTION("begin, end, begin2, end2")
        {
            ss.replace(first, last, data.begin(), data.end());
        }
        SECTION("begin, end, pointer, count")
        {
            ss.replace(first, last, data.data(), data.size());
        }
        SECTION("begin, end, pointer")
        {
            ss.replace(first, last, data.data());
        }
        SECTION("begin, end, string_view")
        {
            ss.replace(first, last, data);
        }
        CAPTURE(ss);
        CHECK(ss.starts_with(std::basic_string_view<value_type>{data.begin(), 1}));
        CHECK(ss.ends_with(std::basic_string_view<value_type>{data.end() - 1, 1}));
        CHECK(std::ranges::equal(ss.begin() + 1, ss.end() - 1, data.begin(), data.end()));
    }
    out = ss;
}

#endif // STRUCTURAL_TEST_BASIC_STATIC_STRING_REPLACE_HPP
