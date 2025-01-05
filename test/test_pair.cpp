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
#include "structural/concept_structural_type_value.hpp"
#include "structural/pair.hpp"

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

TEST_CASE("pair")
{
    using namespace structural;

    SECTION("structurality")
    {
        REQUIRE(structural_type<pair<int, double>>);
    }

    SECTION("constructor")
    {
        SECTION("default")
        {
            pair<int, double> t2{};
            REQUIRE(get<0>(t2) == int{});
            REQUIRE(get<1>(t2) == double{});
        }
        SECTION("direct initialization")
        {
            pair<int, double> t{42, 3.141};
            REQUIRE(get<0>(t) == 42);
            REQUIRE(get<1>(t) == 3.141);
        }
    }
    SECTION("assignment")
    {
        pair<int, double> t1;
        pair<int, double> t2{42, 3.141};
        t1 = t2;
        REQUIRE(t1 == t2);
    }
    SECTION("get")
    {
        pair<int, double> t{42, 3.141};
        REQUIRE(get<0>(t) == 42);
        REQUIRE(get<1>(t) == 3.141);
        REQUIRE(get<int>(t) == get<0>(t));
        REQUIRE(get<double>(t) == get<1>(t));
    }
    SECTION("operator==")
    {
        pair<int, double> t1{42, 3.141};
        pair<int, double> t2{};
        REQUIRE(t1 == t1);
        REQUIRE(t1 != t2);
    }
    SECTION("operator<=>")
    {
        SECTION("strong_ordering")
        {
            pair<strongly_ordered, strongly_ordered> t{strongly_ordered{0}, strongly_ordered{1}};
            pair<strongly_ordered, strongly_ordered> less{strongly_ordered{-1}, strongly_ordered{1}};
            pair<strongly_ordered, strongly_ordered> greater{strongly_ordered{1}, strongly_ordered{1}};
            REQUIRE((t <=> t) == std::strong_ordering::equal);
            REQUIRE((t <=> less) == std::strong_ordering::greater);
            REQUIRE((t <=> greater) == std::strong_ordering::less);
        }
        SECTION("weak_ordering")
        {
            pair<weakly_ordered, weakly_ordered> t{weakly_ordered{0}, weakly_ordered{1}};
            pair<weakly_ordered, weakly_ordered> less{weakly_ordered{-1}, weakly_ordered{1}};
            pair<weakly_ordered, weakly_ordered> greater{weakly_ordered{1}, weakly_ordered{1}};
            REQUIRE((t <=> t) == std::weak_ordering::equivalent);
            REQUIRE((t <=> less) == std::weak_ordering::greater);
            REQUIRE((t <=> greater) == std::weak_ordering::less);
        }
        SECTION("partial_ordering")
        {
            pair<partially_ordered, partially_ordered> t{partially_ordered{0}, partially_ordered{1}};
            pair<partially_ordered, partially_ordered> less{partially_ordered{-1}, partially_ordered{1}};
            pair<partially_ordered, partially_ordered> greater{partially_ordered{1}, partially_ordered{1}};
            pair<partially_ordered, partially_ordered> unordered{partially_ordered{1, true}, partially_ordered{1}};
            REQUIRE((t <=> t) == std::partial_ordering::equivalent);
            REQUIRE((t <=> less) == std::partial_ordering::greater);
            REQUIRE((t <=> greater) == std::partial_ordering::less);
            REQUIRE((t <=> unordered) == std::partial_ordering::unordered);
        }
    }
    SECTION("tuple_size")
    {
        REQUIRE(std::tuple_size_v<pair<int, double>> == 2);
        REQUIRE(std::tuple_size_v<pair<int, int>> == 2);
    }
    SECTION("tuple_element")
    {
        REQUIRE(std::same_as<std::tuple_element_t<0, pair<int, double>>, int>);
        REQUIRE(std::same_as<std::tuple_element_t<1, pair<int, double>>, double>);
    }
    SECTION("structural binding")
    {
        pair<int, double> t{42, 3.141};
        auto [i, d] = t;
        REQUIRE(i == 42);
        REQUIRE(d == 3.141);
    }
}
EVAL_TEST_CASE("pair");