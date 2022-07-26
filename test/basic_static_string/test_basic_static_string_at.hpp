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

#ifndef STRUCTURAL_TEST_BASIC_STATIC_STRING_AT_HPP
#define STRUCTURAL_TEST_BASIC_STATIC_STRING_AT_HPP

#include <bugspray/bugspray.hpp>

template<typename test_type>
ASSERTING_FUNCTION(test_basic_static_string_at, (std::basic_string_view<typename test_type::value_type> data))
{
    using value_type = test_type::value_type;
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
}

#endif // STRUCTURAL_TEST_BASIC_STATIC_STRING_AT_HPP
