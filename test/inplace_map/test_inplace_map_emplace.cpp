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
#include "structural/inplace_map.hpp"

#include <bugspray/bugspray.hpp>

#include <concepts>

struct non_assignable
{
    int            i;
    constexpr auto operator==(non_assignable const&) const -> bool = default;
    constexpr auto operator<=>(non_assignable const&) const        = default;
    constexpr auto operator=(non_assignable const&)                = delete;
    constexpr auto operator=(non_assignable&&)                     = delete;
};

struct non_assignable_less
{
    template<std::integral T>
    constexpr auto operator()(T const& lhs, non_assignable const& rhs) const noexcept -> bool
    {
        return lhs < rhs.i;
    }

    template<std::integral T>
    constexpr auto operator()(non_assignable const& lhs, T const& rhs) const noexcept -> bool
    {
        return lhs.i < rhs;
    }

    constexpr auto operator()(non_assignable const& lhs, non_assignable const& rhs) const noexcept -> bool
    {
        return lhs < rhs;
    }
};

TEST_CASE("inplace_map - emplace", "[container]")
{
    using namespace structural;

    SECTION("emplace")
    {
        inplace_map<non_assignable, int, 15, non_assignable_less> sm;
        CHECK((*(sm.emplace(4, 1))).second == 1);
        CHECK((*(sm.emplace(4, 2))).second == 1);
        CHECK((*(sm.emplace(1, 3))).second == 3);
        CHECK((*(sm.emplace(5, 4))).second == 4);
        CHECK((*(sm.emplace(6, 5))).second == 5);
        CHECK((*(sm.emplace(2, 6))).second == 6);
        CHECK((*(sm.emplace(9, 7))).second == 7);
        CHECK((*(sm.emplace(0, 8))).second == 8);

        CHECK(sm.size() == 7);
        CHECK(sm.contains(non_assignable{4}));
        CHECK(sm.contains(non_assignable{1}));
        CHECK(sm.contains(non_assignable{5}));
        CHECK(sm.contains(non_assignable{6}));
        CHECK(sm.contains(non_assignable{2}));
        CHECK(sm.contains(non_assignable{9}));
        CHECK(sm.contains(non_assignable{0}));
    }
    SECTION("try_emplace")
    {
        inplace_map<int, non_assignable, 15> sm;
        CHECK(sm.try_emplace(4, 1).first->second.i == 1);
        CHECK(sm.try_emplace(4, 2).first->second.i == 1);
        CHECK(sm.try_emplace(1, 3).first->second.i == 3);
        CHECK(sm.try_emplace(5, 4).first->second.i == 4);
        CHECK(sm.try_emplace(6, 5).first->second.i == 5);
        CHECK(sm.try_emplace(2, 6).first->second.i == 6);
        CHECK(sm.try_emplace(9, 7).first->second.i == 7);
        CHECK(sm.try_emplace(0, 8).first->second.i == 8);

        CHECK(sm.size() == 7);
        CHECK(sm.contains(4));
        CHECK(sm.contains(1));
        CHECK(sm.contains(5));
        CHECK(sm.contains(6));
        CHECK(sm.contains(2));
        CHECK(sm.contains(9));
        CHECK(sm.contains(0));
    }
}
EVAL_TEST_CASE("inplace_map - emplace");