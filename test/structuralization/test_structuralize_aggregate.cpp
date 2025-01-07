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

#include "structural/structuralize.hpp"

#include <bugspray/bugspray.hpp>

TEST_CASE("structuralize - aggregate", "[utility]")
{
    using namespace structural;

    struct A1
    {
        int   a;
        float b;

        constexpr auto operator==(A1 const&) const -> bool = default;
    };
    struct A2
    {
        int                  a;
        std::optional<float> b;

        constexpr auto operator==(A2 const&) const -> bool = default;
    };

    constexpr auto val1 = STRUCTURALIZE(A1{42, 3.14f});
    constexpr auto val2 = STRUCTURALIZE(A2{42, std::nullopt});

    CHECK(val1 == A1{42, 3.14f});
    CHECK(val2 == tuple{42, std::nullopt});
}
EVAL_TEST_CASE("structuralize - aggregate");