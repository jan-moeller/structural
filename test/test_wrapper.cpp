//
// MIT License
//
// Copyright (c) 2025 Jan Möller
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
#include "structural/wrapper.hpp"

#include <bugspray/bugspray.hpp>

TEST_CASE("wrapper", "[utility]")
{
    using namespace structural;

    struct recursive
    {
        int                    i;
        std::vector<recursive> more{};

        constexpr auto operator==(recursive const&) const -> bool = default;
    };

    constexpr auto wrapped1 = STRUCTURAL_WRAP(recursive{});
    constexpr auto wrapped2 = STRUCTURAL_WRAP(recursive{1});
    constexpr auto wrapped3 = STRUCTURAL_WRAP(recursive{2, {recursive{3}, recursive{4}}});

    CHECK(wrapped1.unwrap() == recursive{});
    CHECK(wrapped2.unwrap() == recursive{1});
    CHECK(wrapped3.unwrap() == recursive{2, {recursive{3}, recursive{4}}});
}
EVAL_TEST_CASE("wrapper");