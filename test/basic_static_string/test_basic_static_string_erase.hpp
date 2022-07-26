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

#ifndef STRUCTURAL_TEST_BASIC_STATIC_STRING_ERASE_HPP
#define STRUCTURAL_TEST_BASIC_STATIC_STRING_ERASE_HPP

#include <bugspray/bugspray.hpp>

template<typename test_type>
ASSERTING_FUNCTION(test_basic_static_string_erase,
                   (std::basic_string_view<typename test_type::value_type> data, test_type& out))
{
    using value_type = test_type::value_type;
    SECTION("erase")
    {
        test_type ss(data.begin(), data.end());
        SECTION("at beginning of string")
        {
            auto iter = ss.begin();
            SECTION("iterator")
            {
                iter = ss.erase(ss.begin());
                CHECK(std::ranges::equal(iter, ss.end(), data.begin() + 1, data.end()));
                CHECK(ss.size() == data.size() - 1);
            }
            SECTION("begin + end")
            {
                iter = ss.erase(ss.begin(), ss.begin() + 2);
                CHECK(std::ranges::equal(iter, ss.end(), data.begin() + 2, data.end()));
                CHECK(ss.size() == data.size() - 2);
            }
            CHECK(iter == ss.begin());
        }
        SECTION("at end of string")
        {
            auto iter = ss.begin();
            SECTION("iterator")
            {
                iter = ss.erase(ss.end() - 1);
                CHECK(std::ranges::equal(ss.begin(), iter, data.begin(), data.end() - 1));
                CHECK(ss.size() == data.size() - 1);
            }
            SECTION("begin + end")
            {
                iter = ss.erase(ss.end() - 2, ss.end());
                CHECK(std::ranges::equal(ss.begin(), iter, data.begin(), data.end() - 2));
                CHECK(ss.size() == data.size() - 2);
            }
            CHECK(iter == ss.end());
        }
        SECTION("in the middle of string")
        {
            SECTION("iterator")
            {
                auto const iter = ss.erase(ss.begin() + 2);
                CHECK(std::ranges::equal(ss.begin(), iter, data.begin(), data.begin() + 2));
                CHECK(std::ranges::equal(iter, ss.end(), data.begin() + 3, data.end()));
                CHECK(ss.size() == data.size() - 1);
            }
            SECTION("begin + end")
            {
                auto const iter = ss.erase(ss.begin() + 1, ss.end() - 1);
                CHECK(std::ranges::equal(ss.begin(), iter, data.begin(), data.begin() + 1));
                CHECK(std::ranges::equal(iter, ss.end(), data.end() - 1, data.end()));
                CHECK(ss.size() == 2);
            }
        }
        SECTION("free erase functions")
        {
            auto const pred = [&](auto const& c)
            {
                return c == data.front();
            };
            SECTION("free erase")
            {
                erase(ss, data.front());
                CHECK(std::ranges::none_of(ss, pred));
            }
            SECTION("free erase_if")
            {
                erase_if(ss, pred);
                CHECK(std::ranges::none_of(ss, pred));
            }
        }
        out = ss;
    }
}

#endif // STRUCTURAL_TEST_BASIC_STATIC_STRING_ERASE_HPP
