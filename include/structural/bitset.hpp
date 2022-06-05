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

#ifndef STRUCTURAL_BITSET_HPP
#define STRUCTURAL_BITSET_HPP

#include <algorithm>
#include <array>
#include <bit>
#include <numeric>
#include <ostream>

#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>

namespace structural
{
template<std::size_t N>
struct bitset;

namespace detail
{
template<std::size_t N>
constexpr std::size_t bits_per_chunk = sizeof(typename bitset<N>::chunk_t) * CHAR_BIT;

template<std::size_t N>
constexpr std::size_t chunk_size = 1 + ((N - 1) / bits_per_chunk<N>);

template<std::size_t N>
constexpr auto chunk_index_for(bitset<N> const& bs, std::size_t pos) noexcept -> std::size_t
{
    assert(pos < N);
    assert(N > 0);
    return bs.chunks.size() - pos / bits_per_chunk<N> - 1;
}

template<std::size_t N>
constexpr auto chunk_for(bitset<N> const& bs, std::size_t pos) noexcept -> typename bitset<N>::chunk_t const&
{
    return bs.chunks[chunk_index_for(bs, pos)];
}

template<std::size_t N>
constexpr auto chunk_for(bitset<N>& bs, std::size_t pos) noexcept -> typename bitset<N>::chunk_t&
{
    return bs.chunks[chunk_index_for(bs, pos)];
}

template<std::size_t N, typename CharT, typename Traits = std::char_traits<CharT>>
constexpr void
set_from_str(bitset<N>& bs, std::basic_string_view<CharT, Traits> sv, CharT zero = CharT('0'), CharT one = CharT('1'))
{
    for (unsigned i = sv.size(); i > 0u; --i)
    {
        if (sv[i - 1] == one)
            bs.set(N - i, true);
        else if (sv[i - 1] == zero)
            bs.set(N - i, false);
        else
            throw std::invalid_argument{"invalid character in bitset initializer string"};
    }
}
} // namespace detail

template<std::size_t N>
struct bitset
{
    using chunk_t = std::uint8_t;

    constexpr bitset() noexcept = default;
    constexpr explicit bitset(unsigned long long val) noexcept
    {
        for (unsigned i = 0u; i < (sizeof(val) * CHAR_BIT); ++i)
            if ((val >> i) & 1u)
                set(i);
    }
    template<typename CharT = char, typename Traits = std::char_traits<CharT>>
    constexpr explicit bitset(std::basic_string_view<CharT, Traits> sv, CharT zero = CharT('0'), CharT one = CharT('1'))
    {
        detail::set_from_str(*this, sv, zero, one);
    }
    constexpr explicit bitset(std::string_view sv, char zero = '0', char one = '1')
    {
        detail::set_from_str(*this, sv, zero, one);
    }

    constexpr auto operator==(bitset const& rhs) const noexcept -> bool = default;

    [[nodiscard]] constexpr auto test(std::size_t pos) const noexcept -> bool
    {
        auto const chunk = detail::chunk_for(*this, pos);
        auto const n     = pos % detail::bits_per_chunk<N>;
        return (chunk >> n) & 1u;
    }

    [[nodiscard]] constexpr auto test_all(bitset const& mask) const noexcept -> bool
    {
        for (std::size_t i = 0; i < chunks.size(); ++i)
            if ((chunks[i] & mask.chunks[i]) != mask.chunks[i])
                return false;
        return true;
    }

    [[nodiscard]] constexpr auto test_any(bitset const& mask) const noexcept -> bool
    {
        for (std::size_t i = 0; i < chunks.size(); ++i)
            if ((chunks[i] & mask.chunks[i]) != 0)
                return true;
        return false;
    }

    [[nodiscard]] constexpr auto test_none(bitset const& mask) const noexcept -> bool { return !test_any(mask); }

