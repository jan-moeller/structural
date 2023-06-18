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
#include "structural/static_vector.hpp"

#include <bugspray/bugspray.hpp>

TEST_CASE("static_vector - write access", "[container][static_vector]")
{
    using namespace structural;

    constexpr std::size_t C = 10;

    bs::static_for_each_type<int, bs::string>( //
        [&]<typename inner_value_type, auto I> {
            bs::static_for_each_type<static_vector<inner_value_type, C>,
                                     static_vector<static_vector<inner_value_type, C>, C>>( //
                [&]<typename test_type>
                {
                    using value_type = test_type::value_type;

                    DYNAMIC_SECTION(bs::stringify_typename<test_type>())
                    {
                        SECTION("assign")
                        {
                            test_type dest{value_type{}, value_type{}};
                            SECTION("initializer_list")
                            {
                                dest.assign({value_type{}, value_type{}, value_type{}});
                                CHECK(dest.size() == 3);
                            }
                            SECTION("iterators")
                            {
                                std::array const values{value_type{}, value_type{}, value_type{}};
                                dest.assign(values.begin(), values.end());
                                CHECK(dest.size() == 3);
                            }
                        }
                        SECTION("clear")
                        {
                            test_type sv{value_type{}, value_type{}, value_type{}};
                            sv.clear();
                            REQUIRE(sv.size() == 0);
                        }
                        SECTION("insert")
                        {
                            test_type sv;
                            SECTION("from const_reference")
                            {
                                value_type val{};
                                sv.insert(sv.begin(), val);
                                REQUIRE(sv.size() == 1);
                                sv.insert(sv.begin(), val);
                                REQUIRE(sv.size() == 2);
                                sv.insert(sv.end(), val);
                                REQUIRE(sv.size() == 3);
                                sv.insert(sv.begin() + 2, val);
                                REQUIRE(sv.size() == 4);
                            }
                            SECTION("from r-value reference")
                            {
                                sv.insert(sv.begin(), value_type{});
                                REQUIRE(sv.size() == 1);
                                sv.insert(sv.begin(), value_type{});
                                REQUIRE(sv.size() == 2);
                                sv.insert(sv.end(), value_type{});
                                REQUIRE(sv.size() == 3);
                                sv.insert(sv.begin() + 2, value_type{});
                                REQUIRE(sv.size() == 4);
                            }
                            SECTION("from range")
                            {
                                std::array vals{value_type{}, value_type{}};
                                sv.insert(sv.begin(), vals.begin(), vals.begin());
                                REQUIRE(sv.size() == 0);
                                sv.insert(sv.begin(), vals.begin(), vals.end());
                                REQUIRE(sv.size() == 2);
                                sv.insert(sv.begin(), vals.begin(), vals.end());
                                REQUIRE(sv.size() == 4);
                                sv.insert(sv.end(), vals.begin(), vals.end());
                                REQUIRE(sv.size() == 6);
                                sv.insert(sv.begin() + 2, vals.begin(), vals.end());
                                REQUIRE(sv.size() == 8);
                            }
                            SECTION("from initializer_list")
                            {
                                sv.insert(sv.begin(), std::initializer_list<value_type>{});
                                REQUIRE(sv.size() == 0);
                                sv.insert(sv.begin(), {value_type{}});
                                REQUIRE(sv.size() == 1);
                                sv.insert(sv.begin(), {value_type{}, value_type{}});
                                REQUIRE(sv.size() == 3);
                                sv.insert(sv.begin() + 1, {value_type{}, value_type{}});
                                REQUIRE(sv.size() == 5);
                                sv.insert(sv.end(), {value_type{}, value_type{}});
                                REQUIRE(sv.size() == 7);
                            }
                        }
                        SECTION("emplace")
                        {
                            test_type sv;
                            sv.emplace(sv.begin());
                            REQUIRE(sv.size() == 1);
                            sv.emplace(sv.begin());
                            REQUIRE(sv.size() == 2);
                            sv.emplace(sv.end());
                            REQUIRE(sv.size() == 3);
                            sv.emplace(sv.begin() + 1);
                            REQUIRE(sv.size() == 4);
                        }
                        SECTION("erase")
                        {
                            test_type  sv{value_type{}, value_type{}, value_type{}, value_type{}, value_type{}};
                            auto const old_size = sv.size();
                            SECTION("member")
                            {
                                SECTION("at iterator")
                                {
                                    sv.erase(sv.begin());
                                    REQUIRE(sv.size() == old_size - 1);
                                    sv.erase(sv.end() - 1);
                                    REQUIRE(sv.size() == old_size - 2);
                                    sv.erase(sv.begin() + 1);
                                    REQUIRE(sv.size() == old_size - 3);
                                }
                                SECTION("range")
                                {
                                    sv.erase(sv.begin(), sv.begin());
                                    REQUIRE(sv.size() == old_size);
                                    sv.erase(sv.begin(), sv.begin() + 1);
                                    REQUIRE(sv.size() == old_size - 1);
                                    sv.erase(sv.end() - 1, sv.end());
                                    REQUIRE(sv.size() == old_size - 2);
                                    sv.erase(sv.begin() + 1, sv.begin() + 3);
                                    REQUIRE(sv.size() == old_size - 4);
                                }
                            }
                            SECTION("free")
                            {
                                SECTION("value")
                                {
                                    REQUIRE(erase(sv, value_type{}) == old_size);
                                    REQUIRE(sv.size() == 0);
                                }
                                SECTION("predicate")
                                {
                                    REQUIRE(erase_if(sv,
                                                     [n = 0](value_type const& /*e*/) mutable
                                                     {
                                                         ++n;
                                                         return n < 3;
                                                     })
                                            == 3);
                                    REQUIRE(sv.size() == old_size - 3);
                                }
                            }
                        }
                        SECTION("emplace_back")
                        {
                            test_type sv;
                            sv.emplace_back();
                            REQUIRE(sv.size() == 1);
                        }
                        SECTION("pop_back")
                        {
                            test_type sv{value_type{}, value_type{}, value_type{}};
                            sv.pop_back();
                            REQUIRE(sv.size() == 2);
                        }
                    }
                });
        });
}
EVAL_TEST_CASE("static_vector - write access");