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
#include "concept_structural_type_value.hpp"
#include "structural/static_vector.hpp"

#include <bugspray/bugspray.hpp>
#include <bugspray/utility/static_for_each_type.hpp>
#include <bugspray/utility/stringify_typename.hpp>

#include <algorithm>
#include <array>

using namespace structural;

TEST_CASE("static_vector", "[container]")
{
    using namespace bs;

    constexpr std::size_t test_capacity = 10;
    static_for_each_type<int, bs::string>( //
        [&]<typename inner_value_type, auto I> {
            static_for_each_type<static_vector<inner_value_type, test_capacity>,
                                 static_vector<static_vector<inner_value_type, test_capacity>, test_capacity>>( //
                [&]<typename test_type, auto J> {
                    using value_type = test_type::value_type;

                    DYNAMIC_SECTION(stringify_typename<test_type>())
                    {
                        SECTION("structurality", only_if(structural_type<inner_value_type>))
                        {
                            CHECK(structural_type<test_type>);
                        }

                        SECTION("construction")
                        {
                            SECTION("default")
                            {
                                test_type sv;
                                CHECK(sv.empty());
                            }
                            SECTION("count")
                            {
                                test_type sv(3);
                                CHECK(sv.size() == 3);
                            }
                            SECTION("count, value")
                            {
                                test_type sv(3, value_type{});
                                CHECK(sv.size() == 3);
                            }
                            SECTION("range")
                            {
                                test_type data{value_type{}, value_type{}, value_type{}};
                                test_type sv{data.begin(), data.end()};
                                CHECK(sv.size() == 3);
                                CHECK(std::ranges::equal(sv.begin(), sv.end(), data.begin(), data.end()));
                            }
                            SECTION("initializer_list")
                            {
                                test_type sv{value_type{}, value_type{}, value_type{}};
                                CHECK(sv.size() == 3);
                                CHECK(std::ranges::all_of(sv, [](value_type const& e) { return e == value_type{}; }));
                            }
                            SECTION("copy")
                            {
                                test_type sv{value_type{}, value_type{}, value_type{}};
                                test_type copy = sv;
                                CHECK(sv == copy);
                            }
                            SECTION("move")
                            {
                                test_type       sv{value_type{}, value_type{}, value_type{}};
                                test_type const copy  = sv;
                                test_type       moved = std::move(sv);
                                CHECK(moved == copy);
                            }
                        }
                        SECTION("assignment")
                        {
                            test_type const sv{value_type{}, value_type{}, value_type{}};
                            SECTION("copy")
                            {
                                test_type dest{value_type{}, value_type{}};
                                dest = sv;
                                CHECK(dest == sv);
                            }
                            SECTION("move")
                            {
                                auto      copy = sv;
                                test_type dest{value_type{}, value_type{}, value_type{}};
                                dest = std::move(copy);
                                CHECK(dest == sv);
                            }
                            SECTION("initializer_list")
                            {
                                test_type dest{value_type{}, value_type{}, value_type{}};
                                dest = {value_type{}, value_type{}};
                                CAPTURE(dest);
                                CHECK(dest.size() == 2);
                            }
                        }
                        SECTION("assign")
                        {
                            test_type dest{value_type{}, value_type{}};
                            SECTION("count, value")
                            {
                                dest.assign(3, value_type{});
                                CHECK(dest.size() == 3);
                            }
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
                        SECTION("at", runtime)
                        {
                            static_for_each_type<test_type,
                                                 test_type const>( //
                                [&]<typename cur_test_type>
                                {
                                    CAPTURE(stringify_typename<cur_test_type>());
                                    cur_test_type dest{value_type{}, value_type{}};
                                    REQUIRE_NOTHROW(dest.at(0));
                                    REQUIRE(dest.at(0) == value_type{});
                                    REQUIRE_THROWS_AS(std::invalid_argument, dest.at(2));
                                });
                        }
                        SECTION("operator[]")
                        {
                            static_for_each_type<test_type,
                                                 test_type const>( //
                                [&]<typename cur_test_type>
                                {
                                    CAPTURE(stringify_typename<cur_test_type>());
                                    cur_test_type dest{value_type{}, value_type{}};
                                    REQUIRE(dest[0] == value_type{});
                                });
                        }
                        SECTION("front")
                        {
                            static_for_each_type<test_type,
                                                 test_type const>( //
                                [&]<typename cur_test_type>
                                {
                                    CAPTURE(stringify_typename<cur_test_type>());
                                    cur_test_type dest{value_type{}, value_type{}, value_type{}};
                                    REQUIRE(dest.front() == value_type{});
                                });
                        }
                        SECTION("back")
                        {
                            static_for_each_type<test_type,
                                                 test_type const>( //
                                [&]<typename cur_test_type>
                                {
                                    CAPTURE(stringify_typename<cur_test_type>());
                                    cur_test_type dest{value_type{}, value_type{}};
                                    REQUIRE(dest.back() == value_type{});
                                });
                        }
                        SECTION("forward iteration")
                        {
                            static_for_each_type<test_type,
                                                 test_type const>( //
                                [&]<typename cur_test_type>
                                {
                                    CAPTURE(stringify_typename<cur_test_type>());
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
                            static_for_each_type<test_type,
                                                 test_type const>( //
                                [&]<typename cur_test_type>
                                {
                                    CAPTURE(stringify_typename<cur_test_type>());
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
                        SECTION("max_size")
                        {
                            REQUIRE(test_type{}.max_size() >= test_type{}.capacity());
                        }
                        SECTION("capacity")
                        {
                            REQUIRE(test_type{}.capacity() == test_capacity);
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
                            SECTION("from n const_references")
                            {
                                value_type val{};
                                sv.insert(sv.begin(), 0, val);
                                REQUIRE(sv.size() == 0);
                                sv.insert(sv.begin(), 2, val);
                                REQUIRE(sv.size() == 2);
                                sv.insert(sv.begin(), 2, val);
                                REQUIRE(sv.size() == 4);
                                sv.insert(sv.end(), 2, val);
                                REQUIRE(sv.size() == 6);
                                sv.insert(sv.begin() + 2, 2, val);
                                REQUIRE(sv.size() == 8);
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
                                                     [n = 0](value_type const& e) mutable
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
                        SECTION("swap")
                        {
                            SECTION("member")
                            {
                                test_type sv1{value_type{}, value_type{}, value_type{}};
                                test_type sv2{value_type{}, value_type{}};
                                sv1.swap(sv2);
                                REQUIRE(sv1.size() == 2);
                                REQUIRE(sv2.size() == 3);
                            }
                            SECTION("free")
                            {
                                test_type sv1{value_type{}, value_type{}, value_type{}};
                                test_type sv2{value_type{}, value_type{}};
                                swap(sv1, sv2);
                                REQUIRE(sv1.size() == 2);
                                REQUIRE(sv2.size() == 3);
                            }
                        }
                        SECTION("equality")
                        {
                            test_type sv1{value_type{}, value_type{}, value_type{}};
                            test_type sv2{value_type{}, value_type{}};
                            REQUIRE(sv1 != sv2);
                            REQUIRE(sv1 == sv1);
                        }
                        SECTION("spaceship")
                        {
                            test_type sv1{value_type{}, value_type{}, value_type{}};
                            test_type sv2{value_type{}, value_type{}};
                            REQUIRE((sv1 <=> sv2) == std::strong_ordering::greater);
                            REQUIRE((sv2 <=> sv1) == std::strong_ordering::less);
                            REQUIRE((sv1 <=> sv1) == std::strong_ordering::equal);
                        }
                    }
                });
        });
}
EVAL_TEST_CASE("static_vector");
