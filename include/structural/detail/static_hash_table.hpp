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

#ifndef STRUCTURAL_STATIC_HASH_TABLE_HPP
#define STRUCTURAL_STATIC_HASH_TABLE_HPP

#include "structural/hash.hpp"
#include "structural/pair.hpp"

#include <algorithm>
#include <array>
#include <functional>
#include <utility>

#include <cassert>
#include <cstddef>

namespace structural::detail
{
template<typename T, std::size_t Capacity, typename Hash, typename Equal, bool Const>
struct static_hash_table_iterator;

template<typename T, std::size_t Capacity, typename Hash = hash<T>, typename Equal = std::equal_to<T>>
struct static_hash_table
{
    using key_type        = T;
    using value_type      = T;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using hasher          = Hash;
    using key_equal       = Equal;
    using reference       = value_type&;
    using const_reference = value_type const&;
    using pointer         = value_type*;
    using const_pointer   = value_type const*;
    using iterator        = static_hash_table_iterator<T, Capacity, Hash, Equal, false>;
    using const_iterator  = static_hash_table_iterator<T, Capacity, Hash, Equal, true>;

    static constexpr size_type s_invalid_idx = -1;

    struct bucket_t
    {
        size_type idx = s_invalid_idx;
    };
    struct active_node_t
    {
        T           payload;
        std::size_t next = s_invalid_idx;

        template<typename... Ts>
        constexpr active_node_t(Ts&&... ts)
            : payload(std::forward<Ts>(ts)...)
        {
        }
    };
    struct inactive_node_t
    {
        std::size_t next_free_idx = s_invalid_idx;
    };
    union node_t
    {
        inactive_node_t inactive{};
        active_node_t   active;

        constexpr node_t() noexcept
            : inactive(inactive_node_t{})
        {
        }

        constexpr ~node_t() noexcept {}
    };

    constexpr static_hash_table() noexcept
        : static_hash_table(Hash())
    {
    }

    constexpr explicit static_hash_table(Hash const& hash, key_equal const& equal = key_equal())
        : hash_fn(hash)
        , equal_fn(equal)
    {
        size_type i = 0;
        for (auto& n : nodes)
            n.inactive.next_free_idx = ++i;
        buckets.fill(bucket_t{s_invalid_idx});
    }

    template<std::forward_iterator First, std::sentinel_for<First> Last>
    constexpr static_hash_table(First&&          first,
                                Last&&           last,
                                Hash const&      hash  = Hash(),
                                key_equal const& equal = key_equal())
        : static_hash_table(hash, equal)
    {
        for (auto iter = first; iter != last; ++iter)
            insert(*std::forward<First>(iter));
    }

    constexpr static_hash_table(static_hash_table const& other)
        : static_hash_table(other.begin(), other.end(), other.hash_fn, other.equal_fn)
    {
    }

    constexpr static_hash_table(static_hash_table&& other)
        : static_hash_table(std::move(other).begin(),
                            std::move(other).end(),
                            std::move(other.hash_fn),
                            std::move(other.equal_fn))
    {
    }

    constexpr ~static_hash_table() { clear(); }

    constexpr auto operator=(static_hash_table const& other) -> static_hash_table&
    {
        if (this != &other)
        {
            clear();
            for (auto&& e : other)
                insert(e);
        }
        return *this;
    }

    constexpr auto operator=(static_hash_table&& other) -> static_hash_table&
    {
        if (this != &other)
        {
            clear();
            for (auto&& e : std::move(other))
                insert(std::move(e));
            other.clear();
        }
        return *this;
    }

    template<typename... Ks>
    constexpr auto allocate_node(Ks&&... values) -> size_type
    {
        assert(next_available_idx < Capacity);
        size_type const idx = next_available_idx;
        next_available_idx  = nodes[idx].inactive.next_free_idx;
        std::construct_at(&nodes[idx].active, std::forward<Ks>(values)...);
        ++node_count;
        return idx;
    }

    constexpr void deallocate_node(size_type idx)
    {
        assert(idx != s_invalid_idx);
        std::destroy_at(&nodes[idx].active);
        nodes[idx].inactive = inactive_node_t{.next_free_idx = next_available_idx};
        next_available_idx  = idx;
        --node_count;
    }

