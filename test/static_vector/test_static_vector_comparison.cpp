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

TEST_CASE("static_vector - comparison", "[container][static_vector]")
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
EVAL_TEST_CASE("static_vector - comparison");