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

#ifndef STRUCTURAL_INPLACE_RED_BLACK_TREE_HPP
#define STRUCTURAL_INPLACE_RED_BLACK_TREE_HPP

#include "structural/pair.hpp"

#include <ctrx/contracts.hpp>

#include <algorithm>
#include <array>
#include <functional>
#include <limits>
#include <memory>

#include <cstdint>

namespace structural::detail
{
template<typename T, std::size_t Capacity, typename Compare, bool Const>
struct inplace_red_black_tree_iterator;

template<typename T, std::size_t Capacity, typename Compare = std::less<T>>
struct inplace_red_black_tree
{
    static constexpr std::size_t s_invalid_idx = std::numeric_limits<std::size_t>::max();
    enum class color_t
    {
        red,
        black
    };
    struct active_node_t
    {
        T           payload;
        std::size_t left   = s_invalid_idx;
        std::size_t right  = s_invalid_idx;
        std::size_t parent = s_invalid_idx;
        color_t     color  = color_t::red;

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

    using value_type             = node_t;
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t;
    using reference              = value_type&;
    using const_reference        = value_type const&;
    using pointer                = value_type*;
    using const_pointer          = value_type const*;
    using iterator               = inplace_red_black_tree_iterator<T, Capacity, Compare, false>;
    using const_iterator         = inplace_red_black_tree_iterator<T, Capacity, Compare, true>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    constexpr inplace_red_black_tree() noexcept
    {
        size_type i = 0;
        for (auto& n : nodes)
            n.inactive.next_free_idx = ++i;
    }

    constexpr inplace_red_black_tree(auto first, auto last, auto comp)
        : cmp(comp)
    {
        size_type i = 0;
        for (auto& n : nodes)
            n.inactive.next_free_idx = ++i;
        for (auto iter = first; iter != last; ++iter)
            insert(*iter);
    }

    constexpr ~inplace_red_black_tree() { clear(); }

    constexpr inplace_red_black_tree(inplace_red_black_tree const& other)
        : inplace_red_black_tree()
    {
        other.visit(other.root_idx, [&](size_type idx) { insert(other.nodes[idx].active.payload); });
    }

    constexpr inplace_red_black_tree(inplace_red_black_tree&& other)
        : inplace_red_black_tree()
    {
        other.visit(other.root_idx, [&](size_type idx) { insert(std::move(other.nodes[idx].active.payload)); });
        other.clear();
    }

    constexpr auto operator=(inplace_red_black_tree const& other) -> inplace_red_black_tree&
    {
        if (this != &other)
        {
            clear();
            other.visit(other.root_idx, [&](size_type idx) { insert(other.nodes[idx].active.payload); });
        }
        return *this;
    }

    constexpr auto operator=(inplace_red_black_tree&& other) -> inplace_red_black_tree&
    {
        if (this != &other)
        {
            clear();
            other.visit(other.root_idx, [&](size_type idx) { insert(std::move(other.nodes[idx].active.payload)); });
            other.clear();
        }
        return *this;
    }