    [[nodiscard]] constexpr auto all() const noexcept -> bool
    {
        for (std::size_t i = 0; i < N; ++i)
            if (!test(i))
                return false;
        return true;
    }

    [[nodiscard]] constexpr auto any() const noexcept -> bool
    {
        for (std::size_t i = 0; i < N; ++i)
            if (test(i))
                return true;
        return false;
    }

    [[nodiscard]] constexpr auto none() const noexcept -> bool
    {
        for (std::size_t i = 0; i < N; ++i)
            if (test(i))
                return false;
        return true;
    }

    [[nodiscard]] constexpr auto count() const noexcept
    {
        return std::reduce(chunks.begin(),
                           chunks.end(),
                           0,
                           [](std::size_t cur, chunk_t c) { return cur + std::popcount(c); });
    }
    [[nodiscard]] constexpr auto size() const noexcept { return N; }

    [[maybe_unused]] constexpr auto set() noexcept -> bitset&
    {
        for (std::size_t i = 0; i < N; ++i)
            set(i);
        return *this;
    }

    [[maybe_unused]] constexpr auto set(std::size_t pos, bool value = true) noexcept -> bitset&
    {
        auto&      chunk = detail::chunk_for(*this, pos);
        auto const n     = pos % detail::bits_per_chunk<N>;
        chunk            = (chunk & ~(1UL << n)) | (value << n);
        return *this;
    }

    [[maybe_unused]] constexpr auto reset() noexcept -> bitset&
    {
        for (std::size_t i = 0; i < N; ++i)
            reset(i);
        return *this;
    }

    [[maybe_unused]] constexpr auto reset(std::size_t pos) noexcept -> bitset&
    {
        set(pos, false);
        return *this;
    }

    [[maybe_unused]] constexpr auto flip() noexcept -> bitset&
    {
        for (std::size_t i = 0; i < N; ++i)
            flip(i);
        return *this;
    }

    [[maybe_unused]] constexpr auto flip(std::size_t pos) noexcept -> bitset&
    {
        auto&      chunk = detail::chunk_for(*this, pos);
        auto const n     = pos % detail::bits_per_chunk<N>;
        chunk ^= 1u << n;
        return *this;
    }

    template<class CharT = char, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT>>
    [[nodiscard]] auto to_string(CharT zero = CharT('0'), CharT one = CharT('1')) const
        -> std::basic_string<CharT, Traits, Allocator>
    {
        std::basic_string<CharT, Traits, Allocator> str;
        for (std::size_t i = size(); i > 0; --i)
            if (test(i - 1))
                str += one;
            else
                str += zero;
        return str;
    }

    [[nodiscard]] constexpr auto to_ulong() const -> unsigned long
    {
        if (sizeof(unsigned long) * CHAR_BIT < N)
            throw std::overflow_error{"Bitset cannot be represented by unsigned long"};

        return to_ullong();
    }

    [[nodiscard]] constexpr auto to_ullong() const -> unsigned long long
    {
        if (sizeof(unsigned long long) * CHAR_BIT < N)
            throw std::overflow_error{"Bitset cannot be represented by unsigned long long"};

        unsigned long long result = 0u;
        for (std::size_t i = 0; i < chunks.size(); ++i)
            result |= (chunks[chunks.size() - i - 1] << (i * detail::bits_per_chunk<N>));
        return result;
    }

    constexpr auto operator&=(bitset const& rhs) noexcept -> bitset&
    {
        for (std::size_t i = 0; i < chunks.size(); ++i)
            chunks[i] &= rhs.chunks[i];
        return *this;
    }

    constexpr auto operator|=(bitset const& rhs) noexcept -> bitset&
    {
        for (std::size_t i = 0; i < chunks.size(); ++i)
            chunks[i] |= rhs.chunks[i];
        return *this;
    }

    constexpr auto operator^=(bitset const& rhs) noexcept -> bitset&
    {
        for (std::size_t i = 0; i < chunks.size(); ++i)
            chunks[i] ^= rhs.chunks[i];
        return *this;
    }

