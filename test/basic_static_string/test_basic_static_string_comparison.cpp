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

TEST_CASE("basic_static_string - comparison", "[container][static_string]")
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
                SECTION("operator==")
                {
                    test_type ss(data.begin(), data.end());
                    SECTION("static_string == static_string")
                    {
                        test_type same = ss;
                        test_type not_same(value_type('a'));
                        CHECK(ss == same);
                        CHECK(ss != not_same);
                    }
                    SECTION("static_string == pointer")
                    {
                        CHECK(ss == data.begin());
                        CHECK(data.begin() == ss);
                        CHECK(ss != (data.begin() + 1));
                        CHECK((data.begin() + 1) != ss);
                    }
                    SECTION("static_string == string_view")
                    {
                        CHECK(ss == data);
                        CHECK(data == ss);
                        CHECK(ss != data.substr(1));
                        CHECK(data.substr(1) != ss);
                    }
                }
                SECTION("operator<=>")
                {
                    test_type ss(data.begin(), data.end());
                    test_type same = ss;
                    test_type lesser({value_type('a')});
                    test_type greater = ss;
                    greater += value_type('a');
                    SECTION("static_string <=> static_string")
                    {
                        CHECK((ss <=> same) == std::strong_ordering::equal);
                        CHECK((same <=> ss) == std::strong_ordering::equal);

                        CHECK((ss <=> lesser) == std::strong_ordering::greater);
                        CHECK((lesser <=> ss) == std::strong_ordering::less);

                        CHECK((ss <=> greater) == std::strong_ordering::less);
                        CHECK((greater <=> ss) == std::strong_ordering::greater);
                    }
                    SECTION("static_string <=> pointer")
                    {
                        CHECK((ss <=> same.c_str()) == std::strong_ordering::equal);
                        CHECK((same.c_str() <=> ss) == std::strong_ordering::equal);

                        CHECK((ss <=> lesser.c_str()) == std::strong_ordering::greater);
                        CHECK((lesser.c_str() <=> ss) == std::strong_ordering::less);

                        CHECK((ss <=> greater.c_str()) == std::strong_ordering::less);
                        CHECK((greater.c_str() <=> ss) == std::strong_ordering::greater);
                    }
                    SECTION("static_string <=> string_view")
                    {
                        CHECK((ss <=> same.substr()) == std::strong_ordering::equal);
                        CHECK((same.substr() <=> ss) == std::strong_ordering::equal);

                        CHECK((ss <=> lesser.substr()) == std::strong_ordering::greater);
                        CHECK((lesser.substr() <=> ss) == std::strong_ordering::less);

                        CHECK((ss <=> greater.substr()) == std::strong_ordering::less);
                        CHECK((greater.substr() <=> ss) == std::strong_ordering::greater);
                    }
                }
            }
        });
}
EVAL_TEST_CASE("basic_static_string - comparison");