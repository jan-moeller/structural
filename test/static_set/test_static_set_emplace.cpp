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
#include "structural/static_set.hpp"

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

TEST_CASE("static_set - emplace", "[container]")
{
    using namespace structural;

    static_set<non_assignable, 15, non_assignable_less> ss;
    CHECK((*(ss.emplace(4))).i == 4);
    CHECK((*(ss.emplace(4))).i == 4);
    CHECK((*(ss.emplace(1))).i == 1);
    CHECK((*(ss.emplace(5))).i == 5);
    CHECK((*(ss.emplace(6))).i == 6);
    CHECK((*(ss.emplace(2))).i == 2);
    CHECK((*(ss.emplace(9))).i == 9);
    CHECK((*(ss.emplace(0))).i == 0);

    CHECK(ss.size() == 7);
    CHECK(ss.contains(4));
    CHECK(ss.contains(1));
    CHECK(ss.contains(5));
    CHECK(ss.contains(6));
    CHECK(ss.contains(2));
    CHECK(ss.contains(9));
    CHECK(ss.contains(0));
}
EVAL_TEST_CASE("static_set - emplace");