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

#ifndef STRUCTURAL_INPLACE_VECTOR_HPP
#define STRUCTURAL_INPLACE_VECTOR_HPP

#include "structural/uninitialized_array.hpp"

#include <ctrx/contracts.hpp>

#include <algorithm>
#include <compare>

#include <cstddef>

namespace structural
{
template<typename T, std::size_t Capacity>
struct inplace_vector
{
    using value_type             = uninitialized_array<T, Capacity>::value_type;
    using size_type              = uninitialized_array<T, Capacity>::size_type;
    using difference_type        = uninitialized_array<T, Capacity>::difference_type;
    using reference              = uninitialized_array<T, Capacity>::reference;
    using const_reference        = uninitialized_array<T, Capacity>::const_reference;
    using pointer                = uninitialized_array<T, Capacity>::pointer;
    using const_pointer          = uninitialized_array<T, Capacity>::const_pointer;
    using iterator               = uninitialized_array<T, Capacity>::iterator;
    using const_iterator         = uninitialized_array<T, Capacity>::const_iterator;
    using reverse_iterator       = uninitialized_array<T, Capacity>::reverse_iterator;
    using const_reverse_iterator = uninitialized_array<T, Capacity>::const_reverse_iterator;

    constexpr ~inplace_vector() { clear(); }

    constexpr inplace_vector() = default;
    template<std::input_iterator Iter, std::sentinel_for<Iter> Sentinel>
    constexpr inplace_vector(Iter begin, Sentinel end)
    {
        std::ranges::copy(begin, end, std::back_inserter(*this));
    }
    constexpr inplace_vector(std::initializer_list<T> init)
        : inplace_vector(init.begin(), init.end())
    {
    }

    constexpr inplace_vector(inplace_vector const& other)
        : inplace_vector(other.begin(), other.end())
    {
    }
    constexpr inplace_vector(inplace_vector&& other)
        : inplace_vector(std::move(other).begin(), std::move(other).end())
    {
    }

    constexpr auto operator=(inplace_vector const& other) -> inplace_vector&
    {
        clear();
        std::ranges::copy(other.begin(), other.end(), std::back_inserter(*this));
        return *this;
    }

    constexpr auto operator=(inplace_vector&& other) -> inplace_vector&
    {
        clear();
        for (auto&& e : std::move(other))
            push_back(std::move(e));
        return *this;
    }

    constexpr auto operator=(std::initializer_list<value_type> ilist) -> inplace_vector&
    {
        *this = inplace_vector{ilist};
        return *this;
    }

    constexpr void assign(std::initializer_list<T> ilist) { *this = inplace_vector{ilist}; }
    template<std::input_iterator Iter, std::sentinel_for<Iter> Sentinel>
    constexpr void assign(Iter first, Sentinel last)
    {
        *this = inplace_vector{first, last};
    }

    constexpr auto at(size_type pos) -> reference
    {
        if (pos >= size())
            throw std::invalid_argument{"inplace_vector::at"};
        return array[pos];
    }
    constexpr auto at(size_type pos) const -> const_reference
    {
        if (pos >= size())
            throw std::invalid_argument{"inplace_vector::at"};
        return array[pos];
    }

    constexpr auto operator[](size_type pos) -> reference { return array[pos]; }
    constexpr auto operator[](size_type pos) const -> const_reference { return array[pos]; }

    constexpr auto front() -> reference { return array.front(); }
    constexpr auto front() const -> const_reference { return array.front(); }

    constexpr auto back() -> reference { return *(begin() + count - 1); }
    constexpr auto back() const -> const_reference { return *(begin() + count - 1); }

    constexpr auto begin() noexcept -> iterator { return array.begin(); }
    constexpr auto begin() const noexcept -> const_iterator { return array.begin(); }
    constexpr auto cbegin() const noexcept -> const_iterator { return begin(); }

    constexpr auto end() noexcept -> iterator { return begin() + count; }
    constexpr auto end() const noexcept -> const_iterator { return begin() + count; }
    constexpr auto cend() const noexcept -> const_iterator { return end(); }

    constexpr auto rbegin() noexcept -> reverse_iterator { return std::make_reverse_iterator(end()); }
    constexpr auto rbegin() const noexcept -> const_reverse_iterator { return std::make_reverse_iterator(end()); }
    constexpr auto crbegin() const noexcept -> const_reverse_iterator { return rbegin(); }

    constexpr auto rend() noexcept -> reverse_iterator { return std::make_reverse_iterator(begin()); }
    constexpr auto rend() const noexcept -> const_reverse_iterator { return std::make_reverse_iterator(begin()); }
    constexpr auto crend() const noexcept -> const_reverse_iterator { return rend(); }

