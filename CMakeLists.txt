#
# MIT License
#
# Copyright (c) 2022 Jan Möller
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
cmake_minimum_required(VERSION 3.20)
project(structural)

add_library(structural
        include/structural/basic_static_string.hpp
        include/structural/bitset.hpp
        include/structural/detail/split.hpp
        include/structural/detail/static_for.hpp
        include/structural/detail/string_view_like.hpp
        include/structural/detail/to_underlying.hpp
        include/structural/detail/trim.hpp
        include/structural/detail/uninitialized_array_details.hpp
        include/structural/named_bitset.hpp
        include/structural/static_vector.hpp
        include/structural/uninitialized_array.hpp
        )
target_include_directories(structural PUBLIC include/)
set_target_properties(structural PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(structural PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )

include(FetchContent)
fetchcontent_declare(
        Bugspray
        GIT_REPOSITORY git@github.com:jan-moeller/bugspray.git
        GIT_TAG dev
)
fetchcontent_makeavailable(Bugspray)

add_executable(structural-tests
        test/basic_static_string/test_basic_static_string_append.hpp
        test/basic_static_string/test_basic_static_string_assign.hpp
        test/basic_static_string/test_basic_static_string_at.hpp
        test/basic_static_string/test_basic_static_string_c_str.hpp
        test/basic_static_string/test_basic_static_string_clear.hpp
        test/basic_static_string/test_basic_static_string_compare.hpp
        test/basic_static_string/test_basic_static_string_constructors.hpp
        test/basic_static_string/test_basic_static_string_conversion_string_view.hpp
        test/basic_static_string/test_basic_static_string_empty_size.hpp
        test/basic_static_string/test_basic_static_string_erase.hpp
        test/basic_static_string/test_basic_static_string_front_back.hpp
        test/basic_static_string/test_basic_static_string_hash.hpp
        test/basic_static_string/test_basic_static_string_insert.hpp
        test/basic_static_string/test_basic_static_string_iterators.hpp
        test/basic_static_string/test_basic_static_string_operator+.hpp
        test/basic_static_string/test_basic_static_string_operator+=.hpp
        test/basic_static_string/test_basic_static_string_operator<=>.hpp
        test/basic_static_string/test_basic_static_string_operator==.hpp
        test/basic_static_string/test_basic_static_string_operator[].hpp
        test/basic_static_string/test_basic_static_string_push_pop_back.hpp
        test/basic_static_string/test_basic_static_string_replace.hpp
        test/basic_static_string/test_basic_static_string_starts_with_ends_with.hpp
        test/basic_static_string/test_basic_static_string_substr.hpp
        test/concept_structural_type_value.hpp
        test/test_basic_static_string.cpp
        test/test_bitset.cpp
        test/test_named_bitset.cpp
        test/test_static_vector.cpp
        test/test_uninitialized_array.cpp
        )
target_link_libraries(structural-tests PUBLIC bugspray-with-main structural)
set_target_properties(structural-tests PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )
set_target_properties(structural-tests PROPERTIES COMPILE_FLAGS -fconstexpr-ops-limit=2147483648)
