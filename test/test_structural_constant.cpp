//
// MIT License
//
// Copyright (c) 2023 Jan Möller
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
#include "structural/structural_constant.hpp"

#include <bugspray/bugspray.hpp>

#include <array>

inline constexpr auto raw_i = 3;
inline constexpr auto raw_j = -1;
inline constexpr auto raw_k = std::array{1, 2, 3};

constexpr auto raw_l(int i)
{
    return i;
}

TEST_CASE("structural_constant", "[utility]")
{
    using namespace structural;

    auto i = constant<raw_i>;
    CHECK(i.value == raw_i);

    auto j = constant<&raw_j>;
    CHECK(j.value == &raw_j);

    auto k = constant<raw_k>;
    CHECK(k.value == raw_k);

    auto l = constant<raw_l>;
    CHECK(l.value == raw_l);

    SECTION("should be structural")
    {
        CHECK(structural_type<decltype(i)>);
        CHECK(structural_type<decltype(j)>);
    }
    SECTION("Unary +")
    {
        CHECK((+i).value == +raw_i);
    }
    SECTION("Unary -")
    {
        CHECK((-i).value == -raw_i);
    }
    SECTION("Unary !")
    {
        CHECK((!i).value == !raw_i);
    }
    SECTION("Unary ~")
    {
        CHECK((~i).value == ~raw_i);
    }
    SECTION("Unary *")
    {
        CHECK((*j).value == raw_j);
    }
    SECTION("Binary +")
    {
        CHECK((i + *j).value == raw_i + raw_j);
        CHECK((*j + i).value == raw_j + raw_i);
    }
    SECTION("Binary -")
    {
        CHECK((i - *j).value == raw_i - raw_j);
        CHECK((*j - i).value == raw_j - raw_i);
    }
    SECTION("Binary *")
    {
        CHECK((i * *j).value == raw_i * raw_j);
        CHECK((*j * i).value == raw_j * raw_i);
    }
    SECTION("Binary /")
    {
        CHECK((i / *j).value == raw_i / raw_j);
        CHECK((*j / i).value == raw_j / raw_i);
    }
    SECTION("Binary %")
    {
        CHECK((i % *j).value == raw_i % raw_j);
        CHECK((*j % i).value == raw_j % raw_i);
    }
    SECTION("Binary ==")
    {
        CHECK((i == *j).value == (raw_i == raw_j));
        CHECK((*j == i).value == (raw_j == raw_i));
    }
    SECTION("Binary !=")
    {
        CHECK((i != *j).value == (raw_i != raw_j));
        CHECK((*j != i).value == (raw_j != raw_i));
    }
    SECTION("Binary <")
    {
        CHECK((i < *j).value == (raw_i < raw_j));
        CHECK((*j < i).value == (raw_j < raw_i));
    }
    SECTION("Binary <=")
    {
        CHECK((i <= *j).value == (raw_i <= raw_j));
        CHECK((*j <= i).value == (raw_j <= raw_i));
    }
    SECTION("Binary >")
    {
        CHECK((i > *j).value == (raw_i > raw_j));
        CHECK((*j > i).value == (raw_j > raw_i));
    }
    SECTION("Binary >=")
    {
        CHECK((i >= *j).value == (raw_i >= raw_j));
        CHECK((*j >= i).value == (raw_j >= raw_i));
    }
    SECTION("Binary &&")
    {
        CHECK((i && *j).value == (raw_i && raw_j));
        CHECK((*j && i).value == (raw_j && raw_i));
    }
    SECTION("Binary ||")
    {
        CHECK((i || *j).value == (raw_i || raw_j));
        CHECK((*j || i).value == (raw_j || raw_i));
    }
    SECTION("Binary &")
    {
        CHECK((i & *j).value == (raw_i & raw_j));
        CHECK((*j & i).value == (raw_j & raw_i));
    }
    SECTION("Binary |")
    {
        CHECK((i | *j).value == (raw_i | raw_j));
        CHECK((*j | i).value == (raw_j | raw_i));
    }
    SECTION("Binary ^")
    {
        CHECK((i ^ *j).value == (raw_i ^ raw_j));
        CHECK((*j ^ i).value == (raw_j ^ raw_i));
    }
    SECTION("Binary <<")
    {
        CHECK((*j << i).value == (raw_j << raw_i));
    }
    SECTION("Binary >>")
    {
        CHECK((*j >> i).value == (raw_j >> raw_i));
    }
    SECTION("Binary []")
    {
        CHECK((k[constant<0>]).value == raw_k[0]);
        CHECK((k[constant<1>]).value == raw_k[1]);
        CHECK((k[constant<2>]).value == raw_k[2]);
    }
    SECTION("Binary ()")
    {
        CHECK((l(constant<0>)).value == raw_l(0));
        CHECK((l(constant<1>)).value == raw_l(1));
        CHECK((l(constant<2>)).value == raw_l(2));
    }
    SECTION("convertible to underlying type")
    {
        CHECK(static_cast<std::remove_cvref_t<decltype(raw_i)>>(i) == i.value);
        CHECK(static_cast<std::remove_cvref_t<decltype(&raw_j)>>(j) == j.value);
    }
    SECTION("compiletime concept matches correctly")
    {
        CHECK(compiletime<int> == false);
        CHECK(compiletime<int, int> == false);
        CHECK(compiletime<structural_constant<int, 2>> == true);
        CHECK(compiletime<structural_constant<int, 2>, int> == true);
        CHECK(compiletime<structural_constant<int, 2>, float> == true);
        CHECK(compiletime<structural_constant<int, 2>, char const*> == false);
    }
    SECTION("implicitly convertible to value_type")
    {
        constexpr auto f = [](std::array<int, 3> const& a)
        {
            return a;
        };
        CHECK(i == raw_i);
        CHECK(*j == raw_j);
        CHECK(f(k) == raw_k);
        CHECK(l == raw_l);
    }
}
EVAL_TEST_CASE("structural_constant");