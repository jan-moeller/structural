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

#if __cpp_lib_constexpr_memory >= 202202L

#include "structural/structuralize.hpp"

#include <bugspray/bugspray.hpp>

TEST_CASE("structuralize - unique_ptr", "[utility]")
{
    using namespace structural;

    constexpr auto opt1 = STRUCTURALIZE(std::unique_ptr<int>{});
    constexpr auto opt2 = STRUCTURALIZE(std::make_unique<int>(42));

    CHECK(opt1 == nullptr);
    CHECK(opt2 == 42);
}
EVAL_TEST_CASE("structuralize - unique_ptr");

#endif