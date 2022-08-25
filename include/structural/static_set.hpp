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

#ifndef STRUCTURAL_STATIC_SET_HPP
#define STRUCTURAL_STATIC_SET_HPP

#include "structural/detail/static_red_black_tree.hpp"

#include <functional>

namespace structural
{
template<typename Key, std::size_t Capacity, typename Compare = std::less<Key>>
struct static_set
{
    using key_type               = Key;
    using value_type             = Key;
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t;
    using key_compare            = Compare;
    using value_compare          = Compare;
    using reference              = value_type&;
    using const_reference        = value_type const&;
    using pointer                = value_type*;
    using const_pointer          = value_type const*;
    using iterator               = typename detail::static_red_black_tree<Key, Capacity, Compare>::iterator;
    using const_iterator         = typename detail::static_red_black_tree<Key, Capacity, Compare>::const_iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    constexpr static_set() noexcept = default;
    constexpr static_set(Compare const& cmp) noexcept
        : data(cmp)
    {
    }
    template<std::input_iterator First, std::sentinel_for<First> Last>
    constexpr static_set(First first, Last last, Compare const& comp = Compare())
        : data(first, last, comp)
    {
    }
    constexpr static_set(std::initializer_list<value_type> init, Compare const& comp = Compare())
        : data(init.begin(), init.end(), comp)
    {
    }
    constexpr auto operator=(std::initializer_list<value_type> init) -> static_set&
    {
        data.clear();
        for (auto&& e : init)
            data.insert(e);
        return *this;
    }

    constexpr auto begin() const noexcept -> const_iterator { return data.begin(); }
    constexpr auto cbegin() const noexcept -> const_iterator { return data.begin(); }
    constexpr auto end() const noexcept -> const_iterator { return data.end(); }
    constexpr auto cend() const noexcept -> const_iterator { return data.end(); }
    constexpr auto rbegin() const noexcept -> const_iterator { return std::reverse_iterator{end()}; }
    constexpr auto crbegin() const noexcept -> const_iterator { return std::reverse_iterator{cend()}; }
    constexpr auto rend() const noexcept -> const_iterator { return std::reverse_iterator{begin()}; }
    constexpr auto crend() const noexcept -> const_iterator { return std::reverse_iterator{cbegin()}; }

    [[nodiscard]] constexpr auto        empty() const noexcept -> bool { return data.empty(); }
    [[nodiscard]] constexpr auto        size() const noexcept -> size_type { return data.size(); }
    [[nodiscard]] constexpr static auto capacity() noexcept -> size_type { return Capacity; }

    constexpr void clear() { data.clear(); }

    constexpr auto insert(value_type const& value) -> const_iterator { return data.insert(value); }
    constexpr auto insert(value_type&& value) -> const_iterator { return data.insert(std::move(value)); }
    template<std::input_iterator First, std::sentinel_for<First> Last>
    constexpr void insert(First first, Last last)
    {
        for (auto iter = first; iter != last; ++iter)
            insert(*iter);
    }
    constexpr void insert(std::initializer_list<value_type> init) { insert(init.begin(), init.end()); }

    template<typename... Args>
    constexpr auto emplace(Args&&... args) -> const_iterator
    {
        return data.emplace(std::forward<Args>(args)...);
    }

    constexpr auto erase(const_iterator pos) -> const_iterator { return data.erase(*pos); }
    constexpr auto erase(const_iterator first, const_iterator last) -> const_iterator
    {
        auto iter = first;
        while (iter != last)
        {
            auto next = iter;
            ++next;
            erase(iter);
            iter = next;
        }
        return last;
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
    [[nodiscard]] constexpr auto contains(K const& value) noexcept -> bool
    {
        return count(value) != 0;
    }

    template<typename K>
    constexpr auto equal_range(K const& x) const -> pair<const_iterator, const_iterator>
    {
        return data.equal_range(x);
    }
    template<typename K>
    constexpr auto lower_bound(K const& x) const -> const_iterator
    {
        return data.lower_bound(x);
    }
    template<typename K>
    constexpr auto upper_bound(K const& x) const -> const_iterator
    {
        return data.upper_bound(x);
    }

    friend constexpr auto operator==(static_set const& lhs, static_set const& rhs) -> bool = default;

    // -- internal API

    detail::static_red_black_tree<Key, Capacity, Compare> data;
};

template<typename Key, std::size_t Capacity, typename Compare, typename Pred>
constexpr auto erase_if(static_set<Key, Capacity, Compare>& c, Pred pred) ->
    typename static_set<Key, Capacity, Compare>::size_type
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

#endif // STRUCTURAL_STATIC_SET_HPP
