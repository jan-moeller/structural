#
# MIT License
#
# Copyright (c) 2023 Jan Möller
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
cmake_minimum_required(VERSION 3.14)
project(structural-tests LANGUAGES CXX)

#############################################################################################################
# Dependencies
#############################################################################################################
include(../cmake/CPM.cmake)

CPMAddPackage(NAME structural SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
CPMAddPackage("gh:jan-moeller/bugspray@0.3.0")

#############################################################################################################
# Unit test target
#############################################################################################################
add_executable(${PROJECT_NAME}
        basic_static_string/basic_static_string_test_data.hpp
        basic_static_string/test_basic_static_string_assignment.cpp
        basic_static_string/test_basic_static_string_capacity_deduction.cpp
        basic_static_string/test_basic_static_string_comparison.cpp
        basic_static_string/test_basic_static_string_construction.cpp
        basic_static_string/test_basic_static_string_read_access.cpp
        basic_static_string/test_basic_static_string_structurality.cpp
        basic_static_string/test_basic_static_string_write_access.cpp
        concept_structural_type_value.hpp
        static_map/test_static_map_assignment.cpp
        static_map/test_static_map_constructor.cpp
        static_map/test_static_map_emplace.cpp
        static_map/test_static_map_erase.cpp
        static_map/test_static_map_find.cpp
        static_map/test_static_map_insert.cpp
        static_map/test_static_map_lower_upper_bound.cpp
        static_map/test_static_map_structurality.cpp
        static_set/test_static_set_assignment.cpp
        static_set/test_static_set_constructor.cpp
        static_set/test_static_set_emplace.cpp
        static_set/test_static_set_erase.cpp
        static_set/test_static_set_find.cpp
        static_set/test_static_set_insert.cpp
        static_set/test_static_set_lower_upper_bound.cpp
        static_set/test_static_set_structurality.cpp
        static_unordered_map/test_static_map_structurality.cpp
        static_unordered_map/test_static_unordered_map_assignment.cpp
        static_unordered_map/test_static_unordered_map_constructor.cpp
        static_unordered_map/test_static_unordered_map_emplace.cpp
        static_unordered_map/test_static_unordered_map_erase.cpp
        static_unordered_map/test_static_unordered_map_find.cpp
        static_unordered_map/test_static_unordered_map_insert.cpp
        static_unordered_set/test_static_unordered_set_assignment.cpp
        static_unordered_set/test_static_unordered_set_constructor.cpp
        static_unordered_set/test_static_unordered_set_emplace.cpp
        static_unordered_set/test_static_unordered_set_erase.cpp
        static_unordered_set/test_static_unordered_set_find.cpp
        static_unordered_set/test_static_unordered_set_insert.cpp
        static_unordered_set/test_static_unordered_set_structurality.cpp
        static_vector/test_static_vector_assignment.cpp
        static_vector/test_static_vector_comparison.cpp
        static_vector/test_static_vector_construction.cpp
        static_vector/test_static_vector_read_access.cpp
        static_vector/test_static_vector_structurality.cpp
        static_vector/test_static_vector_write_access.cpp
        test_bitset.cpp
        test_hash.cpp
        test_named_bitset.cpp
        test_pair.cpp
        test_tuple.cpp
        test_uninitialized_array.cpp
)
target_link_libraries(${PROJECT_NAME} PUBLIC bugspray-with-main structural::structural)
set_target_properties(${PROJECT_NAME} PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set_target_properties(${PROJECT_NAME} PROPERTIES
            COMPILE_FLAGS -fconstexpr-ops-limit=2147483648
    )
endif ()

# enable compiler warnings for main library target
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(structural PUBLIC -Wall -Wpedantic -Wextra -Werror)
elseif (MSVC)
    target_compile_options(structural PUBLIC /W4 /WX)
endif ()

enable_testing()
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
