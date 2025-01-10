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
project(structural
        VERSION 0.5.0
        LANGUAGES CXX
)

#############################################################################################################
# Prevent in-source builds
#############################################################################################################
if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
            FATAL_ERROR
            "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
    )
endif ()

#############################################################################################################
# Dependencies
#############################################################################################################
include(cmake/CPM.cmake)

CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.11.0")
CPMAddPackage("gh:jan-moeller/ctrx@1.2.0")

#############################################################################################################
# Build configuration
#############################################################################################################
message(STATUS "------------------------------------------------------------------------------")
message(STATUS "    ${PROJECT_NAME} (${PROJECT_VERSION})")
message(STATUS "------------------------------------------------------------------------------")
message(STATUS "Build type:                ${CMAKE_BUILD_TYPE}")

#############################################################################################################
# Main library target
#############################################################################################################
add_library(${PROJECT_NAME}
        include/structural/basic_inplace_string.hpp
        include/structural/bitset.hpp
        include/structural/concept_structural_type_value.hpp
        include/structural/detail/hash_combine.hpp
        include/structural/detail/inplace_hash_table.hpp
        include/structural/detail/inplace_red_black_tree.hpp
        include/structural/detail/inplace_unordered_map_details.hpp
        include/structural/detail/split.hpp
        include/structural/detail/static_for.hpp
        include/structural/detail/string_view_like.hpp
        include/structural/detail/to_underlying.hpp
        include/structural/detail/trim.hpp
        include/structural/detail/tuple_impl.hpp
        include/structural/detail/uninitialized_array_details.hpp
        include/structural/hash.hpp
        include/structural/inplace_map.hpp
        include/structural/inplace_set.hpp
        include/structural/inplace_string.hpp
        include/structural/inplace_unordered_map.hpp
        include/structural/inplace_unordered_set.hpp
        include/structural/inplace_vector.hpp
        include/structural/named_bitset.hpp
        include/structural/pair.hpp
        include/structural/serialization/serialize_aggregate.hpp
        include/structural/serialization/serialize_arithmetic.hpp
        include/structural/serialization/serialize_chrono.hpp
        include/structural/serialization/serialize_enum.hpp
        include/structural/serialization/serialize_expected.hpp
        include/structural/serialization/serialize_optional.hpp
        include/structural/serialization/serialize_range.hpp
        include/structural/serialization/serialize_tuple_like.hpp
        include/structural/serialization/serialize_unique_ptr.hpp
        include/structural/serialization/serialize_variant.hpp
        include/structural/serialization/serializer.hpp
        include/structural/serialization.hpp
        include/structural/structural_constant.hpp
        include/structural/structuralization/structuralize_aggregate.hpp
        include/structural/structuralization/structuralize_optional.hpp
        include/structural/structuralization/structuralize_range.hpp
        include/structural/structuralization/structuralize_structural.hpp
        include/structural/structuralization/structuralize_tuple_like.hpp
        include/structural/structuralization/structuralize_unique_ptr.hpp
        include/structural/structuralization/structuralize_variant.hpp
        include/structural/structuralization/structuralizer.hpp
        include/structural/structuralize.hpp
        include/structural/tuple.hpp
        include/structural/uninitialized_array.hpp
        include/structural/wrapper.hpp
)
target_include_directories(
        ${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)
target_link_libraries(${PROJECT_NAME} PUBLIC
        ctrx::ctrx
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")

string(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)
packageProject(
        NAME ${PROJECT_NAME}
        VERSION ${PROJECT_VERSION}
        NAMESPACE ${PROJECT_NAME}
        BINARY_DIR ${PROJECT_BINARY_DIR}
        INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
        INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
        VERSION_HEADER "${VERSION_HEADER_LOCATION}"
        COMPATIBILITY SameMajorVersion
        DEPENDENCIES "ctrx 1.2.0"
)
