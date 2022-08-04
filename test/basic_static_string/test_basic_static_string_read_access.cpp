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
#include "basic_static_string_test_data.hpp"
#include "structural/basic_static_string.hpp"

#include <bugspray/bugspray.hpp>

TEST_CASE("basic_static_string - read access", "[container][static_string]")
{
    using namespace structural;

    constexpr std::size_t C = 16;
    bs::static_for_each_type<char, wchar_t, char8_t, char16_t, char32_t>( //
        [&]<typename value_type>
        {
            using test_type = basic_static_string<value_type, C>;
            auto const data = basic_static_string_test_data_sv<value_type>;
            DYNAMIC_SECTION(bs::stringify_typename<test_type>())
            {
                test_type ss{data};
                SECTION("at")
                {
                    bs::static_for_each_type<test_type, test_type const>( //
                        [&]<typename T>
                        {
                            CAPTURE(bs::stringify_typename<T>());
                            T ss(data.begin(), data.end() - 1);
                            CHECK(ss.at(0) == data[0]);
                            CHECK(ss.at(1) == data[1]);
                            SECTION("out of bounds", runtime)
                            {
                                CHECK_THROWS_AS(std::invalid_argument, ss.at(ss.size()));
                            }
                        });
                }
                SECTION("operator[]")
                {
                    bs::static_for_each_type<test_type, test_type const>( //
                        [&]<typename T>
                        {
                            CAPTURE(bs::stringify_typename<T>());
                            T ss(data.begin(), data.end() - 1);
                            CHECK(ss[0] == data[0]);
                            CHECK(ss[1] == data[1]);
                        });
                }
                SECTION("c_str")
                {
                    test_type ss(data.begin(), data.end());
                    CHECK(std::basic_string_view(ss.c_str()) == data.begin());
                }
                SECTION("compare")
                {
                    test_type ss(data.begin(), data.end());
                    test_type same = ss;
                    test_type lesser({value_type('a')});
                    test_type greater = ss;
                    greater += value_type('a');
                    SECTION("static_string")
                    {
                        CHECK(ss.compare(same) == 0);
                        CHECK(ss.compare(lesser) > 0);
                        CHECK(ss.compare(greater) < 0);
                    }
                    SECTION("pointer")
                    {
                        CHECK(ss.compare(same.data()) == 0);
                        CHECK(ss.compare(lesser.data()) > 0);
                        CHECK(ss.compare(greater.data()) < 0);
                    }
                    SECTION("string_view")
                    {
                        CHECK(ss.compare(std::basic_string_view<value_type>{same}) == 0);
                        CHECK(ss.compare(std::basic_string_view<value_type>{lesser}) > 0);
                        CHECK(ss.compare(std::basic_string_view<value_type>{greater}) < 0);
                    }
                }
                SECTION("conversion to string_view")
                {
                    test_type                          ss(data.begin(), data.end() - 1);
                    std::basic_string_view<value_type> sv = ss;
                    CHECK(ss == sv);
                }
                SECTION("empty & size")
                {
                    test_type ss(data.begin(), data.end());
                    CHECK(!ss.empty());
                    CHECK(ss.size() == data.size());
                    test_type ss2;
                    CHECK(ss2.empty());
                    CHECK(ss2.size() == 0);
                }
                SECTION("front & back")
                {
                    bs::static_for_each_type<test_type, test_type const>( //
                        [&]<typename T>
                        {
                            CAPTURE(bs::stringify_typename<T>());
                            T ss(data.begin(), data.end() - 1);
                            CHECK(ss.front() == *data.begin());
                            CHECK(ss.front() == *ss.begin());
                            CHECK(ss.back() == *(data.end() - 2));
                            CHECK(ss.back() == *(ss.end() - 1));
                        });
                }
                SECTION("hash", runtime)
                {
                    test_type  ss{data};
                    auto const h = std::hash<test_type>{}(ss);
                    CHECK(h == std::hash<std::basic_string_view<typename test_type::value_type>>{}(data));
                }
                SECTION("iterators")
                {
                    test_type ss(data.begin(), data.end());
                    CHECK(std::ranges::equal(ss.begin(), ss.end(), data.begin(), data.end()));
                    CHECK(std::ranges::equal(ss.cbegin(), ss.cend(), data.cbegin(), data.cend()));
                    CHECK(std::ranges::equal(ss.rbegin(), ss.rend(), data.rbegin(), data.rend()));
                    CHECK(std::ranges::equal(ss.crbegin(), ss.crend(), data.crbegin(), data.crend()));
                }
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
            }
            SECTION("begins_with & ends_with")
            {
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
            SECTION("substr")
            {
                test_type ss{data.begin(), data.end()};
                CHECK(ss.substr() == ss);
                CHECK(ss.substr(0, 0).empty());
                CHECK(ss.substr(1, ss.size() - 2)
                      == std::basic_string_view<value_type>{data.begin() + 1, data.end() - 1});
            }
        });
}
EVAL_TEST_CASE("basic_static_string - read access");