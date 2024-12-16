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

#ifndef STRUCTURAL_BASIC_INPLACE_MAP_HPP
#define STRUCTURAL_BASIC_INPLACE_MAP_HPP

#include "structural/detail/inplace_red_black_tree.hpp"

#include <functional>

namespace structural
{
template<typename Key, typename T, std::size_t Capacity, typename Compare = std::less<Key>>
struct inplace_map
{
    using key_type        = Key;
    using mapped_type     = T;
    using value_type      = pair<Key const, T>;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using key_compare     = Compare;
    using reference       = value_type&;
    using const_reference = value_type const&;
    using pointer         = value_type*;
    using const_pointer   = value_type const*;

    struct value_compare
    {
        constexpr auto operator()(value_type const& lhs, value_type const& rhs) const -> bool
        {
            return cmp(lhs.first, rhs.first);
        }
        constexpr auto operator()(key_type const& lhs, value_type const& rhs) const -> bool
        {
            return cmp(lhs, rhs.first);
        }
        constexpr auto operator()(value_type const& lhs, key_type const& rhs) const -> bool
        {
            return cmp(lhs.first, rhs);
        }
        Compare cmp;
    };

    using iterator       = typename detail::inplace_red_black_tree<value_type, Capacity, value_compare>::iterator;
    using const_iterator = typename detail::inplace_red_black_tree<value_type, Capacity, value_compare>::const_iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    constexpr inplace_map() noexcept = default;
    constexpr inplace_map(Compare const& cmp) noexcept
        : data(value_compare(cmp))
    {
    }
    template<std::input_iterator First, std::sentinel_for<First> Last>
    constexpr inplace_map(First first, Last last, Compare const& comp = Compare())
        : data(first, last, value_compare(comp))
    {
    }
    constexpr inplace_map(std::initializer_list<value_type> init, Compare const& comp = Compare())
        : data(init.begin(), init.end(), value_compare(comp))
    {
    }
    constexpr auto operator=(std::initializer_list<value_type> init) -> inplace_map&
    {
        data.clear();
        for (auto&& e : init)
            data.insert(e);
        return *this;
    }

    constexpr auto at(Key const& key) -> T&
    {
        auto const iter = find(key);
        if (iter == end())
            throw std::out_of_range{"inplace_map::at"};
        return data.nodes[iter.idx].active.payload.second;
    }
    constexpr auto at(Key const& key) const -> T const&
    {
        auto const iter = find(key);
        if (iter == end())
            throw std::out_of_range{"inplace_map::at"};
        return data.nodes[iter.idx].active.payload.second;
    }
    constexpr auto operator[](Key const& key) -> T&
    {
        auto [iter, _] = insert_or_assign(key, T{});
        return data.nodes[iter.idx].active.payload.second;
    }
    constexpr auto operator[](Key&& key) -> T&
    {
        auto [iter, _] = insert_or_assign(std::move(key), T{});
        return data.nodes[iter.idx].active.payload.second;
    }

    constexpr auto begin() noexcept -> iterator { return data.begin(); }
    constexpr auto begin() const noexcept -> const_iterator { return data.begin(); }
    constexpr auto cbegin() const noexcept -> const_iterator { return data.begin(); }
    constexpr auto end() noexcept -> iterator { return data.end(); }
    constexpr auto end() const noexcept -> const_iterator { return data.end(); }
    constexpr auto cend() const noexcept -> const_iterator { return data.end(); }
    constexpr auto rbegin() noexcept -> iterator { return std::reverse_iterator{end()}; }
    constexpr auto rbegin() const noexcept -> const_iterator { return std::reverse_iterator{end()}; }
    constexpr auto crbegin() const noexcept -> const_iterator { return std::reverse_iterator{cend()}; }
    constexpr auto rend() noexcept -> iterator { return std::reverse_iterator{begin()}; }
    constexpr auto rend() const noexcept -> const_iterator { return std::reverse_iterator{begin()}; }
    constexpr auto crend() const noexcept -> const_iterator { return std::reverse_iterator{cbegin()}; }

    [[nodiscard]] constexpr auto        empty() const noexcept -> bool { return data.empty(); }
    [[nodiscard]] constexpr auto        size() const noexcept -> size_type { return data.size(); }
    [[nodiscard]] constexpr static auto capacity() noexcept -> size_type { return Capacity; }

    constexpr void clear() { data.clear(); }

    constexpr auto insert(value_type const& value) -> iterator { return data.insert(value); }
    constexpr auto insert(value_type&& value) -> iterator { return data.insert(std::move(value)); }
    template<std::input_iterator First, std::sentinel_for<First> Last>
    constexpr void insert(First first, Last last)
    {
        for (auto iter = first; iter != last; ++iter)
            insert(*iter);
    }
    constexpr void insert(std::initializer_list<value_type> init) { insert(init.begin(), init.end()); }