    constexpr auto get_node(size_type idx) noexcept -> active_node_t&
    {
        assert(idx != s_invalid_idx);
        return nodes[idx].active;
    }
    constexpr auto get_node(size_type idx) const noexcept -> active_node_t const&
    {
        assert(idx != s_invalid_idx);
        return nodes[idx].active;
    }

    constexpr void clear() noexcept
    {
        for (auto&& b : buckets)
        {
            if (b.idx != s_invalid_idx)
            {
                size_type next = get_node(b.idx).next;
                while (next != s_invalid_idx)
                {
                    auto const idx = next;
                    next           = get_node(next).next;
                    deallocate_node(idx);
                }
                deallocate_node(b.idx);
            }
            b.idx = s_invalid_idx;
        }
        begin_bucket = s_invalid_idx;
    }

    template<typename K>
    constexpr auto insert(K&& value) -> structural::pair<iterator, bool>
    {
        auto const h            = hash_fn(value);
        auto const i            = h % Capacity;
        size_type* new_node_idx = &buckets[i].idx;
        while (*new_node_idx != s_invalid_idx)
        {
            auto& n = nodes[*new_node_idx].active;
            if (equal_fn(n.payload, value))
                return {iterator{this, i, *new_node_idx}, false};
            new_node_idx = &n.next;
        }
        *new_node_idx = allocate_node(std::forward<K>(value));

        if (i < begin_bucket)
            begin_bucket = i;
        return {iterator{this, i, *new_node_idx}, true};
    }

    template<typename... Args>
    constexpr auto emplace(Args&&... args) -> structural::pair<iterator, bool>
    {
        assert(node_count < Capacity);
        auto const  new_idx      = allocate_node(std::forward<Args>(args)...);
        auto const& new_node     = get_node(new_idx).payload;
        auto const  h            = hash_fn(new_node);
        auto const  i            = h % Capacity;
        size_type*  new_node_idx = &buckets[i].idx;
        while (*new_node_idx != s_invalid_idx)
        {
            auto& n = nodes[*new_node_idx].active;
            if (equal_fn(n.payload, new_node))
            {
                deallocate_node(new_idx);
                return {iterator{this, i, *new_node_idx}, false};
            }
            new_node_idx = &n.next;
        }
        *new_node_idx = new_idx;

        if (i < begin_bucket)
            begin_bucket = i;
        return {iterator{this, i, *new_node_idx}, true};
    }

    constexpr auto erase(const_iterator pos) -> iterator
    {
        size_type const bucket_idx = pos.idx;
        size_type const node_idx   = pos.node_idx;
        ++pos;

        if (begin_bucket == bucket_idx)
            begin_bucket = pos.idx;

        size_type n = buckets[bucket_idx].idx;
        if (n == node_idx)
        {
            buckets[bucket_idx].idx = get_node(n).next;
            deallocate_node(n);
            return iterator{this, pos.idx, pos.node_idx};
        }

        for (; n != s_invalid_idx && get_node(n).next != node_idx; n = get_node(n).next)
        {
        }
        assert(get_node(n).next == node_idx);
        get_node(n).next = get_node(get_node(n).next).next;
        deallocate_node(node_idx);
        return iterator{this, pos.idx, pos.node_idx};
    }

    constexpr auto find(auto const& value) -> iterator
    {
        auto iter = std::as_const(*this).find(value);
        return iterator{this, iter.idx, iter.node_idx};
    }
    constexpr auto find(auto const& value) const -> const_iterator
    {
        auto const h        = hash_fn(value);
        auto       i        = h % Capacity;
        size_type  node_idx = buckets[i].idx;
        while (node_idx != s_invalid_idx)
        {
            auto const& n = nodes[node_idx].active;
            if (equal_fn(n.payload, value))
                return const_iterator{this, i, node_idx};
            node_idx = n.next;
        }
        return end();
    }

    [[nodiscard]] constexpr auto begin() const noexcept -> const_iterator
    {
        return const_iterator{this, begin_bucket, buckets[begin_bucket].idx};
    }
    [[nodiscard]] constexpr auto begin() noexcept -> iterator
    {
        return iterator{this, begin_bucket, buckets[begin_bucket].idx};
    }
    [[nodiscard]] constexpr auto end() const noexcept -> const_iterator { return const_iterator{this, s_invalid_idx}; }
    [[nodiscard]] constexpr auto end() noexcept -> iterator { return iterator{this, s_invalid_idx}; }

