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
#include "structural/inplace_vector.hpp"

#include <bugspray/bugspray.hpp>

TEST_CASE("inplace_vector - read access", "[container][inplace_vector]")
{
    using namespace structural;

    constexpr std::size_t C = 10;

    bs::static_for_each_type<int, bs::string>( //
        [&]<typename inner_value_type, auto I> {
            bs::static_for_each_type<inplace_vector<inner_value_type, C>,
                                     inplace_vector<inplace_vector<inner_value_type, C>, C>>( //
                [&]<typename test_type>
                {
                    using value_type = test_type::value_type;

                    DYNAMIC_SECTION(bs::stringify_typename<test_type>())
                    {
                        SECTION("at", runtime)
                        {
                            bs::static_for_each_type<test_type,
                                                     test_type const>( //
                                [&]<typename cur_test_type>
                                {
                                    CAPTURE(bs::stringify_typename<cur_test_type>());
                                    cur_test_type dest{value_type{}, value_type{}};
                                    REQUIRE_NOTHROW(dest.at(0));
                                    REQUIRE(dest.at(0) == value_type{});
                                    REQUIRE_THROWS_AS(std::invalid_argument, dest.at(2));
                                });
                        }
                        SECTION("operator[]")
                        {
                            bs::static_for_each_type<test_type,
                                                     test_type const>( //
                                [&]<typename cur_test_type>
                                {
                                    CAPTURE(bs::stringify_typename<cur_test_type>());
                                    cur_test_type dest{value_type{}, value_type{}};
                                    REQUIRE(dest[0] == value_type{});
                                });
                        }
                        SECTION("front")
                        {
                            bs::static_for_each_type<test_type,
                                                     test_type const>( //
                                [&]<typename cur_test_type>
                                {
                                    CAPTURE(bs::stringify_typename<cur_test_type>());
                                    cur_test_type dest{value_type{}, value_type{}, value_type{}};
                                    REQUIRE(dest.front() == value_type{});
                                });
                        }
                        SECTION("back")
                        {
                            bs::static_for_each_type<test_type,
                                                     test_type const>( //
                                [&]<typename cur_test_type>
                                {
                                    CAPTURE(bs::stringify_typename<cur_test_type>());
                                    cur_test_type dest{value_type{}, value_type{}};
                                    REQUIRE(dest.back() == value_type{});
                                });
                        }
                        SECTION("forward iteration")
                        {
                            bs::static_for_each_type<test_type,
                                                     test_type const>( //
                                [&]<typename cur_test_type>
                                {
                                    CAPTURE(bs::stringify_typename<cur_test_type>());
                                    cur_test_type dest{value_type{}, value_type{}};
                                    std::size_t   counter = 0;
                                    for (auto iter = dest.begin(); iter != dest.end(); ++iter)
                                    {
                                        REQUIRE(*iter == value_type{});
                                        ++counter;
                                    }
                                    REQUIRE(counter == dest.size());
                                });
                        }
                        SECTION("reverse iteration")
                        {
                            bs::static_for_each_type<test_type,
                                                     test_type const>( //
                                [&]<typename cur_test_type>
                                {
                                    CAPTURE(bs::stringify_typename<cur_test_type>());
                                    cur_test_type dest{value_type{}, value_type{}};
                                    std::size_t   counter = 0;
                                    for (auto iter = dest.rbegin(); iter != dest.rend(); ++iter)
                                    {
                                        REQUIRE(*iter == value_type{});
                                        ++counter;
                                    }
                                    REQUIRE(counter == dest.size());
                                });
                        }
                        SECTION("empty")
                        {
                            REQUIRE(test_type{}.empty());
                        }
                        SECTION("size")
                        {
                            REQUIRE(test_type{}.size() == 0);
                            REQUIRE(test_type{value_type{}}.size() == 1);
                        }
                        SECTION("capacity")
                        {
                            REQUIRE(test_type{}.capacity() == C);
                        }
                    }
                });
        });
}
EVAL_TEST_CASE("inplace_vector - read access");