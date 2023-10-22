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
#include "basic_inplace_string_test_data.hpp"
#include "structural/basic_inplace_string.hpp"

#include <bugspray/bugspray.hpp>

TEST_CASE("basic_inplace_string - write access", "[container][inplace_string]")
{
    using namespace structural;

    constexpr std::size_t C = 16;
    bs::static_for_each_type<char, wchar_t, char8_t, char16_t, char32_t>( //
        [&]<typename value_type>
        {
            using test_type = basic_inplace_string<value_type, C>;
            auto const data = basic_inplace_string_test_data_sv<value_type>;
            DYNAMIC_SECTION(bs::stringify_typename<test_type>())
            {
                test_type ss{data};
                SECTION("append")
                {
                    SECTION("from containers")
                    {
                        SECTION("inplace_string")
                        {
                            test_type ss2 = ss;
                            ss.append(ss2);
                        }
                        SECTION("pointer + count")
                        {
                            ss.append(data.data(), data.size());
                        }
                        SECTION("pointer")
                        {
                            ss.append(data.data());
                        }
                        SECTION("iterator pair")
                        {
                            ss.append(data.begin(), data.end());
                        }
                        SECTION("string_view")
                        {
                            ss.append(data);
                        }
                        CHECK(ss.starts_with(data));
                        CHECK(ss.ends_with(data));
                        CHECK(ss.size() == data.size() * 2);
                    }
                    SECTION("initializer_list")
                    {
                        ss.append({value_type('a'), value_type('b')});
                        CHECK(ss.starts_with(data));
                        CHECK(*(ss.end() - 1) == value_type('b'));
                        CHECK(*(ss.end() - 2) == value_type('a'));
                    }
                    SECTION("char")
                    {
                        ss.append(value_type('a'));
                        CHECK(ss.starts_with(data));
                        CHECK(ss.back() == value_type('a'));
                    }
                }
                SECTION("assign")
                {
                    test_type ss(data.begin(), data.end());
                    SECTION("basic_inplace_string copy")
                    {
                        test_type const ss2(data.begin() + 1, data.end());
                        ss.assign(ss2);
                        CHECK(ss.size() == ss2.size());
                        CHECK(ss == ss2);
                    }
                    SECTION("basic_inplace_string move")
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
                }
                SECTION("clear")
                {
                    test_type ss(data.begin(), data.end());
                    ss.clear();
                    CHECK(ss.size() == 0);
                }
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
                }
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
                        SECTION("iterator + inplace_string")
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
                        CHECK(std::basic_string_view{iter, data.size()}
                              == std::basic_string_view{data.data(), data.size()});
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
                        SECTION("iterator + inplace_string")
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
                        CHECK(std::basic_string_view{ss.begin(), iter}
                              == std::basic_string_view{data.data(), data.size()});
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
                        SECTION("iterator + inplace_string")
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
                }
                SECTION("operator+=")
                {
                    SECTION("from containers")
                    {
                        SECTION("inplace_string")
                        {
                            test_type ss2 = ss;
                            ss += ss2;
                        }
                        SECTION("pointer")
                        {
                            ss += data.data();
                        }
                        SECTION("string_view")
                        {
                            ss += data;
                        }
                        CHECK(ss.starts_with(data));
                        CHECK(ss.ends_with(data));
                        CHECK(ss.size() == data.size() * 2);
                    }
                    SECTION("initializer_list")
                    {
                        ss += {value_type('a'), value_type('b')};
                        CHECK(ss.starts_with(data));
                        CHECK(*(ss.end() - 1) == value_type('b'));
                        CHECK(*(ss.end() - 2) == value_type('a'));
                    }
                    SECTION("char")
                    {
                        ss += value_type('a');
                        CHECK(ss.starts_with(data));
                        CHECK(ss.back() == value_type('a'));
                    }
                }
                SECTION("push_back")
                {
                    ss.push_back(data[0]);
                    CHECK(ss.size() == data.size() + 1);
                    CHECK(ss.back() == data[0]);
                }
                SECTION("pop_back")
                {
                    ss.pop_back();
                    CHECK(ss.size() == data.size() - 1);
                    CHECK(ss.back() == *(data.end() - 2));
                }
                SECTION("replace")
                {
                    auto const first = ss.begin() + 1;
                    auto const last  = ss.end() - 1;
                    SECTION("begin, end, inplace_string")
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
                CHECK(*ss.end() == value_type('\0'));
            }
        });
}
EVAL_TEST_CASE("basic_inplace_string - write access");