    constexpr auto get_node(size_type idx) noexcept -> active_node_t&
    {
        CTRX_PRECONDITION(idx != s_invalid_idx);
        return nodes[idx].active;
    }
    constexpr auto get_node(size_type idx) const noexcept -> active_node_t const&
    {
        CTRX_PRECONDITION(idx != s_invalid_idx);
        return nodes[idx].active;
    }
    constexpr void set_left(size_type idx, size_type node) noexcept
    {
        get_node(idx).left = node;
        if (node != s_invalid_idx)
            get_node(node).parent = idx;
    }
    constexpr void set_right(size_type idx, size_type node) noexcept
    {
        get_node(idx).right = node;
        if (node != s_invalid_idx)
            get_node(node).parent = idx;
    }
    constexpr void flip_color(color_t& c) const noexcept { c = (c == color_t::red) ? color_t::black : color_t::red; }
    constexpr void flip_colors(size_type idx) noexcept
    {
        auto& n = get_node(idx);
        flip_color(n.color);
        flip_color(get_node(n.left).color);
        flip_color(get_node(n.right).color);
    }
    constexpr auto rotate_left(size_type idx) noexcept -> size_type
    {
        auto&      n          = get_node(idx);
        auto const right      = n.right;
        auto&      right_node = get_node(n.right);
        set_right(idx, right_node.left);
        set_left(right, idx);
        right_node.color = n.color;
        n.color          = color_t::red;
        return right;
    }
    constexpr auto rotate_right(size_type idx) noexcept -> size_type
    {
        auto&      n         = get_node(idx);
        auto const left      = n.left;
        auto&      left_node = get_node(left);
        set_left(idx, left_node.right);
        set_right(left, idx);
        left_node.color = n.color;
        n.color         = color_t::red;
        return left;
    }
    constexpr auto is_red(size_type idx) const noexcept -> bool
    {
        return idx != s_invalid_idx && get_node(idx).color == color_t::red;
    }
    constexpr auto move_red_left(size_type idx) noexcept -> size_type
    {
        flip_colors(idx);
        auto& n = get_node(idx);
        if (is_red(get_node(n.right).left))
        {
            set_right(idx, rotate_right(n.right));
            idx = rotate_left(idx);
            flip_colors(idx);
        }
        return idx;
    }
    constexpr auto move_red_right(size_type idx) noexcept -> size_type
    {
        flip_colors(idx);
        auto& n = get_node(idx);
        if (is_red(get_node(n.left).left))
        {
            idx = rotate_right(idx);
            flip_colors(idx);
        }
        return idx;
    }
    constexpr auto fix_up(size_type idx) noexcept -> size_type
    {
        if (is_red(get_node(idx).right) /* && !is_red(get_node(idx).left)*/)
            idx = rotate_left(idx);
        if (is_red(get_node(idx).left) && is_red(get_node(get_node(idx).left).left))
            idx = rotate_right(idx);
        if (is_red(get_node(idx).left) && is_red(get_node(idx).right))
            flip_colors(idx);
        return idx;
    }
    constexpr auto detach_min(size_type idx, size_type* min_idx) noexcept -> size_type
    {
        auto& n = get_node(idx);
        if (n.left == s_invalid_idx)
        {
            *min_idx = idx;
            return s_invalid_idx;
        }
        if (!is_red(n.left) && !is_red(get_node(n.left).left))
            idx = move_red_left(idx);
        set_left(idx, detach_min(get_node(idx).left, min_idx));
        return fix_up(idx);
    }
    template<typename... Ks>
    constexpr auto allocate_node(Ks&&... values) -> size_type
    {
        CTRX_PRECONDITION(next_available_idx < Capacity);
        size_type const idx = next_available_idx;
        next_available_idx  = nodes[idx].inactive.next_free_idx;
        std::construct_at(&nodes[idx].active, std::forward<Ks>(values)...);
        ++node_count;
        return idx;
    }
    constexpr void deallocate_node(size_type idx)
    {
        CTRX_PRECONDITION(idx != s_invalid_idx);
        std::destroy_at(&nodes[idx].active);
        nodes[idx].inactive = inactive_node_t{.next_free_idx = next_available_idx};
        next_available_idx  = idx;
        --node_count;
    }
    struct default_overwrite_fn
    {
        constexpr void operator()(auto...) noexcept {}
    };
    struct default_allocate_fn
    {
        template<typename U>
        constexpr auto operator()(inplace_red_black_tree& srbt, U&& v)
        {
            return srbt.allocate_node(std::forward<U>(v));
        }
    };
    template<typename K, typename OverwriteFn, typename AllocateFn>
    constexpr auto insert(size_type   idx,
                          K&&         value,
                          size_type*  dest_idx,
                          bool*       preexisting,
                          OverwriteFn overwrite_fn,
                          AllocateFn  allocate_fn) -> size_type
    {
        if (idx == s_invalid_idx)
        {
            *dest_idx = allocate_fn(*this, std::forward<K>(value));
            if (preexisting)
                *preexisting = false;
            return *dest_idx;
        }

        auto& n = get_node(idx);

        auto const less  = cmp(value, n.payload);
        auto const equal = !less && !cmp(n.payload, value);
        if (equal)
        {
            overwrite_fn(n.payload, std::forward<K>(value));
            if (preexisting)
                *preexisting = true;
            *dest_idx = idx;
        }
        else if (less)
            set_left(idx,
                     insert<K, OverwriteFn, AllocateFn>(n.left,
                                                        std::forward<K>(value),
                                                        dest_idx,
                                                        preexisting,
                                                        overwrite_fn,
                                                        allocate_fn));
        else
            set_right(idx,
                      insert<K, OverwriteFn, AllocateFn>(n.right,
                                                         std::forward<K>(value),
                                                         dest_idx,
                                                         preexisting,
                                                         overwrite_fn,
                                                         allocate_fn));

        return fix_up(idx);
    }
    template<typename K, typename OverwriteFn = default_overwrite_fn, typename AllocateFn = default_allocate_fn>
    constexpr auto insert(K&&         value,
                          bool*       preexisting  = nullptr,
                          OverwriteFn overwrite_fn = OverwriteFn{},
                          AllocateFn  allocate_fn  = AllocateFn{}) -> iterator
    {
        size_t dest_idx           = s_invalid_idx;
        root_idx                  = insert<K, OverwriteFn, AllocateFn>(root_idx,
                                                      std::forward<K>(value),
                                                      &dest_idx,
                                                      preexisting,
                                                      overwrite_fn,
                                                      allocate_fn);
        get_node(root_idx).parent = s_invalid_idx;
        get_node(root_idx).color  = color_t::black;
        return iterator{this, dest_idx};
    }
    template<typename... Ks>
    constexpr auto emplace(Ks&&... values) -> iterator
    {
        auto const new_node_idx = allocate_node(std::forward<Ks>(values)...);
        size_type  dest_idx     = s_invalid_idx;
        auto       overwrite_fn = [&](auto&&...)
        {
            deallocate_node(new_node_idx);
        };
        auto allocate_fn = [&](auto&&...)
        {
            return new_node_idx;
        };
        root_idx = insert(root_idx, nodes[new_node_idx].active.payload, &dest_idx, nullptr, overwrite_fn, allocate_fn);
        get_node(root_idx).parent = s_invalid_idx;
        get_node(root_idx).color  = color_t::black;
        return iterator{this, dest_idx};
    }
    template<typename K>
    constexpr auto erase(size_type idx, K&& value, size_type* erased_idx) -> size_type
    {
        auto& n = get_node(idx);
        if (cmp(value, n.payload))
        {
            if (!is_red(n.left) && !is_red(get_node(n.left).left))
                idx = move_red_left(idx);
            set_left(idx, erase(get_node(idx).left, std::forward<K>(value), erased_idx));
        }
        else
        {
            if (is_red(n.left))
                idx = rotate_right(idx);
            if (!cmp(get_node(idx).payload, value) && get_node(idx).right == s_invalid_idx)
            {
                CTRX_ASSERT(get_node(idx).left == s_invalid_idx);
                *erased_idx = idx;
                return s_invalid_idx;
            }
            if (get_node(idx).right != s_invalid_idx)
            {
                if (!is_red(get_node(idx).right) && !is_red(get_node(get_node(idx).right).left))
                    idx = move_red_right(idx);
                if (!cmp(get_node(idx).payload, value))
                {
                    size_type min_idx = get_node(idx).right;
                    set_right(idx, detach_min(get_node(idx).right, &min_idx));

                    if (get_node(idx).parent != s_invalid_idx)
                    {
                        auto& parent_node = get_node(get_node(idx).parent);
                        if (parent_node.left == idx)
                            set_left(get_node(idx).parent, min_idx);
                        else
                            set_right(get_node(idx).parent, min_idx);
                    }
                    set_left(min_idx, get_node(idx).left);
                    set_right(min_idx, get_node(idx).right);
                    get_node(min_idx).color = get_node(idx).color;

                    *erased_idx = idx;
                    idx         = min_idx;
                }
                else
                    set_right(idx, erase(get_node(idx).right, std::forward<K>(value), erased_idx));
            }
        }
        return fix_up(idx);
    }
    template<typename K>
    constexpr auto erase(K&& value) -> iterator
    {
        size_type erased_idx = s_invalid_idx;
        root_idx             = erase(root_idx, std::forward<K>(value), &erased_idx);
        if (root_idx != s_invalid_idx)
            get_node(root_idx).color = color_t::black;
        if (erased_idx != s_invalid_idx)
        {
            auto const iter = lower_bound(get_node(erased_idx).payload);
            deallocate_node(erased_idx);
            return iter;
        }
        return end();
    }
    constexpr void visit(size_type idx, auto const& visitor) const
    {
        if (idx == s_invalid_idx)
            return;
        auto const& n = get_node(idx);
        visit(n.left, visitor);
        visit(n.right, visitor);
        visitor(idx);
    }
    [[nodiscard]] constexpr auto size() const noexcept -> size_type { return node_count; }
    [[nodiscard]] constexpr auto empty() const noexcept -> bool { return node_count == 0; }
    template<typename K>
    constexpr auto find(K const& value) -> iterator
    {
        auto iter = std::as_const(*this).find(value);
        return iterator{this, iter.idx};
    }
    template<typename K>
    constexpr auto find(K const& value) const -> const_iterator
    {
        auto idx = root_idx;
        while (idx != s_invalid_idx)
        {
            auto const& node  = get_node(idx);
            auto const  less  = cmp(value, node.payload);
            auto const  equal = !less && !cmp(node.payload, value);
            if (equal)
                return const_iterator{this, idx};
            if (less)
                idx = node.left;
            else
                idx = node.right;
        }
        return end();
    }