    template<typename M>
    constexpr auto insert_or_assign(Key const& k, M&& obj) -> pair<iterator, bool>
    {
        auto overwrite_fn = [o = std::forward<M>(obj)](value_type& v, Key const&)
        {
            v.second = std::forward<decltype(o)>(o);
        };
        auto allocate_fn = [&k, o = std::forward<M>(obj)](decltype(data)& t, Key const&)
        {
            return t.allocate_node(value_type(k, std::forward<decltype(o)>(o)));
        };
        bool preexisting = false;
        auto iter        = data.insert(k, &preexisting, overwrite_fn, allocate_fn);
        return structural::make_pair(iter, preexisting);
    }
    template<typename M>
    constexpr auto insert_or_assign(Key&& k, M&& obj) -> pair<iterator, bool>
    {
        auto overwrite_fn = [o = std::forward<M>(obj)](value_type& v, Key const&)
        {
            v.second = std::forward<decltype(o)>(o);
        };
        auto allocate_fn = [o = std::forward<M>(obj)](decltype(data)& t, Key&& k)
        {
            return t.allocate_node(value_type(std::move(k), std::forward<decltype(o)>(o)));
        };
        bool preexisting = false;
        auto iter        = data.insert(std::move(k), &preexisting, overwrite_fn, allocate_fn);
        return structural::make_pair(iter, preexisting);
    }

    template<typename... Args>
    constexpr auto emplace(Args&&... args) -> iterator
    {
        return data.emplace(std::forward<Args>(args)...);
    }
    template<typename... Args>
    constexpr auto try_emplace(Key const& key, Args&&... args) -> pair<iterator, bool>
    {
        auto const new_node_idx = data.allocate_node(std::piecewise_construct,
                                                     forward_as_tuple(key),
                                                     forward_as_tuple(std::forward<Args>(args)...));
        auto       overwrite_fn = [&](auto&&...)
        {
            data.deallocate_node(new_node_idx);
        };
        auto allocate_fn = [&](auto&&...)
        {
            return new_node_idx;
        };
        bool preexisting = false;
        auto iter = data.insert(data.nodes[new_node_idx].active.payload, &preexisting, overwrite_fn, allocate_fn);
        return make_pair(iter, preexisting);
    }
    template<typename... Args>
    constexpr auto try_emplace(Key&& key, Args&&... args) -> pair<iterator, bool>
    {
        auto const new_node_idx = data.allocate_node(std::piecewise_construct,
                                                     forward_as_tuple(std::move(key)),
                                                     forward_as_tuple(std::forward<Args>(args)...));
        auto       overwrite_fn = [&](auto&&...)
        {
            data.deallocate_node(new_node_idx);
        };
        auto allocate_fn = [&](auto&&...)
        {
            return new_node_idx;
        };
        bool preexisting = false;
        auto iter = data.insert(data.nodes[new_node_idx].active.payload, &preexisting, overwrite_fn, allocate_fn);
        return make_pair(iter, preexisting);
    }

    constexpr auto erase(const_iterator pos) -> iterator { return data.erase(*pos); }
    constexpr auto erase(iterator pos) -> iterator { return data.erase(*pos); }
    constexpr auto erase(const_iterator first, const_iterator last) -> iterator
    {
        auto iter = first;
        while (iter != last)
        {
            auto next = iter;
            ++next;
            erase(iter);
            iter = next;
        }
        return iterator(&data, last.idx);
    }
    constexpr auto erase(iterator first, iterator last) -> iterator
    {
        return erase(const_iterator(first), const_iterator(last));
    }
    template<typename K>
    constexpr auto erase(K&& x) -> size_type
    {
        if (data.erase(std::forward<K>(x)))
            return 1;
        return 0;
    }

    template<typename K>
    constexpr auto count(K const& x) const -> size_type
    {
        if (data.find(x) != end())
            return 1;
        return 0;
    }
    template<typename K>
    constexpr auto find(K const& x) const -> const_iterator
    {
        return data.find(x);
    }
    template<typename K>
    constexpr auto find(K const& x) -> iterator
    {
        return data.find(x);
    }

    template<typename K>
    [[nodiscard]] constexpr auto contains(K const& value) const noexcept -> bool
    {
        return count(value) != 0;
    }

    template<typename K>
    constexpr auto equal_range(K const& x) const -> pair<const_iterator, const_iterator>
    {
        return data.equal_range(x);
    }
    template<typename K>
    constexpr auto equal_range(K const& x) -> pair<iterator, iterator>
    {
        return data.equal_range(x);
    }
    template<typename K>
    constexpr auto lower_bound(K const& x) const -> const_iterator
    {
        return data.lower_bound(x);
    }
    template<typename K>
    constexpr auto lower_bound(K const& x) -> iterator
    {
        return data.lower_bound(x);
    }
    template<typename K>
    constexpr auto upper_bound(K const& x) const -> const_iterator
    {
        return data.upper_bound(x);
    }
    template<typename K>
    constexpr auto upper_bound(K const& x) -> iterator
    {
        return data.upper_bound(x);
    }

    friend constexpr auto operator==(inplace_map const& lhs, inplace_map const& rhs) -> bool = default;

    // -- internal API
    detail::inplace_red_black_tree<value_type, Capacity, value_compare> data;
};

template<typename Key, typename T, std::size_t Capacity, typename Compare, typename Pred>
constexpr auto erase_if(inplace_map<Key, T, Capacity, Compare>& c, Pred pred) ->
    typename inplace_map<Key, T, Capacity, Compare>::size_type
{
    auto old_size = c.size();
    for (auto i = c.begin(), last = c.end(); i != last;)
    {
        if (pred(*i))
            i = c.erase(i);
        else
            ++i;
    }
    return old_size - c.size();
}
} // namespace structural

#endif // STRUCTURAL_BASIC_INPLACE_MAP_HPP
