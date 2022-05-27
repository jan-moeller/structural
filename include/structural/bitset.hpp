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

#include <array>
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
} // namespace detail

template<std::size_t N>
struct bitset
{
    using chunk_t = std::uint8_t;

    constexpr auto operator==(bitset const& rhs) const noexcept -> bool = default;

    [[nodiscard]] constexpr auto test(std::size_t pos) const noexcept -> bool
    {
        auto const chunk = detail::chunk_for(*this, pos);
        auto const n     = pos % detail::bits_per_chunk<N>;
        return (chunk >> n) & 1u;
    }

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
    for (std::size_t i = bs.size(); i > 0; --i)
        if (bs.test(i - 1))
            os << '1';
        else
            os << '0';
    return os;
}

template<char... Cs>
constexpr auto operator"" _bits() noexcept -> bitset<sizeof...(Cs)>
{
    constexpr std::size_t size = sizeof...(Cs);
    bitset<size>          bs;
    std::array            str{Cs...};
    for (std::size_t i = 0; i < str.size(); ++i)
    {
        assert(str[i] == '0' || str[i] == '1');
        if (str[i] == '1')
            bs.set(size - i - 1);
    }
    return bs;
}
} // namespace structural

#endif // STRUCTURAL_BITSET_HPP
