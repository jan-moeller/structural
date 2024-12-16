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
#include "structural/hash.hpp"

#include <bugspray/bugspray.hpp>

TEST_CASE("hash", "[utility]")
{
    using namespace structural;

    SECTION("int")
    {
        hash<int>{}(-1);
        hash<int>{}(0);
        hash<int>{}(1);
        CHECK(structural_type<hash<int>>);
    }
    SECTION("float")
    {
        hash<float>{}(-1.f);
        hash<float>{}(0.f);
        hash<float>{}(1.f);
        CHECK(structural_type<hash<float>>);
    }
    SECTION("nullptr_t")
    {
        hash<std::nullptr_t>{}(nullptr);
        CHECK(structural_type<hash<std::nullptr_t>>);
    }
    SECTION("ptr", runtime)
    {
        int         i = 3;
        float const j = 24.4f;
        hash<int*>{}(nullptr);
        hash<int*>{}(&i);
        hash<float const*>{}(nullptr);
        hash<float const*>{}(&j);
        CHECK(structural_type<hash<int*>>);
        CHECK(structural_type<hash<float const*>>);
    }
    SECTION("enum")
    {
        enum foo_t
        {
            foo
        };
        enum class bar_t
        {
            bar
        };
        hash<foo_t>{}(foo_t::foo);
        hash<bar_t>{}(bar_t::bar);
        CHECK(structural_type<hash<foo_t>>);
        CHECK(structural_type<hash<bar_t>>);
    }
    SECTION("range")
    {
        hash<std::array<int, 3>>{}(std::array<int, 3>{1, 2, 3});
        hash<std::string_view>{}(std::string_view{"foobar"});
        CHECK(structural_type<hash<std::string_view>>);
    }
    SECTION("optional")
    {
        hash<std::optional<int>>{}(std::nullopt);
        hash<std::optional<int>>{}({3});
        CHECK(structural_type<hash<std::optional<int>>>);
    }
    SECTION("variant")
    {
        hash<std::variant<int, float>>{}({3});
        hash<std::variant<int, float>>{}({3.5f});
        CHECK(structural_type<hash<std::variant<int, float>>>);
    }
    SECTION("std::monostate")
    {
        hash<std::monostate>{}({});
        CHECK(structural_type<hash<std::monostate>>);
    }
    SECTION("std::bitset", runtime)
    {
        hash<std::bitset<16>>{}({});
        hash<std::bitset<16>>{}(std::bitset<16>{"111011101"});
        CHECK(structural_type<hash<std::bitset<16>>>);
    }
    SECTION("std::unique_ptr", runtime)
    {
        hash<std::unique_ptr<int>>{}({});
        hash<std::unique_ptr<int>>{}(std::make_unique<int>(42));
        CHECK(structural_type<hash<std::unique_ptr<int>>>);
    }
    SECTION("std::shared_ptr", runtime)
    {
        hash<std::shared_ptr<int>>{}({});
        hash<std::shared_ptr<int>>{}(std::make_shared<int>(42));
        CHECK(structural_type<hash<std::shared_ptr<int>>>);
    }
    SECTION("std::type_index", runtime)
    {
        hash<std::type_index>{}(typeid(int));
        hash<std::type_index>{}(typeid(float));
        CHECK(structural_type<hash<std::type_index>>);
    }
}
EVAL_TEST_CASE("hash");