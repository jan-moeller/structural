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

TEST_CASE("structuralize - variant", "[utility]")
{
    using namespace structural;

    constexpr auto var1 = STRUCTURALIZE(std::variant<int>{42});
    constexpr auto var2 = STRUCTURALIZE(std::variant<int, float>{1.23f});
    constexpr auto var3 = STRUCTURALIZE(std::variant<int, float, std::vector<int>>{std::vector{42}});

    CHECK(var1 == 42);
    CHECK(var2 == 1.23f);
    CHECK(var3 == std::array{42});
}
EVAL_TEST_CASE("structuralize - variant");