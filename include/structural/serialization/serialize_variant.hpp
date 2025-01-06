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

#ifndef STRUCTURAL_SERIALIZE_VARIANT_HPP
#define STRUCTURAL_SERIALIZE_VARIANT_HPP

#include "structural/serialization/serializer.hpp"

#include <iterator>
#include <utility>
#include <variant>

#include <cstddef>

namespace structural
{
template<>
struct serializer<std::monostate>
{
    static constexpr void do_serialize(std::monostate const& /*value*/,
                                       std::output_iterator<std::byte> auto& /*out_iter*/)
    {
        // intentionally empty
    }

    static constexpr auto do_deserialize(std::in_place_type_t<std::monostate>, std::input_iterator auto& /*in_iter*/)
        -> std::monostate
    {
        return std::monostate{};
    }
};

template<typename... Ts>
struct serializer<std::variant<Ts...>>
{
    static constexpr void do_serialize(std::variant<Ts...> const&            variant,
                                       std::output_iterator<std::byte> auto& out_iter)
    {
        serialize(variant.index(), out_iter);
        std::visit([&out_iter](auto const& v) { serialize(v, out_iter); }, variant);
    }

    static constexpr auto do_deserialize(std::in_place_type_t<std::variant<Ts...>>, std::input_iterator auto& in_iter)
        -> std::variant<Ts...>
    {
        auto const index = deserialize(std::in_place_type<std::size_t>, in_iter);

        auto const create = [&in_iter, index]<std::size_t I>(auto const& self)
        {
            if (I == index)
            {
                constexpr auto t = std::in_place_type<std::variant_alternative_t<I, std::variant<Ts...>>>;
                return std::variant<Ts...>(std::in_place_index<I>, deserialize(t, in_iter));
            }

            if constexpr (I + 1 < sizeof...(Ts))
                return self.template operator()<I + 1>(self);
            else
                throw std::bad_variant_access();
        };
        return create.template operator()<0>(create);
    }
};
} // namespace structural

#endif // STRUCTURAL_SERIALIZE_VARIANT_HPP
