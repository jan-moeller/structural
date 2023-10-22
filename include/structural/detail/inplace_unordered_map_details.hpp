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

#ifndef STRUCTURAL_INPLACE_UNORDERED_MAP_DETAILS_HPP
#define STRUCTURAL_INPLACE_UNORDERED_MAP_DETAILS_HPP

#include "structural/detail/inplace_hash_table.hpp"
#include "structural/pair.hpp"

#include <cstddef>

namespace structural::detail::inplace_unordered_map
{
template<typename Key, typename T, typename Hash>
struct hasher_t
{
    Hash hasher;
    constexpr hasher_t(Hash const& hash)
        : hasher(hash)
    {
    }

    constexpr auto operator()(Key const& k) const -> std::size_t { return hasher(k); }
    constexpr auto operator()(structural::pair<Key const, T> const& p) const -> std::size_t { return hasher(p.first); }
};

template<typename Key, typename T, typename Equals>
struct equals_t
{
    Equals equals;
    constexpr equals_t(Equals const& equals)
        : equals(equals)
    {
    }

    constexpr auto operator()(structural::pair<Key const, T> const& lhs,
                              structural::pair<Key const, T> const& rhs) const -> bool
    {
        return equals(lhs.first, rhs.first);
    }
    constexpr auto operator()(Key const& lhs, structural::pair<Key const, T> const& rhs) const -> bool
    {
        return equals(lhs, rhs.first);
    }
    constexpr auto operator()(structural::pair<Key const, T> const& lhs, Key const& rhs) const -> bool
    {
        return equals(lhs.first, rhs);
    }
    constexpr auto operator()(Key const& lhs, Key const& rhs) const -> bool { return equals(lhs, rhs); }
};

template<typename Key, typename T, std::size_t Capacity, typename Hash, typename Equal>
using hashmap_t = detail::
    inplace_hash_table<structural::pair<Key const, T>, Capacity, hasher_t<Key, T, Hash>, equals_t<Key, T, Equal>>;
} // namespace structural::detail::inplace_unordered_map

#endif // STRUCTURAL_INPLACE_UNORDERED_MAP_DETAILS_HPP
