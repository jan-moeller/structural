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

#ifndef STRUCTURAL_UNINITIALIZED_ARRAY_DETAILS_HPP
#define STRUCTURAL_UNINITIALIZED_ARRAY_DETAILS_HPP

#include <iterator>
#include <type_traits>

#include <cstddef>

namespace structural
{
template<typename T, std::size_t N>
struct uninitialized_array;

namespace detail::uninitialized_array
{
template<typename T>
union element_t
{
    struct empty_t
    {
    };
    empty_t empty;
    T       value;

    constexpr element_t()
        : empty(){};
    constexpr ~element_t(){};
};

template<typename T, bool Const>
struct iterator
{
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using pointer           = std::conditional_t<Const, value_type const*, value_type*>;
    using reference         = std::conditional_t<Const, value_type const&, value_type&>;
    using iterator_category = std::contiguous_iterator_tag;

    constexpr iterator()                = default;
    constexpr iterator(iterator const&) = default;

    template<bool IsConst = Const>
        requires(IsConst)
    constexpr iterator(iterator<T, false> const& rhs)
        : data(rhs.data)
    {
    }

    constexpr auto operator=(iterator const&) -> iterator& = default;

    constexpr auto operator==(iterator const&) const -> bool = default;
    constexpr auto operator<=>(iterator const&) const        = default;

    constexpr auto operator*() const -> std::add_const_t<reference> { return data->value; }
    constexpr auto operator*() -> reference { return data->value; }

    constexpr auto operator->() const -> std::add_const_t<pointer> { return &data->value; }
    constexpr auto operator->() -> pointer { return &data->value; }

    constexpr auto operator++() -> iterator&
    {
        ++data;
        return *this;
    }
    constexpr auto operator++(int) -> iterator
    {
        auto copy = *this;
        ++(*this);
        return copy;
    }

    constexpr auto operator+=(difference_type n) -> iterator&
    {
        data += n;
        return *this;
    }

    friend constexpr auto operator+(iterator iter, difference_type n) -> iterator { return iter += n; }
    friend constexpr auto operator+(difference_type n, iterator iter) -> iterator { return iter += n; }

    constexpr auto operator--() -> iterator&
    {
        --data;
        return *this;
    }
    constexpr auto operator--(int) -> iterator
    {
        auto copy = *this;
        --(*this);
        return copy;
    }

    constexpr auto operator-=(difference_type n) -> iterator&
    {
        data -= n;
        return *this;
    }

    constexpr auto        operator-(iterator const& rhs) const -> difference_type { return data - rhs.data; }
    friend constexpr auto operator-(iterator iter, difference_type n) -> iterator { return iter -= n; }

    constexpr auto operator[](difference_type n) const -> reference { return *(*this + n); }

    // --- internal API

    constexpr explicit iterator(std::conditional_t<Const, element_t<value_type> const*, element_t<value_type>*> ptr)
        : data(ptr)
    {
    }

    std::conditional_t<Const, element_t<value_type> const*, element_t<value_type>*> data;
};

} // namespace detail::uninitialized_array
} // namespace structural

#endif // STRUCTURAL_UNINITIALIZED_ARRAY_DETAILS_HPP
