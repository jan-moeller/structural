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

#ifndef STRUCTURAL_WRAPPER_HPP
#define STRUCTURAL_WRAPPER_HPP

#include "structural/serialization.hpp"

#include <array>
#include <iterator>
#include <utility>

#include <cstddef>

namespace structural
{
namespace detail
{
template<typename T>
consteval auto serialized_size(T const& t) -> std::size_t
{
    std::vector<std::byte> v;

    auto iter = std::back_inserter(v);
    serialize(t, iter);

    return v.size();
}
} // namespace detail

template<typename T, std::size_t N>
struct wrapper
{
    using wrapped_type = T;

    consteval explicit wrapper(T const& t)
    {
        auto iter = data.begin();
        serialize(t, iter);
    }

    constexpr auto unwrap() const -> T
    {
        auto iter = data.begin();
        return deserialize<T>(std::in_place_type<T>, iter);
    }

    std::array<std::byte, N> data{};
};

#define STRUCTURAL_WRAP(...)                                                                                           \
    structural::wrapper<std::remove_cvref_t<decltype(__VA_ARGS__)>, structural::detail::serialized_size(__VA_ARGS__)>  \
    {                                                                                                                  \
        __VA_ARGS__                                                                                                    \
    }
} // namespace structural

#endif // STRUCTURAL_WRAPPER_HPP
