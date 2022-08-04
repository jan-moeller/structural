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

TEST_CASE("static_vector - construction", "[container][static_vector]")
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
                        SECTION("default")
                        {
                            test_type sv;
                            CHECK(sv.empty());
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
                });
        });
}
EVAL_TEST_CASE("static_vector - construction");