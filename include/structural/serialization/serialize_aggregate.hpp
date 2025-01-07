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

#ifndef STRUCTURAL_SERIALIZE_AGGREGATE_HPP
#define STRUCTURAL_SERIALIZE_AGGREGATE_HPP

#include "structural/serialization/serialize_tuple_like.hpp"

#include <iterator>
#include <tuple>
#include <utility>

#include <cstddef>

namespace structural
{
namespace detail
{
struct anything
{
    template<class T>
    /* implicit */ operator T() const;
};

template<typename T, typename... Ts>
concept direct_list_initializable = requires(Ts... ts) { T{ts...}; };

template<typename T, std::size_t N, typename = std::make_index_sequence<N>>
struct is_direct_list_initializable_n;

template<typename T, std::size_t N, std::size_t... Is>
struct is_direct_list_initializable_n<T, N, std::index_sequence<Is...>>
{
    template<std::size_t>
    using type = anything;

    static constexpr bool value = direct_list_initializable<T, type<Is>...>;
};
template<typename T, std::size_t N>
inline constexpr bool is_direct_list_initializable_n_v = is_direct_list_initializable_n<T, N>::value;

template<typename T>
constexpr auto to_tuple(T&& value)
{
    using type = std::decay_t<T>;
    static_assert(!is_direct_list_initializable_n_v<type, 11>,
                  "Aggregates supported up to size 10 - please provide custom serializer for this type.");

#define STRUCTURAL_GEN_ID_1 p1
#define STRUCTURAL_GEN_ID_2 STRUCTURAL_GEN_ID_1, p2
#define STRUCTURAL_GEN_ID_3 STRUCTURAL_GEN_ID_2, p3
#define STRUCTURAL_GEN_ID_4 STRUCTURAL_GEN_ID_3, p4
#define STRUCTURAL_GEN_ID_5 STRUCTURAL_GEN_ID_4, p5
#define STRUCTURAL_GEN_ID_6 STRUCTURAL_GEN_ID_5, p6
#define STRUCTURAL_GEN_ID_7 STRUCTURAL_GEN_ID_6, p7
#define STRUCTURAL_GEN_ID_8 STRUCTURAL_GEN_ID_7, p8
#define STRUCTURAL_GEN_ID_9 STRUCTURAL_GEN_ID_8, p9
#define STRUCTURAL_GEN_ID_10 STRUCTURAL_GEN_ID_9, p10
#define STRUCTURAL_GEN_ID(N) STRUCTURAL_GEN_ID_##N

#define STRUCTURAL_GEN_CASE(N, ...)                                                                                    \
    __VA_ARGS__ if constexpr (is_direct_list_initializable_n_v<type, N>)                                               \
    {                                                                                                                  \
        auto&& [STRUCTURAL_GEN_ID(N)] = std::forward<T>(value);                                                        \
        return std::tuple{STRUCTURAL_GEN_ID(N)};                                                                       \
    }
    STRUCTURAL_GEN_CASE(10)
    STRUCTURAL_GEN_CASE(9, else)
    STRUCTURAL_GEN_CASE(8, else)
    STRUCTURAL_GEN_CASE(7, else)
    STRUCTURAL_GEN_CASE(6, else)
    STRUCTURAL_GEN_CASE(5, else)
    STRUCTURAL_GEN_CASE(4, else)
    STRUCTURAL_GEN_CASE(3, else)
    STRUCTURAL_GEN_CASE(2, else)
    STRUCTURAL_GEN_CASE(1, else)
    else if constexpr (is_direct_list_initializable_n_v<type, 0>)
    {
        return std::tuple{};
    }
#undef STRUCTURAL_GEN_CASE
#undef STRUCTURAL_GEN_ID
#undef STRUCTURAL_GEN_ID_1
#undef STRUCTURAL_GEN_ID_2
#undef STRUCTURAL_GEN_ID_3
#undef STRUCTURAL_GEN_ID_4
#undef STRUCTURAL_GEN_ID_5
#undef STRUCTURAL_GEN_ID_6
#undef STRUCTURAL_GEN_ID_7
#undef STRUCTURAL_GEN_ID_8
#undef STRUCTURAL_GEN_ID_9
#undef STRUCTURAL_GEN_ID_10
}
} // namespace detail

template<typename T>
concept decomposable_aggregate = std::is_aggregate_v<T> && !tuple_like<T>;

template<decomposable_aggregate T>
struct serializer<T>
{
    static constexpr void do_serialize(T const& value, std::output_iterator<std::byte> auto& out_iter)
    {
        serialize(detail::to_tuple(value), out_iter);
    }

    static constexpr auto do_deserialize(std::in_place_type_t<T>, std::input_iterator auto& in_iter) -> T
    {
        return std::make_from_tuple<T>(
            deserialize(std::in_place_type<decltype(detail::to_tuple(std::declval<T>()))>, in_iter));
    }
};
} // namespace structural

#endif // STRUCTURAL_SERIALIZE_AGGREGATE_HPP
