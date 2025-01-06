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

#ifndef STRUCTURALIZE_UNIQUE_PTR_HPP
#define STRUCTURALIZE_UNIQUE_PTR_HPP

#include <version>
#if __cpp_lib_constexpr_memory >= 202202L

#include "structural/structuralization/structuralizer.hpp"

#include <memory>

namespace structural
{
template<typename T, wrapper WrappedValue>
struct structuralizer<std::unique_ptr<T>, WrappedValue>
{
    static consteval auto do_structuralize()
    {
        if constexpr (WrappedValue.unwrap() != nullptr)
            return STRUCTURALIZE(*WrappedValue.unwrap());
        else
            return nullptr;
    }
};
} // namespace structural

#endif

#endif // STRUCTURALIZE_UNIQUE_PTR_HPP
