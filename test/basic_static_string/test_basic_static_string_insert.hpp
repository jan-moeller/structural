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

#ifndef STRUCTURAL_TEST_BASIC_STATIC_STRING_INSERT_HPP
#define STRUCTURAL_TEST_BASIC_STATIC_STRING_INSERT_HPP

#include <bugspray/bugspray.hpp>

template<typename test_type>
ASSERTING_FUNCTION(test_basic_static_string_insert,
                   (std::basic_string_view<typename test_type::value_type> data, test_type& out))
{
    using value_type = test_type::value_type;
    SECTION("insert")
    {
        test_type ss(data.begin(), data.end());
        SECTION("at beginning of string")
        {
            auto iter = ss.begin();
            SECTION("iterator + char")
            {
                iter = ss.insert(ss.begin(), data[0]);
                CHECK(ss.size() == data.size() + 1);
                CHECK(ss.front() == data[0]);
                iter += 1;
            }
            SECTION("iterator + pointer")
            {
                iter = ss.insert(ss.begin(), data.data());
                CHECK(ss.starts_with(data.data()));
                CHECK(std::ranges::equal(iter, iter + data.size(), data.begin(), data.end()));
                iter += data.size();
            }
            SECTION("iterator + pointer + count")
            {
                iter = ss.insert(ss.begin(), data.data(), data.size());
                CHECK(ss.starts_with(data.data()));
                CHECK(std::ranges::equal(iter, iter + data.size(), data.begin(), data.end()));
                iter += data.size();
            }
            SECTION("iterator + static_string")
            {
                iter = ss.insert(ss.begin(), ss);
                CHECK(ss.starts_with(data.data()));
                CHECK(std::ranges::equal(iter, iter + data.size(), data.begin(), data.end()));
                iter += data.size();
            }
            SECTION("iterator + string_view")
            {
                iter = ss.insert(ss.begin(), std::basic_string_view{data.data(), data.size()});
                CHECK(ss.starts_with(data.data()));
                CHECK(std::ranges::equal(iter, iter + data.size(), data.begin(), data.end()));
                iter += data.size();
            }
            SECTION("iterator + iterator pair")
            {
                iter = ss.insert(ss.begin(), data.begin(), data.end());
                CHECK(ss.starts_with(data.data()));
                CHECK(std::ranges::equal(iter, iter + data.size(), data.begin(), data.end()));
                iter += data.size();
            }
            SECTION("iterator + initializer_list")
            {
                iter = ss.insert(ss.begin(), {value_type('v'), value_type('b')});
                CHECK(ss[0] == value_type('v'));
                CHECK(ss[1] == value_type('b'));
                iter += 2;
            }
            REQUIRE((ss.end() - iter) == data.size());
            CHECK(std::basic_string_view{iter, data.size()} == std::basic_string_view{data.data(), data.size()});
        }
        SECTION("at end of string")
        {
            auto iter = ss.end();
            SECTION("iterator + char")
            {
                iter = ss.insert(ss.end(), data[0]);
                CHECK(ss.size() == data.size() + 1);
                CHECK(ss.back() == data[0]);
            }
            SECTION("iterator + pointer")
            {
                iter = ss.insert(ss.end(), data.data());
                CHECK(ss.ends_with(data.data()));
            }
            SECTION("iterator + pointer + count")
            {
                iter = ss.insert(ss.end(), data.data(), data.size());
                CHECK(ss.ends_with(std::basic_string_view(data.data(), data.size())));
            }
            SECTION("iterator + static_string")
            {
                iter = ss.insert(ss.end(), ss);
                CHECK(ss.ends_with(data));
            }
            SECTION("iterator + string_view")
            {
                iter = ss.insert(ss.end(), data);
                CHECK(ss.ends_with(data));
            }
            SECTION("iterator + iterator pair")
            {
                iter = ss.insert(ss.end(), data.begin(), data.end());
                CHECK(ss.ends_with(data));
            }
            SECTION("iterator + initializer_list")
            {
                iter = ss.insert(ss.end(), {value_type('v'), value_type('b')});
                CHECK(ss[ss.size() - 2] == value_type('v'));
                CHECK(ss[ss.size() - 1] == value_type('b'));
            }
            REQUIRE((iter - ss.begin()) == data.size());
            CHECK(std::basic_string_view{ss.begin(), iter} == std::basic_string_view{data.data(), data.size()});
        }
        SECTION("in the middle of string")
        {
            auto const insertiter = ss.begin() + ss.size() / 2;
            auto       iter       = insertiter;
            SECTION("iterator + char")
            {
                iter = ss.insert(insertiter, data[0]);
                CHECK(ss.size() == data.size() + 1);
                CHECK(*insertiter == data[0]);
                iter += 1;
            }
            SECTION("iterator + pointer")
            {
                iter = ss.insert(insertiter, data.data());
                CHECK(std::basic_string_view(insertiter, data.size())
                      == std::basic_string_view(data.data(), data.size()));
                iter += data.size();
            }
            SECTION("iterator + pointer + count")
            {
                iter = ss.insert(insertiter, data.data(), data.size());
                CHECK(std::basic_string_view(insertiter, data.size())
                      == std::basic_string_view(data.data(), data.size()));
                iter += data.size();
            }
            SECTION("iterator + static_string")
            {
                iter = ss.insert(insertiter, ss);
                CHECK(std::basic_string_view(insertiter, data.size())
                      == std::basic_string_view(data.data(), data.size()));
                iter += data.size();
            }
            SECTION("iterator + string_view")
            {
                iter = ss.insert(insertiter, std::basic_string_view{data.data(), data.size()});
                CHECK(std::basic_string_view(insertiter, data.size())
                      == std::basic_string_view(data.data(), data.size()));
                iter += data.size();
            }
            SECTION("iterator + iterator pair")
            {
                iter = ss.insert(insertiter, data.begin(), data.end());
                CHECK(std::basic_string_view(insertiter, data.size())
                      == std::basic_string_view(data.data(), data.size()));
                iter += data.size();
            }
            SECTION("iterator + initializer_list")
            {
                iter = ss.insert(insertiter, {value_type('v'), value_type('b')});
                CHECK(*(insertiter) == value_type('v'));
                CHECK(*(insertiter + 1) == value_type('b'));
                iter += 2;
            }
            auto const             leading_count  = insertiter - ss.begin();
            auto const             trailing_count = ss.end() - iter;
            std::basic_string_view leading_sv{ss.begin(), insertiter};
            std::basic_string_view trailing_sv{iter, ss.end()};
            CAPTURE(ss, leading_count, trailing_count, leading_sv, trailing_sv);
            REQUIRE((leading_count + trailing_count) == data.size());
            CHECK(leading_sv == std::basic_string_view(data.data(), leading_count));
            CHECK(trailing_sv == std::basic_string_view(data.data() + leading_count, trailing_count));
        }
        out = ss;
    }
}

#endif // STRUCTURAL_TEST_BASIC_STATIC_STRING_INSERT_HPP