    constexpr void clear()
    {
        if (root_idx != s_invalid_idx)
            visit(root_idx, [this](size_type idx) { deallocate_node(idx); });
        root_idx = s_invalid_idx;
    }

    template<typename K>
    constexpr auto lower_bound(K const& x, size_type idx) const -> const_iterator
    {
        if (idx == s_invalid_idx)
            return end();
        auto const& n = get_node(idx);
        if (cmp(n.payload, x))
            return lower_bound(x, n.right);
        if (cmp(x, n.payload))
        {
            auto const l = lower_bound(x, n.left);
            if (l == end())
                return const_iterator{this, idx};
            return l;
        }
        return const_iterator{this, idx};
    }
    template<typename K>
    constexpr auto lower_bound(K const& x) const -> const_iterator
    {
        return lower_bound(x, root_idx);
    }
    template<typename K>
    constexpr auto lower_bound(K const& x) -> iterator
    {
        auto iter = std::as_const(*this).lower_bound(x);
        return iterator{this, iter.idx};
    }
    template<typename K>
    constexpr auto upper_bound(K const& x, size_type idx) const -> const_iterator
    {
        if (idx == s_invalid_idx)
            return end();
        auto const& n = get_node(idx);
        if (cmp(x, n.payload))
        {
            auto const l = upper_bound(x, n.left);
            if (l == end())
                return const_iterator{this, idx};
            return l;
        }
        return upper_bound(x, n.right);
    }
    template<typename K>
    constexpr auto upper_bound(K const& x) const -> const_iterator
    {
        return upper_bound(x, root_idx);
    }
    template<typename K>
    constexpr auto upper_bound(K const& x) -> iterator
    {
        auto iter = std::as_const(*this).upper_bound(x);
        return iterator{this, iter.idx};
    }
    template<typename K>
    constexpr auto equal_range(K const& x) const -> structural::pair<const_iterator, const_iterator>
    {
        return structural::pair<const_iterator, const_iterator>{lower_bound(x), upper_bound(x)};
    }
    template<typename K>
    constexpr auto equal_range(K const& x) -> structural::pair<iterator, iterator>
    {
        return structural::pair<iterator, iterator>{lower_bound(x), upper_bound(x)};
    }

