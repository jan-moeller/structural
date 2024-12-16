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
#include "structural/uninitialized_array.hpp"

#include <bugspray/bugspray.hpp>
#include <bugspray/utility/static_for_each_type.hpp>
#include <bugspray/utility/stringify_typename.hpp>

#include <iterator>

struct non_trivial
{
    constexpr non_trivial() {}
    constexpr ~non_trivial() {}
    constexpr auto operator==(non_trivial const&) const -> bool = default;
};

TEST_CASE("uninitialized_array", "[container]")
{
    using namespace structural;

    std::array<uninitialized_array<int, 3>, 5> some_data;
    some_data[0].construct_at(std::size_t{0}, 42);
    [[maybe_unused]] std::tuple<std::array<uninitialized_array<int, 3>, 5>> some_tuple;

    SECTION("structurality")
    {
        CHECK(structural_type<uninitialized_array<int, 3>>);
        CHECK(structural_type<uninitialized_array<double, 3>>);
        CHECK(structural_type<uninitialized_array<uninitialized_array<int, 3>, 3>>);
        CHECK(structural_type<uninitialized_array<uninitialized_array<double, 3>, 3>>);
    }

    SECTION("iterators are contiguous")
    {
        bs::static_for_each_type<uninitialized_array<int, 3>,
                                 uninitialized_array<int, 3> const,
                                 uninitialized_array<uninitialized_array<int, 3>, 3>,
                                 uninitialized_array<uninitialized_array<int, 3>, 3> const>(
            [&]<typename test_type>()
            {
                CAPTURE(bs::stringify_typename<test_type>());

                REQUIRE(std::contiguous_iterator<typename test_type::iterator>);
                REQUIRE(std::contiguous_iterator<typename test_type::const_iterator>);

                test_type                          foo{};
                typename test_type::const_iterator a{&foo.array[0]};
                typename test_type::const_iterator b{&foo.array[1]};
                typename test_type::const_iterator c{&foo.array[2]};
                std::iter_difference_t<test_type>  n = b - a;

                CAPTURE(n);

                SECTION("(a += n) is equal to b")
                {
                    auto const lhs = (a += n);
                    REQUIRE(lhs == b);
                }
                SECTION("std::addressof(a += n) is equal to std::addressof(a)")
                {
                    auto const lhs = std::addressof(a += n);
                    REQUIRE(lhs == std::addressof(a));
                }
                SECTION("(a + n) is equal to (a += n)")
                {
                    auto const lhs = a + n;
                    REQUIRE(lhs == (a += n));
                }
                SECTION("(a + n) is equal to (n + a)")
                {
                    REQUIRE((a + n) == (n + a));
                }
                SECTION(
                    "For any two positive integers x and y, if a + (x + y) is valid, then a + (x + y) is equal to (a + x) + y")
                {
                    int const x = 1;
                    int const y = 1;
                    REQUIRE((a + (x + y)) == ((a + x) + y));
                }
                SECTION("a + 0 is equal to a")
                {
                    REQUIRE((a + 0) == a);
                }
                SECTION("If (a + (n - 1)) is valid, then --b is equal to (a + (n - 1))")
                {
                    auto const lhs = --b;
                    REQUIRE(lhs == (a + (n - 1)));
                }
                SECTION("(b += -n) is equal to a")
                {
                    auto const lhs = (b += -n);
                    REQUIRE(lhs == a);
                }
                SECTION("(b -= n) is equal to a")
                {
                    auto const lhs = (b -= n);
                    REQUIRE(lhs == a);
                }
                SECTION("std::addressof(b -= n) is equal to std::addressof(b)")
                {
                    auto const lhs = std::addressof(b -= n);
                    REQUIRE(lhs == std::addressof(b));
                }
                SECTION("(b - n) is equal to (b -= n)")
                {
                    auto const lhs = b - n;
                    REQUIRE(lhs == (b -= n));
                }
                SECTION("If b is dereferenceable, then a[n] is valid and is equal to *b")
                {
                    if constexpr (!std::is_const_v<test_type>
                                  && std::equality_comparable<typename test_type::value_type>)
                    {
                        foo.construct_at(b);
                        REQUIRE(a[n] == *b);
                        foo.destroy_at(b);
                    }
                }
                SECTION("bool(a <= b) is true")
                {
                    REQUIRE(a <= b);
                }

                SECTION("std::to_address(a) == std::addressof(*a)")
                {
                    REQUIRE(std::to_address(a) == std::addressof(*a));
                }
                SECTION("std::to_address(b) == std::to_address(a) + std::iter_difference_t<I>(b - a)")
                {
                    auto const rhs = std::to_address(a)
                                     + std::iter_difference_t<typename test_type::const_iterator>(b - a);
                    REQUIRE(std::to_address(b) == rhs);
                }
                SECTION("std::to_address(c) == std::to_address(a) + std::iter_difference_t<I>(c - a)")
                {
                    auto const rhs = std::to_address(a)
                                     + std::iter_difference_t<typename test_type::const_iterator>(c - a);
                    REQUIRE(std::to_address(c) == rhs);
                }
            });
    }

    SECTION("construction & destruction")
    {
        SECTION("trivial")
        {
            uninitialized_array<int, 3> array{};
            array.construct_at(1, 42);
            CHECK(array[1] == 42);
            array.destroy_at(1);
        }

        SECTION("trivial nested")
        {
            uninitialized_array<uninitialized_array<int, 3>, 3> array{};
            array.construct_at(1);
            array[1].construct_at(1, 42);
            CHECK(array[1][1] == 42);
            array[1].destroy_at(1);
            array.destroy_at(1);
        }

        SECTION("non-trivial")
        {
            uninitialized_array<non_trivial, 3> array{};
            array.construct_at(1);
            CHECK(array[1] == non_trivial{});
            array.destroy_at(1);
        }

        SECTION("non-trivial nested")
        {
            uninitialized_array<uninitialized_array<non_trivial, 3>, 3> array{};
            array.construct_at(1);
            array[1].construct_at(1);
            CHECK(array[1][1] == non_trivial{});
            array[1].destroy_at(1);
            array.destroy_at(1);
        }
    }

    uninitialized_array<int, 3> array{};
    SECTION("operator[]", runtime)
    {
        array.construct_at(1);
        CHECK(array[1] == 0);
        array.destroy_at(1);
    }
}
EVAL_TEST_CASE("uninitialized_array");