    [[nodiscard]] constexpr auto operator~() const noexcept -> bitset
    {
        auto copy = *this;
        copy.flip();
        return copy;
    }

    constexpr auto operator<<=(std::size_t pos) noexcept -> bitset&
    {
        constexpr auto bits_per_chunk = detail::bits_per_chunk<N>;
        for (std::size_t i = 0; i < chunks.size(); ++i)
        {
            auto const next_chunk     = i + 1 < chunks.size() ? chunks[i + 1] : 0u;
            auto const bits_from_next = next_chunk >> (bits_per_chunk - pos);
            auto const shifted        = chunks[i] << pos;
            chunks[i]                 = shifted | bits_from_next;
        }
        if constexpr (N > 0)
            chunks.front() &= (0xffu >> (bits_per_chunk - N % bits_per_chunk));
        return *this;
    }

    constexpr auto operator<<(std::size_t pos) const noexcept -> bitset
    {
        auto copy = *this;
        copy <<= pos;
        return copy;
    }

    constexpr auto operator>>=(std::size_t pos) noexcept -> bitset&
    {
        for (std::size_t i = chunks.size(); i > 0; --i)
        {
            auto const n  = i > 1 ? chunks[i - 2] : 0;
            chunks[i - 1] = (chunks[i - 1] >> pos) | (n << (detail::bits_per_chunk<N> - pos));
        }
        return *this;
    }

    constexpr auto operator>>(std::size_t pos) const noexcept -> bitset
    {
        auto copy = *this;
        copy >>= pos;
        return copy;
    }

    std::array<chunk_t, detail::chunk_size<N>> chunks{};
};

template<std::size_t N>
constexpr auto operator&(bitset<N> const& lhs, bitset<N> const& rhs) noexcept -> bitset<N>
{
    auto copy = lhs;
    copy &= rhs;
    return copy;
}

template<std::size_t N>
constexpr auto operator|(bitset<N> const& lhs, bitset<N> const& rhs) noexcept -> bitset<N>
{
    auto copy = lhs;
    copy |= rhs;
    return copy;
}

template<std::size_t N>
constexpr auto operator^(bitset<N> const& lhs, bitset<N> const& rhs) noexcept -> bitset<N>
{
    auto copy = lhs;
    copy ^= rhs;
    return copy;
}

template<std::size_t N>
constexpr auto operator<<(std::ostream& os, bitset<N> const& bs) noexcept -> std::ostream&
{
    return os << bs.to_string();
}

template<char... Cs>
constexpr auto operator"" _bits() noexcept
{
    constexpr std::array  str{Cs...};
    constexpr std::size_t size = sizeof...(Cs) - std::ranges::count(str, '\'');
    bitset<size>          bs;

    int offset = 0;
    for (std::size_t i = 0; i < str.size(); ++i)
    {
        char const c = str[str.size() - i - 1];
        assert(c == '0' || c == '1' || c == '\'');
        if (c == '1')
            bs.set(i - offset);
        if (c == '\'')
            ++offset;
    }
    return bs;
}
} // namespace structural

namespace std
{
template<std::size_t N>
struct hash<structural::bitset<N>>
{
    constexpr auto operator()(structural::bitset<N> const& bs) const noexcept -> std::size_t
    {
        std::size_t    hash      = 0u;
        constexpr auto max_width = sizeof(std::size_t) * CHAR_BIT;
        for (std::size_t i = 0; i < bs.chunks.size(); ++i)
        {
            auto const offset = i * sizeof(typename structural::bitset<N>::chunk_t) * CHAR_BIT;
            hash ^= std::size_t(bs.chunks[bs.chunks.size() - i - 1]) << (offset % max_width);
        }
        return hash;
    }
};
} // namespace std

#endif // STRUCTURAL_BITSET_HPP