    [[nodiscard]] constexpr auto begin() const noexcept -> const_iterator
    {
        auto idx = root_idx;
        if (idx == s_invalid_idx)
            return end();
        while (get_node(idx).left != s_invalid_idx)
            idx = get_node(idx).left;
        return const_iterator{this, idx};
    }
    [[nodiscard]] constexpr auto begin() noexcept -> iterator
    {
        auto iter = std::as_const(*this).begin();
        return iterator{this, iter.idx};
    }
    [[nodiscard]] constexpr auto end() const noexcept -> const_iterator { return const_iterator{this, s_invalid_idx}; }
    [[nodiscard]] constexpr auto end() noexcept -> iterator { return iterator{this, s_invalid_idx}; }

    friend constexpr auto operator==(inplace_red_black_tree const& lhs, inplace_red_black_tree const& rhs) -> bool
    {
        return lhs.size() == rhs.size() && std::ranges::equal(lhs, rhs);
    }

    std::array<node_t, Capacity> nodes;
    Compare                      cmp;
    size_type                    root_idx           = s_invalid_idx;
    size_type                    next_available_idx = 0;
    size_type                    node_count         = 0;
};

template<typename T, std::size_t Capacity, typename Compare, bool Const>
struct inplace_red_black_tree_iterator
{
    using iterator_concept  = std::bidirectional_iterator_tag;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type        = std::conditional_t<Const, T const, T>;
    using difference_type   = std::ptrdiff_t;
    using pointer           = value_type*;
    using reference         = value_type&;