    [[nodiscard]] constexpr auto        empty() const noexcept -> bool { return count == 0; }
    [[nodiscard]] constexpr auto        size() const noexcept -> size_type { return count; }
    [[nodiscard]] constexpr static auto capacity() noexcept -> size_type { return Capacity; }

    constexpr void clear() noexcept
    {
        for (size_type i = 0; i < count; ++i)
            array.destroy_at(i);
        count = 0;
    }

    constexpr auto insert(const_iterator pos, const_reference value) -> iterator { return insert(pos, {value}); }
    constexpr auto insert(const_iterator pos, value_type&& value) -> iterator
    {
        CTRX_PRECONDITION(size() < capacity());
        if (pos == end())
        {
            push_back(std::move(value));
            return end() - 1;
        }

        push_back(std::move(back()));
        for (auto iter = (end() - 1); iter != pos; --iter)
            *iter = std::move(*(iter - 1));
        iterator insertion_pos = begin() + (pos - begin());
        *insertion_pos         = std::move(value);
        return insertion_pos;
    }
    template<std::input_iterator Iter, std::sentinel_for<Iter> Sentinel>
    constexpr auto insert(const_iterator pos, Iter first, Sentinel last) -> iterator
    {
        auto middle = end();
        for (std::input_iterator auto iter = first; iter != last; ++iter)
            push_back(*iter);
        iterator mutable_pos = begin() + (pos - begin());
        std::ranges::rotate(mutable_pos, middle, end());
        return mutable_pos;
    }
    constexpr auto insert(const_iterator pos, std::initializer_list<T> ilist) -> iterator
    {
        return insert(pos, ilist.begin(), ilist.end());
    }

    template<typename... Args>
    constexpr auto emplace(const_iterator pos, Args&&... args) -> iterator
    {
        auto middle = end();
        emplace_back(std::forward<Args>(args)...);
        iterator mutable_pos = begin() + (pos - begin());
        std::ranges::rotate(mutable_pos, middle, end());
        return mutable_pos;
    }

    constexpr auto erase(const_iterator pos) -> iterator { return erase(pos, pos + 1); }
    constexpr auto erase(const_iterator first, const_iterator last) -> iterator
    {
        auto const n              = last - first;
        auto       insertion_iter = begin() + (first - begin());
        for (auto iter = last; iter != end(); ++iter)
            *insertion_iter++ = std::move(*iter);
        for (auto iter = end() - n; iter != end(); ++iter)
            array.destroy_at(iter);
        count -= n;
        return begin() + (first - begin());
    }

    constexpr void push_back(const_reference value) { emplace_back(value); }
    constexpr void push_back(value_type&& value) { emplace_back(std::move(value)); }

    template<typename... Args>
    constexpr auto emplace_back(Args&&... args) -> reference
    {
        CTRX_PRECONDITION(size() < capacity());
        array.construct_at(end(), std::forward<Args>(args)...);
        ++count;
        return back();
    }

    constexpr void pop_back() { erase(begin() + (size() - 1)); }

    constexpr auto operator==(inplace_vector const& other) const -> bool
    {
        return std::ranges::equal(begin(), end(), other.begin(), other.end());
    }
    constexpr auto operator<=>(inplace_vector const& rhs) const -> decltype(auto)
        requires(std::three_way_comparable_with<value_type const, value_type const> || (requires(value_type const e) {
                     e < e;
                 } && std::convertible_to<decltype(std::declval<value_type>() < std::declval<value_type>()), bool>))
    {
        return std::lexicographical_compare_three_way(
            begin(),
            end(),
            rhs.begin(),
            rhs.end(),
            [](value_type const& lhs, value_type const& rhs)
            {
                if constexpr (std::three_way_comparable_with<value_type const, value_type const>)
                {
                    return lhs <=> rhs;
                }
                else
                {
                    return lhs < rhs   ? std::weak_ordering::less
                           : rhs < lhs ? std::weak_ordering::greater
                                       : std::weak_ordering::equivalent;
                }
            });
    }

    // -- internal API

    uninitialized_array<T, Capacity> array{};
    size_type                        count = 0;
};

template<typename T, std::size_t Capacity, typename U>
constexpr auto erase(inplace_vector<T, Capacity>& c, U const& value) -> inplace_vector<T, Capacity>::size_type
{
    auto it = std::remove(c.begin(), c.end(), value);
    auto r  = std::distance(it, c.end());
    c.erase(it, c.end());
    return r;
}

template<typename T, std::size_t Capacity, typename Pred>
constexpr auto erase_if(inplace_vector<T, Capacity>& c, Pred pred) -> inplace_vector<T, Capacity>::size_type
{
    auto it = std::remove_if(c.begin(), c.end(), pred);
    auto r  = std::distance(it, c.end());
    c.erase(it, c.end());
    return r;
}

} // namespace structural

#endif // STRUCTURAL_INPLACE_VECTOR_HPP
