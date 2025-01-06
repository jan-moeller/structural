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

#ifndef STRUCTURAL_SERIALIZE_CHRONO_HPP
#define STRUCTURAL_SERIALIZE_CHRONO_HPP

#include "structural/serialization/serializer.hpp"

#include <chrono>
#include <iterator>
#include <type_traits>

#include <cstddef>

namespace structural
{
template<typename Rep, typename Period>
struct serializer<std::chrono::duration<Rep, Period>>
{
    static constexpr void do_serialize(std::chrono::duration<Rep, Period>    value,
                                       std::output_iterator<std::byte> auto& out_iter)
    {
        return serialize(value.count(), out_iter);
    }

    static constexpr auto do_deserialize(std::in_place_type_t<std::chrono::duration<Rep, Period>>,
                                         std::input_iterator auto& in_iter) -> std::chrono::duration<Rep, Period>
    {
        return std::chrono::duration<Rep, Period>(deserialize(std::in_place_type<Rep>, in_iter));
    }
};

template<typename Clock, typename Duration>
struct serializer<std::chrono::time_point<Clock, Duration>>
{
    static constexpr void do_serialize(std::chrono::time_point<Clock, Duration> value,
                                       std::output_iterator<std::byte> auto&    out_iter)
    {
        return serialize(value.time_since_epoch(), out_iter);
    }

    static constexpr auto do_deserialize(std::in_place_type_t<std::chrono::time_point<Clock, Duration>>,
                                         std::input_iterator auto& in_iter) -> std::chrono::time_point<Clock, Duration>
    {
        return std::chrono::time_point<Clock, Duration>(deserialize(std::in_place_type<Duration>, in_iter));
    }
};
} // namespace structural

#endif // STRUCTURAL_SERIALIZE_CHRONO_HPP
