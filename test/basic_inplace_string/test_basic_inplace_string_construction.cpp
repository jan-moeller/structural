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

TEST_CASE("basic_inplace_string - construction", "[container][inplace_string]")
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
                SECTION("default")
                {
                    test_type ss;
                    CHECK(ss.size() == 0);
                    CHECK(*ss.end() == value_type('\0'));
                }
                SECTION("pointer + size")
                {
                    test_type ss(data.data(), data.size());
                    CHECK(ss.size() == data.size());
                    CHECK(std::ranges::equal(ss, data));
                    CHECK(*ss.end() == value_type('\0'));
                }
                SECTION("pointer")
                {
                    test_type ss(data.data());
                    CHECK(ss.size() == data.size());
                    CHECK(std::ranges::equal(ss.begin(), ss.end(), data.begin(), data.end()));
                    CHECK(*ss.end() == value_type('\0'));
                }
                SECTION("iterators")
                {
                    test_type ss(data.begin(), data.end());
                    CHECK(ss.size() == data.size());
                    CHECK(std::ranges::equal(ss, data));
                    CHECK(*ss.end() == value_type('\0'));
                }
                SECTION("initializer list")
                {
                    test_type ss{value_type('f'), value_type('o')};
                    CHECK(ss.size() == 2);
                    CHECK(ss[0] == value_type('f'));
                    CHECK(ss[1] == value_type('o'));
                    CHECK(*ss.end() == value_type('\0'));
                }
                SECTION("string_view")
                {
                    test_type ss{data};
                    CHECK(ss.size() == data.size());
                    CHECK(std::ranges::equal(ss, data));
                    CHECK(*ss.end() == value_type('\0'));
                }
            }
        });
}
EVAL_TEST_CASE("basic_inplace_string - construction");