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
        include/structural/detail/static_red_black_tree.hpp
        include/structural/detail/string_view_like.hpp
        include/structural/detail/to_underlying.hpp
        include/structural/detail/trim.hpp
        include/structural/detail/tuple_impl.hpp
        include/structural/detail/uninitialized_array_details.hpp
        include/structural/named_bitset.hpp
        include/structural/pair.hpp
        include/structural/static_map.hpp
        include/structural/static_set.hpp
        include/structural/static_string.hpp
        include/structural/static_vector.hpp
        include/structural/tuple.hpp
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
        GIT_REPOSITORY https://github.com/jan-moeller/bugspray.git
        GIT_TAG v0.1.0
)
fetchcontent_makeavailable(Bugspray)

add_executable(structural-tests
        test/basic_static_string/basic_static_string_test_data.hpp
        test/basic_static_string/test_basic_static_string_assignment.cpp
        test/basic_static_string/test_basic_static_string_capacity_deduction.cpp
        test/basic_static_string/test_basic_static_string_comparison.cpp
        test/basic_static_string/test_basic_static_string_construction.cpp
        test/basic_static_string/test_basic_static_string_read_access.cpp
        test/basic_static_string/test_basic_static_string_structurality.cpp
        test/basic_static_string/test_basic_static_string_write_access.cpp
        test/concept_structural_type_value.hpp
        test/static_map/test_static_map_assignment.cpp
        test/static_map/test_static_map_constructor.cpp
        test/static_map/test_static_map_emplace.cpp
        test/static_map/test_static_map_erase.cpp
        test/static_map/test_static_map_find.cpp
        test/static_map/test_static_map_insert.cpp
        test/static_map/test_static_map_lower_upper_bound.cpp
        test/static_map/test_static_map_structurality.cpp
        test/static_set/test_static_set_assignment.cpp
        test/static_set/test_static_set_constructor.cpp
        test/static_set/test_static_set_emplace.cpp
        test/static_set/test_static_set_erase.cpp
        test/static_set/test_static_set_find.cpp
        test/static_set/test_static_set_insert.cpp
        test/static_set/test_static_set_lower_upper_bound.cpp
        test/static_set/test_static_set_structurality.cpp
        test/static_vector/test_static_vector_assignment.cpp
        test/static_vector/test_static_vector_comparison.cpp
        test/static_vector/test_static_vector_construction.cpp
        test/static_vector/test_static_vector_read_access.cpp
        test/static_vector/test_static_vector_structurality.cpp
        test/static_vector/test_static_vector_write_access.cpp
        test/test_bitset.cpp
        test/test_named_bitset.cpp
        test/test_pair.cpp
        test/test_tuple.cpp
        test/test_uninitialized_array.cpp
        )
target_link_libraries(structural-tests PUBLIC bugspray-with-main structural)
set_target_properties(structural-tests PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )
set_target_properties(structural-tests PROPERTIES COMPILE_FLAGS -fconstexpr-ops-limit=2147483648)

include(CTest)
enable_testing()
add_test(NAME structural-tests COMMAND structural-tests)