    constexpr inplace_red_black_tree_iterator() = default;

    constexpr inplace_red_black_tree_iterator(std::conditional_t<Const,
                                                                inplace_red_black_tree<T, Capacity, Compare> const*,
                                                                inplace_red_black_tree<T, Capacity, Compare>*> container,
                                             typename inplace_red_black_tree<T, Capacity, Compare>::size_type  idx)
        : container(container)
        , idx(idx){};

    constexpr inplace_red_black_tree_iterator(inplace_red_black_tree_iterator<T, Capacity, Compare, false> const& rhs)
        requires(Const)
        : container(rhs.container)
        , idx(rhs.idx){};

    constexpr inplace_red_black_tree_iterator(inplace_red_black_tree_iterator const&) = default;

    constexpr auto operator=(inplace_red_black_tree_iterator const&) -> inplace_red_black_tree_iterator& = default;

    constexpr auto operator*() const noexcept -> reference { return container->get_node(idx).payload; }
    constexpr auto operator->() const noexcept -> pointer { return &**this; }

    constexpr auto operator++() noexcept -> inplace_red_black_tree_iterator&
    {
        auto const* n = &container->get_node(idx);
        if (n->right != s_invalid_idx)
        {
            idx = n->right;
            n   = &container->get_node(idx);
            while (n->left != s_invalid_idx)
            {
                idx = n->left;
                n   = &container->get_node(idx);
            }
            return *this;
        }

        while (n->parent != s_invalid_idx)
        {
            auto const parent_idx        = n->parent;
            n                            = &container->get_node(parent_idx);
            bool const arrived_from_left = n->left == idx;
            idx                          = parent_idx;
            if (arrived_from_left)
                return *this;
        }

        idx = s_invalid_idx;
        return *this;
    }

    constexpr auto operator++(int) noexcept -> inplace_red_black_tree_iterator
    {
        auto const copy = *this;
        ++*this;
        return copy;
    }

    constexpr auto operator--() noexcept -> inplace_red_black_tree_iterator&
    {
        auto const* n = &container->get_node(idx);
        if (n->left != s_invalid_idx)
        {
            idx = n->left;
            n   = &container->get_node(idx);
            while (n->right != s_invalid_idx)
            {
                idx = n->right;
                n   = &container->get_node(idx);
            }
            return *this;
        }

        while (n->parent != s_invalid_idx)
        {
            auto const parent_idx         = n->parent;
            n                             = &container->get_node(parent_idx);
            bool const arrived_from_right = n->right == idx;
            idx                           = parent_idx;
            if (arrived_from_right)
                return *this;
        }

        idx = s_invalid_idx;
        return *this;
    }

    constexpr auto operator--(int) noexcept -> inplace_red_black_tree_iterator
    {
        auto const copy = *this;
        --*this;
        return copy;
    }

    constexpr auto operator==(inplace_red_black_tree_iterator const& rhs) const -> bool
    {
        if (container && rhs.container)
            return container == rhs.container && idx == rhs.idx;
        if (!container && rhs.container)
            return false;
        if (container && !rhs.container)
            return false;
        return true;
    }

    static constexpr auto s_invalid_idx = inplace_red_black_tree<T, Capacity, Compare>::s_invalid_idx;
    std::conditional_t<Const,
                       inplace_red_black_tree<T, Capacity, Compare> const*,
                       inplace_red_black_tree<T, Capacity, Compare>*>
                                                                    container = nullptr;
    typename inplace_red_black_tree<T, Capacity, Compare>::size_type idx       = s_invalid_idx;
};
} // namespace structural::detail

#endif // STRUCTURAL_INPLACE_RED_BLACK_TREE_HPP
