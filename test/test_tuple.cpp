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
#include "structural/tuple.hpp"

#include <bugspray/bugspray.hpp>

struct strongly_ordered
{
    int            i;
    constexpr auto operator==(strongly_ordered const& rhs) const noexcept -> bool = default;
    constexpr auto operator<=>(strongly_ordered const& rhs) const noexcept -> std::strong_ordering
    {
        return i <=> rhs.i;
    }
};
struct weakly_ordered
{
    int            i;
    constexpr auto operator==(weakly_ordered const& rhs) const noexcept -> bool = default;
    constexpr auto operator<=>(weakly_ordered const& rhs) const noexcept -> std::weak_ordering { return i <=> rhs.i; }
};
struct partially_ordered
{
    int            i;
    bool           unordered                                                       = false;
    constexpr auto operator==(partially_ordered const& rhs) const noexcept -> bool = default;
    constexpr auto operator<=>(partially_ordered const& rhs) const noexcept -> std::partial_ordering
    {
        if (unordered || rhs.unordered)
            return std::partial_ordering::unordered;
        return i <=> rhs.i;
    }
};

TEST_CASE("tuple")
{
    using namespace structural;

    SECTION("structurality")
    {
        REQUIRE(structural_type<tuple<>>);
        REQUIRE(structural_type<tuple<int>>);
        REQUIRE(structural_type<tuple<int, double>>);
    }

    SECTION("constructor")
    {
        SECTION("default")
        {
            [[maybe_unused]] tuple<> t1{};
            tuple<int, double>       t2{};
            REQUIRE(get<0>(t2) == int{});
            REQUIRE(get<1>(t2) == double{});
        }
        SECTION("direct initialization")
        {
            tuple<int, double> t{42, 3.141};
            REQUIRE(get<0>(t) == 42);
            REQUIRE(get<1>(t) == 3.141);
        }
    }
    SECTION("assignment")
    {
        SECTION("empty tuple")
        {
            tuple<> t1;
            tuple<> t2;
            t1 = t1;
            REQUIRE(t1 == tuple<>());
            t1 = t2;
            REQUIRE(t1 == t2);
        }
        SECTION("non-empty tuple")
        {
            tuple<int, double> t1;
            tuple<int, double> t2{42, 3.141};
            t1 = t1;
            REQUIRE(t1 == tuple<int, double>());
            t1 = t2;
            REQUIRE(t1 == t2);
        }
    }
    SECTION("get")
    {
        tuple<int, double> t{42, 3.141};
        REQUIRE(get<0>(t) == 42);
        REQUIRE(get<1>(t) == 3.141);
        REQUIRE(get<int>(t) == 42);
        REQUIRE(get<double>(t) == 3.141);

        REQUIRE(get<0>(static_cast<tuple<int, double> const&>(t)) == 42);
        REQUIRE(get<1>(static_cast<tuple<int, double> const&>(t)) == 3.141);
        REQUIRE(get<int>(static_cast<tuple<int, double> const&>(t)) == 42);
        REQUIRE(get<double>(static_cast<tuple<int, double> const&>(t)) == 3.141);

        REQUIRE(get<0>((tuple{42, 3.141})) == 42);
        REQUIRE(get<1>((tuple{42, 3.141})) == 3.141);
        REQUIRE(get<int>((tuple{42, 3.141})) == 42);
        REQUIRE(get<double>((tuple{42, 3.141})) == 3.141);

        REQUIRE(get<0>(static_cast<tuple<int, double> const&&>(tuple{42, 3.141})) == 42);
        REQUIRE(get<1>(static_cast<tuple<int, double> const&&>(tuple{42, 3.141})) == 3.141);
        REQUIRE(get<int>(static_cast<tuple<int, double> const&&>(tuple{42, 3.141})) == 42);
        REQUIRE(get<double>(static_cast<tuple<int, double> const&&>(tuple{42, 3.141})) == 3.141);
    }
    SECTION("operator==")
    {
        SECTION("empty")
        {
            REQUIRE(tuple<>() == tuple<>());
        }
        SECTION("non-empty")
        {
            tuple<int, double> t1{42, 3.141};
            tuple<int, double> t2{};
            REQUIRE(t1 == t1);
            REQUIRE(t1 != t2);
        }
    }
    SECTION("operator<=>")
    {
        SECTION("empty")
        {
            REQUIRE((tuple<>() <=> tuple<>()) == std::strong_ordering::equal);
        }
        SECTION("strong_ordering")
        {
            tuple<strongly_ordered> t{strongly_ordered{0}};
            tuple<strongly_ordered> less{strongly_ordered{-1}};
            tuple<strongly_ordered> greater{strongly_ordered{1}};
            REQUIRE((t <=> t) == std::strong_ordering::equal);
            REQUIRE((t <=> less) == std::strong_ordering::greater);
            REQUIRE((t <=> greater) == std::strong_ordering::less);
        }
        SECTION("weak_ordering")
        {
            tuple<weakly_ordered> t{weakly_ordered{0}};
            tuple<weakly_ordered> less{weakly_ordered{-1}};
            tuple<weakly_ordered> greater{weakly_ordered{1}};
            REQUIRE((t <=> t) == std::weak_ordering::equivalent);
            REQUIRE((t <=> less) == std::weak_ordering::greater);
            REQUIRE((t <=> greater) == std::weak_ordering::less);
        }
        SECTION("partial_ordering")
        {
            tuple<partially_ordered> t{partially_ordered{0}};
            tuple<partially_ordered> less{partially_ordered{-1}};
            tuple<partially_ordered> greater{partially_ordered{1}};
            tuple<partially_ordered> unordered{partially_ordered{1, true}};
            REQUIRE((t <=> t) == std::partial_ordering::equivalent);
            REQUIRE((t <=> less) == std::partial_ordering::greater);
            REQUIRE((t <=> greater) == std::partial_ordering::less);
            REQUIRE((t <=> unordered) == std::partial_ordering::unordered);
        }
    }
    SECTION("tuple_size")
    {
        REQUIRE(std::tuple_size_v<tuple<>> == 0);
        REQUIRE(std::tuple_size_v<tuple<int>> == 1);
        REQUIRE(std::tuple_size_v<tuple<int, double>> == 2);
        REQUIRE(std::tuple_size_v<tuple<int, int>> == 2);
    }
    SECTION("tuple_element")
    {
        REQUIRE(std::same_as<std::tuple_element_t<0, tuple<int, double>>, int>);
        REQUIRE(std::same_as<std::tuple_element_t<1, tuple<int, double>>, double>);
    }
    SECTION("structural binding")
    {
        tuple<int, double> t{42, 3.141};
        auto [i, d] = t;
        REQUIRE(i == 42);
        REQUIRE(d == 3.141);
    }
    SECTION("make_tuple")
    {
        auto t = make_tuple(3, 42.4);
        REQUIRE(get<0>(t) == 3);
        REQUIRE(get<1>(t) == 42.4);
    }
    SECTION("forward_as_tuple")
    {
        SECTION("lvalue input")
        {
            int const i = 3;
            auto      t = forward_as_tuple(i);
            REQUIRE(get<0>(t) == i);
            REQUIRE(std::same_as<decltype(t), tuple<const int&>>);
        }
        SECTION("rvalue input")
        {
            REQUIRE(get<0>(forward_as_tuple(3)) == 3);
            REQUIRE(std::same_as<decltype(forward_as_tuple(3)), tuple<int&&>>);
        }
    }
}
EVAL_TEST_CASE("tuple");