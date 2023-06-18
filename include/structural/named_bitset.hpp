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

#ifndef STRUCTURAL_NAMED_BITSET_HPP
#define STRUCTURAL_NAMED_BITSET_HPP

#include "bitset.hpp"
#include "structural/detail/split.hpp"
#include "structural/detail/to_underlying.hpp"
#include "structural/detail/trim.hpp"

#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>

#include <cstddef>

#define STRUCTURAL_MAKE_NAMED_BITSET_IMPL_GEN_OP(op, enum_name, bitset_name)                                           \
    constexpr auto operator op(enum_name lhs, enum_name rhs) noexcept -> bitset_name                                   \
    {                                                                                                                  \
        return bitset_name{lhs} op bitset_name{rhs};                                                                   \
    }                                                                                                                  \
    constexpr auto operator op(enum_name lhs, bitset_name rhs) noexcept -> bitset_name                                 \
    {                                                                                                                  \
        return bitset_name{lhs} op rhs;                                                                                \
    }                                                                                                                  \
    constexpr auto operator op(bitset_name lhs, enum_name rhs) noexcept -> bitset_name                                 \
    {                                                                                                                  \
        return lhs op bitset_name{rhs};                                                                                \
    }
#define STRUCTURAL_MAKE_NAMED_BITSET(enum_name, bitset_name, ...)                                                      \
    enum class enum_name                                                                                               \
    {                                                                                                                  \
        __VA_ARGS__                                                                                                    \
    };                                                                                                                 \
    static constexpr std::size_t bitset_name##_size = std::ranges::count(std::string_view{#__VA_ARGS__}, ',') + 1;     \
    using bitset_name                               = structural::named_bitset<enum_name, bitset_name##_size>;         \
    STRUCTURAL_MAKE_NAMED_BITSET_IMPL_GEN_OP(&, enum_name, bitset_name)                                                \
    STRUCTURAL_MAKE_NAMED_BITSET_IMPL_GEN_OP(|, enum_name, bitset_name)                                                \
    STRUCTURAL_MAKE_NAMED_BITSET_IMPL_GEN_OP(^, enum_name, bitset_name)                                                \
                                                                                                                       \
    constexpr auto to_string_view(enum_name e)->std::string_view                                                       \
    {                                                                                                                  \
        constexpr auto names = []()                                                                                    \
        {                                                                                                              \
            auto names = structural::detail::split<bitset_name##_size>(#__VA_ARGS__, ',');                             \
            std::ranges::transform(names,                                                                              \
                                   names.begin(),                                                                      \
                                   [](std::string_view sv) { return structural::detail::trim(sv); });                  \
            return names;                                                                                              \
        }();                                                                                                           \
        auto const idx = structural::detail::to_underlying(e);                                                         \
        assert(std::size_t(idx) < names.size());                                                                                    \
        return names[idx];                                                                                             \
    }                                                                                                                  \
    auto to_string(enum_name e)->std::string                                                                           \
    {                                                                                                                  \
        return std::string{to_string_view(e)};                                                                         \
    } // TODO: Make constexpr as soon as std lib supports constexpr std::string

namespace structural
{
template<typename Enum, std::size_t N>
    requires(std::is_enum_v<Enum>)
struct named_bitset
{
    constexpr named_bitset() = default;
    constexpr /* implicit */ named_bitset(Enum e) { bits.set(detail::to_underlying(e)); }

    constexpr auto operator==(named_bitset const&) const noexcept -> bool = default;

    [[nodiscard]] constexpr auto test(Enum e) const noexcept -> bool { return bits.test(detail::to_underlying(e)); }
    [[nodiscard]] constexpr auto test_all(named_bitset const& mask) const noexcept -> bool
    {
        return bits.test_all(mask.bits);
    }

    [[nodiscard]] constexpr auto test_any(named_bitset const& mask) const noexcept -> bool
    {
        return bits.test_any(mask.bits);
    }

    [[nodiscard]] constexpr auto test_none(named_bitset const& mask) const noexcept -> bool
    {
        return bits.test_none(mask.bits);
    }

    [[nodiscard]] constexpr auto all() const noexcept -> bool { return bits.all(); }

    [[nodiscard]] constexpr auto any() const noexcept -> bool { return bits.any(); }

    [[nodiscard]] constexpr auto none() const noexcept -> bool { return bits.none(); }