    [[nodiscard]] constexpr auto        size() const noexcept -> size_type { return node_count; }
    [[nodiscard]] constexpr auto        empty() const noexcept -> size_type { return size() == 0; }
    [[nodiscard]] constexpr static auto capacity() noexcept -> size_type { return Capacity; }
    [[nodiscard]] constexpr auto        load_factor() const -> float { return size() / static_cast<float>(capacity()); }

    friend constexpr auto operator==(static_hash_table const& lhs, static_hash_table const& rhs) -> bool
    {
        if (lhs.size() != rhs.size())
            return false;
        for (auto&& e : lhs)
            if (rhs.find(e) == rhs.end())
                return false;
        return true;
    }

    std::array<node_t, Capacity>   nodes{};
    std::array<bucket_t, Capacity> buckets{};
    hasher                         hash_fn{};
    key_equal                      equal_fn{};
    size_type                      next_available_idx = 0;
    size_type                      node_count         = 0;
    size_type                      begin_bucket       = s_invalid_idx;
};

template<typename T, std::size_t Capacity, typename Hash, typename Equal, bool Const>
struct static_hash_table_iterator
{
    using iterator_concept  = std::forward_iterator_tag;
    using iterator_category = std::forward_iterator_tag;
    using value_type        = std::conditional_t<Const, T const, T>;
    using difference_type   = std::ptrdiff_t;
    using pointer           = value_type*;
    using reference         = value_type&;

    using container_ptr = std::conditional_t<Const,
                                             static_hash_table<T, Capacity, Hash, Equal> const*,
                                             static_hash_table<T, Capacity, Hash, Equal>*>;
    using size_type     = typename static_hash_table<T, Capacity, Hash, Equal>::size_type;

    static constexpr auto s_invalid_idx = static_hash_table<T, Capacity, Hash, Equal>::s_invalid_idx;

    constexpr static_hash_table_iterator() = default;

    constexpr static_hash_table_iterator(container_ptr container, size_type idx)
        : static_hash_table_iterator(container,
                                     idx,
                                     idx != s_invalid_idx ? container->buckets[idx].idx : s_invalid_idx){};

    constexpr static_hash_table_iterator(container_ptr container, size_type idx, size_type node_idx)
        : container(container)
        , idx(idx)
        , node_idx(node_idx){};

    constexpr static_hash_table_iterator(static_hash_table_iterator<T, Capacity, Hash, Equal, false> const& rhs)
        requires(Const)
    : container(rhs.container)
    , idx(rhs.idx)
    , node_idx(rhs.node_idx){};

    constexpr static_hash_table_iterator(static_hash_table_iterator const&) = default;

    constexpr auto operator*() const noexcept -> reference { return container->get_node(node_idx).payload; }
    constexpr auto operator->() const noexcept -> pointer { return &**this; }

    constexpr auto operator++() noexcept -> static_hash_table_iterator&
    {
        assert(idx != s_invalid_idx);
        if (container->get_node(node_idx).next != s_invalid_idx)
        {
            node_idx = container->get_node(node_idx).next;
            return *this;
        }

        do
        {
            ++idx;
        } while (idx < Capacity && container->buckets[idx].idx == s_invalid_idx);
        if (idx < Capacity)
            node_idx = container->buckets[idx].idx;
        else
        {
            idx      = s_invalid_idx;
            node_idx = s_invalid_idx;
        }
        return *this;
    }

    constexpr auto operator++(int) noexcept -> static_hash_table_iterator
    {
        auto const copy = *this;
        ++*this;
        return copy;
    }

    constexpr auto operator==(static_hash_table_iterator const& rhs) const -> bool
    {
        if (container && rhs.container)
            return container == rhs.container && idx == rhs.idx;
        if (!container && rhs.container)
            return false;
        if (container && !rhs.container)
            return false;
        return true;
    }

    container_ptr container = nullptr;
    size_type     idx       = s_invalid_idx;
    size_type     node_idx  = s_invalid_idx;
};

} // namespace structural::detail

#endif // STRUCTURAL_STATIC_HASH_TABLE_HPP
