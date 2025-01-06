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

#ifndef STRUCTURALIZER_HPP
#define STRUCTURALIZER_HPP

#include "structural/concept_structural_type_value.hpp"
#include "structural/wrapper.hpp"

namespace structural
{
template<typename T, wrapper WrappedValue>
struct structuralizer;

template<typename T, wrapper WrappedValue>
consteval auto structuralize()
{
    return structuralizer<T, WrappedValue>::do_structuralize();
}

#define STRUCTURALIZE(...)                                                                                             \
    structural::structuralize<std::remove_cvref_t<decltype(__VA_ARGS__)>, STRUCTURAL_WRAP(__VA_ARGS__)>()
} // namespace structural

#endif // STRUCTURALIZER_HPP
