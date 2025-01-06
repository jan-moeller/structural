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
#include "serialize_helper.hpp"

#include <bugspray/bugspray.hpp>

TEST_CASE("serialize - chrono", "[utility]")
{
    using namespace structural;
    using namespace std::chrono_literals;

    CHECK(deserialize<std::chrono::hours>(serialize(42h)) == 42h);
    CHECK(deserialize<std::chrono::minutes>(serialize(-42min)) == -42min);

    CHECK(deserialize<std::chrono::local_days>(serialize(std::chrono::local_days(std::chrono::days{42})))
          == std::chrono::local_days(std::chrono::days{42}));
}
EVAL_TEST_CASE("serialize - chrono");