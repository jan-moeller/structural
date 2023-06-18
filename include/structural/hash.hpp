//
// MIT License
//
// Copyright (c) 2022 Jan Möller
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

#ifndef STRUCTURAL_HASH_HPP
#define STRUCTURAL_HASH_HPP

#include "structural/detail/hash_combine.hpp"
#include "structural/detail/static_for.hpp"

#include <bit>
#include <bitset>
#include <concepts>
#include <memory>
#include <optional>
#include <ranges>
#include <typeindex>
#include <variant>

#include <climits>
#include <cstddef>
#include <cstdint>

namespace structural
{
template<typename T>
struct hash;

template<std::integral T>
struct hash<T>
{
    constexpr auto operator()(T const& value) const noexcept -> std::size_t
    {
        return std::bit_cast<std::make_unsigned_t<T>>(value);
    }
};

template<std::floating_point T>
struct hash<T>
{
    constexpr auto operator()(T const& value) const noexcept -> std::size_t
    {
        if (value == 0)
            return 0;
        auto const bytes = std::bit_cast<std::array<std::byte, sizeof(T)>>(value);
        return detail::hash_bytes(bytes.data(), sizeof(T));
    }
};

template<>
struct hash<nullptr_t>
{
    constexpr auto operator()(nullptr_t const& /*value*/) const noexcept -> std::size_t { return 0; }
};

template<typename T>
struct hash<T*>
{
    auto operator()(T* const& value) const noexcept -> std::size_t { return reinterpret_cast<std::uintptr_t>(value); }
};

template<typename T>
    requires(std::is_enum_v<T>)
struct hash<T>
{
    constexpr auto operator()(T const& value) const noexcept -> std::size_t
    {
        using U = std::underlying_type_t<T>;
        return hash<U>{}(static_cast<U>(value));
    }
};

template<std::ranges::range T>
struct hash<T>
{
    constexpr auto operator()(T const& value) const -> std::size_t
    {
        std::size_t h = 0;
        for (auto&& e : value)
            h = detail::hash_combine(h, hash<typename T::value_type>{}(e));
        return h;
    }
};

template<typename T>
struct hash<std::optional<T>>
{
    constexpr auto operator()(std::optional<T> const& opt) const -> std::size_t
    {
        if (opt)
            return hash<T>{}(opt.value());
        return 0;
    }
};

template<typename... Ts>
struct hash<std::variant<Ts...>>
{
    constexpr auto operator()(std::variant<Ts...> const& value) const -> std::size_t
    {
        return std::visit([]<typename T>(T const& e) { return hash<T>{}(e); }, value);
    }
};

template<>
struct hash<std::monostate>
{
    constexpr auto operator()(std::monostate const& /*value*/) const noexcept -> std::size_t { return 0; }
};

template<std::size_t N>
struct hash<std::bitset<N>>
{
    constexpr auto operator()(std::bitset<N> const& value) const noexcept -> std::size_t
    {
        if constexpr (N <= sizeof(unsigned long) * CHAR_BIT)
            return hash<unsigned long>{}(value.to_ulong());
        else if constexpr (N <= sizeof(unsigned long long) * CHAR_BIT)
            return hash<unsigned long long>{}(value.to_ulonglong());
        else
            return hash<std::string>{}(value.to_string());
    }
};

template<typename T, typename Deleter>
struct hash<std::unique_ptr<T, Deleter>>
{
    constexpr auto operator()(std::unique_ptr<T, Deleter> const& value) const -> std::size_t
    {
        return hash<typename std::unique_ptr<T, Deleter>::pointer>{}(value.get());
    }
};

template<typename T>
struct hash<std::shared_ptr<T>>
{
    constexpr auto operator()(std::shared_ptr<T> const& value) const -> std::size_t
    {
        return hash<typename std::shared_ptr<T>::element_type*>{}(value.get());
    }
};

template<>
struct hash<std::type_index>
{
    auto operator()(std::type_index const& value) const noexcept -> std::size_t { return value.hash_code(); }
};
} // namespace structural

#endif // STRUCTURAL_HASH_HPP
