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
CPMAddPackage("gh:jan-moeller/bugspray@0.4.1")

#############################################################################################################
# Unit test target
#############################################################################################################
add_executable(${PROJECT_NAME}
        basic_inplace_string/basic_inplace_string_test_data.hpp
        basic_inplace_string/test_basic_inplace_string_assignment.cpp
        basic_inplace_string/test_basic_inplace_string_capacity_deduction.cpp
        basic_inplace_string/test_basic_inplace_string_comparison.cpp
        basic_inplace_string/test_basic_inplace_string_construction.cpp
        basic_inplace_string/test_basic_inplace_string_read_access.cpp
        basic_inplace_string/test_basic_inplace_string_structurality.cpp
        basic_inplace_string/test_basic_inplace_string_write_access.cpp
        inplace_map/test_inplace_map_assignment.cpp
        inplace_map/test_inplace_map_constructor.cpp
        inplace_map/test_inplace_map_emplace.cpp
        inplace_map/test_inplace_map_erase.cpp
        inplace_map/test_inplace_map_find.cpp
        inplace_map/test_inplace_map_insert.cpp
        inplace_map/test_inplace_map_lower_upper_bound.cpp
        inplace_map/test_inplace_map_structurality.cpp
        inplace_set/test_inplace_set_assignment.cpp
        inplace_set/test_inplace_set_constructor.cpp
        inplace_set/test_inplace_set_emplace.cpp
        inplace_set/test_inplace_set_erase.cpp
        inplace_set/test_inplace_set_find.cpp
        inplace_set/test_inplace_set_insert.cpp
        inplace_set/test_inplace_set_lower_upper_bound.cpp
        inplace_set/test_inplace_set_structurality.cpp
        inplace_unordered_map/test_inplace_map_structurality.cpp
        inplace_unordered_map/test_inplace_unordered_map_assignment.cpp
        inplace_unordered_map/test_inplace_unordered_map_constructor.cpp
        inplace_unordered_map/test_inplace_unordered_map_emplace.cpp
        inplace_unordered_map/test_inplace_unordered_map_erase.cpp
        inplace_unordered_map/test_inplace_unordered_map_find.cpp
        inplace_unordered_map/test_inplace_unordered_map_insert.cpp
        inplace_unordered_set/test_inplace_unordered_set_assignment.cpp
        inplace_unordered_set/test_inplace_unordered_set_constructor.cpp
        inplace_unordered_set/test_inplace_unordered_set_emplace.cpp
        inplace_unordered_set/test_inplace_unordered_set_erase.cpp
        inplace_unordered_set/test_inplace_unordered_set_find.cpp
        inplace_unordered_set/test_inplace_unordered_set_insert.cpp
        inplace_unordered_set/test_inplace_unordered_set_structurality.cpp
        inplace_vector/test_inplace_vector_assignment.cpp
        inplace_vector/test_inplace_vector_comparison.cpp
        inplace_vector/test_inplace_vector_construction.cpp
        inplace_vector/test_inplace_vector_read_access.cpp
        inplace_vector/test_inplace_vector_structurality.cpp
        inplace_vector/test_inplace_vector_write_access.cpp
        test_bitset.cpp
        test_hash.cpp
        test_named_bitset.cpp
        test_pair.cpp
        test_structural_constant.cpp
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
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set_target_properties(${PROJECT_NAME} PROPERTIES
            COMPILE_FLAGS -fconstexpr-steps=2147483648
    )
endif ()

# enable compiler warnings for main library target
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(structural PUBLIC -Wall -Wpedantic -Wextra -Werror)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(structural PUBLIC -Wall -Wpedantic -Wextra -Werror -Wno-self-assign-overloaded)
elseif (MSVC)
    target_compile_options(structural PUBLIC /W4 /WX)
endif ()

enable_testing()
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