    [[nodiscard]] constexpr auto        count() const noexcept { return bits.count(); }
    [[nodiscard]] static constexpr auto size() noexcept { return N; }

    [[maybe_unused]] constexpr auto set() noexcept -> named_bitset&
    {
        bits.set();
        return *this;
    }

    [[maybe_unused]] constexpr auto set(Enum e, bool value = true) noexcept -> named_bitset&
    {
        bits.set(detail::to_underlying(e), value);
        return *this;
    }

    [[maybe_unused]] constexpr auto reset() noexcept -> named_bitset&
    {
        bits.reset();
        return *this;
    }

    [[maybe_unused]] constexpr auto reset(Enum e) noexcept -> named_bitset&
    {
        bits.reset(detail::to_underlying(e));
        return *this;
    }

    [[maybe_unused]] constexpr auto flip() noexcept -> named_bitset&
    {
        bits.flip();
        return *this;
    }

    [[maybe_unused]] constexpr auto flip(Enum e) noexcept -> named_bitset&
    {
        bits.flip(detail::to_underlying(e));
        return *this;
    }

    template<class CharT = char, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT>>
    [[nodiscard]] auto to_string() const -> std::basic_string<CharT, Traits, Allocator>
    {
        std::basic_string<CharT, Traits, Allocator> str;
        for (std::size_t i = 0; i < bits.size(); ++i)
            if (bits.test(i))
            {
                if (!str.empty())
                    str += " | ";
                auto const sv = to_string_view(Enum(i));
                str += sv;
            }
        return str;
    }

    [[nodiscard]] constexpr auto to_ulong() const -> unsigned long { return bits.to_ulong(); }
    [[nodiscard]] constexpr auto to_ullong() const -> unsigned long long { return bits.to_ullong(); }

    constexpr auto operator&=(named_bitset const& rhs) noexcept -> named_bitset&
    {
        bits &= rhs.bits;
        return *this;
    }

    constexpr auto operator|=(named_bitset const& rhs) noexcept -> named_bitset&
    {
        bits |= rhs.bits;
        return *this;
    }

    constexpr auto operator^=(named_bitset const& rhs) noexcept -> named_bitset&
    {
        bits ^= rhs.bits;
        return *this;
    }

    [[nodiscard]] constexpr auto operator~() const noexcept -> named_bitset
    {
        auto copy = *this;
        copy.flip();
        return copy;
    }

    constexpr auto operator<<=(std::size_t pos) noexcept -> named_bitset&
    {
        bits <<= pos;
        return *this;
    }

    constexpr auto operator<<(std::size_t pos) const noexcept -> named_bitset
    {
        auto copy = *this;
        copy <<= pos;
        return copy;
    }

    constexpr auto operator>>=(std::size_t pos) noexcept -> named_bitset&
    {
        bits >>= pos;
        return *this;
    }

    constexpr auto operator>>(std::size_t pos) const noexcept -> named_bitset
    {
        auto copy = *this;
        copy >>= pos;
        return copy;
    }

    bitset<N> bits;
};

template<typename Enum, std::size_t N>
    requires(std::is_enum_v<Enum>)
constexpr auto operator&(named_bitset<Enum, N> const& lhs, named_bitset<Enum, N> const& rhs) noexcept
    -> named_bitset<Enum, N>
{
    auto copy = lhs;
    copy &= rhs;
    return copy;
}

template<typename Enum, std::size_t N>
    requires(std::is_enum_v<Enum>)
constexpr auto operator|(named_bitset<Enum, N> const& lhs, named_bitset<Enum, N> const& rhs) noexcept
    -> named_bitset<Enum, N>
{
    auto copy = lhs;
    copy |= rhs;
    return copy;
}

template<typename Enum, std::size_t N>
    requires(std::is_enum_v<Enum>)
constexpr auto operator^(named_bitset<Enum, N> const& lhs, named_bitset<Enum, N> const& rhs) noexcept
    -> named_bitset<Enum, N>
{
    auto copy = lhs;
    copy ^= rhs;
    return copy;
}

template<typename Enum, std::size_t N>
    requires(std::is_enum_v<Enum>)
constexpr auto operator<<(std::ostream& os, named_bitset<Enum, N> const& bs) noexcept -> std::ostream&
{
    return os << bs.to_string();
}
} // namespace structural

#endif // STRUCTURAL_NAMED_BITSET_HPP
