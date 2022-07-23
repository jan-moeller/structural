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

#ifndef STRUCTURAL_UNINITIALIZED_ARRAY_HPP
#define STRUCTURAL_UNINITIALIZED_ARRAY_HPP

#include "structural/detail/uninitialized_array_details.hpp"

#include <array>
#include <iterator>

#include <cstddef>

namespace structural
{
template<typename T, std::size_t N>
struct uninitialized_array
{
    using value_type             = T;
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t;
    using reference              = value_type&;
    using const_reference        = value_type const&;
    using pointer                = value_type*;
    using const_pointer          = value_type const*;
    using iterator               = detail::uninitialized_array::iterator<T, false>;
    using const_iterator         = detail::uninitialized_array::iterator<T, true>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    constexpr uninitialized_array()  = default;
    constexpr ~uninitialized_array() = default;

    constexpr uninitialized_array(uninitialized_array const&) = delete;
    constexpr uninitialized_array(uninitialized_array&&)      = delete;

    constexpr auto operator=(uninitialized_array const&) = delete;
    constexpr auto operator=(uninitialized_array&&)      = delete;

    constexpr auto at(size_type pos) -> reference { return array.at(pos).value; }
    constexpr auto at(size_type pos) const -> const_reference { return array.at(pos).value; }

    constexpr auto operator[](size_type pos) -> reference { return array[pos].value; }
    constexpr auto operator[](size_type pos) const -> const_reference { return array[pos].value; }

    constexpr auto front() -> reference { return array.front().value; }
    constexpr auto front() const -> const_reference { return array.front().value; }

    constexpr auto back() -> reference { return array.back().value; }
    constexpr auto back() const -> const_reference { return array.back().value; }

    // TODO: This causes UB at the callsite. Need to investigate what we can do.
    // auto data() noexcept -> pointer { return &front(); }
    // auto data() const noexcept -> const_pointer { return data(); }

    constexpr auto begin() noexcept -> iterator { return iterator{array.begin()}; }
    constexpr auto begin() const noexcept -> const_iterator { return const_iterator{array.begin()}; }
    constexpr auto cbegin() const noexcept -> const_iterator { return begin(); }

    constexpr auto end() noexcept -> iterator { return iterator{array.end()}; }
    constexpr auto end() const noexcept -> const_iterator { return const_iterator{array.end()}; }
    constexpr auto cend() const noexcept -> const_iterator { return end(); }

    constexpr auto rbegin() noexcept -> reverse_iterator { return reverse_iterator{end()}; }
    constexpr auto rbegin() const noexcept -> const_reverse_iterator { return const_reverse_iterator{end()}; }
    constexpr auto crbegin() const noexcept -> const_reverse_iterator { return rbegin(); }

    constexpr auto rend() noexcept -> reverse_iterator { return reverse_iterator{begin()}; }
    constexpr auto rend() const noexcept -> const_reverse_iterator { return const_reverse_iterator{begin()}; }
    constexpr auto crend() const noexcept -> const_reverse_iterator { return rend(); }

    [[nodiscard]] constexpr auto empty() const noexcept -> bool { return array.empty(); }
    [[nodiscard]] constexpr auto size() const noexcept -> size_type { return array.size(); }
    [[nodiscard]] constexpr auto max_size() const noexcept -> size_type { return array.max_size(); }

    template<typename... Ts>
    constexpr void construct_at(size_type pos, Ts&&... values)
    {
        construct_at(begin() + pos, std::forward<Ts>(values)...);
    }
    template<typename... Ts>
    constexpr void construct_at(const_iterator pos, Ts&&... values)
    {
        std::destroy_at(std::addressof(pos.data->empty));
        std::construct_at(std::addressof(pos.data->value), std::forward<Ts>(values)...);
    }

    constexpr void destroy_at(size_type pos) { destroy_at(begin() + pos); }
    constexpr void destroy_at(const_iterator pos)
    {
        std::destroy_at(std::addressof(pos.data->value));
        std::construct_at(std::addressof(pos.data->empty));
    }

    std::array<detail::uninitialized_array::element_t<value_type>, N> array{};
};

template<typename T, std::size_t N>
    requires(std::is_trivially_default_constructible_v<T> && std::is_trivially_destructible_v<T>)
struct uninitialized_array<T, N>
{
    using value_type             = T;
    using size_type              = std::array<T, N>::size_type;
    using difference_type        = std::array<T, N>::difference_type;
    using reference              = std::array<T, N>::reference;
    using const_reference        = std::array<T, N>::const_reference;
    using pointer                = std::array<T, N>::pointer;
    using const_pointer          = std::array<T, N>::const_pointer;
    using iterator               = std::array<T, N>::iterator;
    using const_iterator         = std::array<T, N>::const_iterator;
    using reverse_iterator       = std::array<T, N>::reverse_iterator;
    using const_reverse_iterator = std::array<T, N>::const_reverse_iterator;

    constexpr uninitialized_array()  = default;
    constexpr ~uninitialized_array() = default;

    constexpr uninitialized_array(uninitialized_array const&) = delete;
    constexpr uninitialized_array(uninitialized_array&&)      = delete;

    constexpr auto operator=(uninitialized_array const&) = delete;
    constexpr auto operator=(uninitialized_array&&)      = delete;

    constexpr auto at(size_type pos) -> reference { return array.at(pos); }
    constexpr auto at(size_type pos) const -> const_reference { return array.at(pos); }

    constexpr auto operator[](size_type pos) -> reference { return array[pos]; }
    constexpr auto operator[](size_type pos) const -> const_reference { return array[pos]; }

    constexpr auto front() -> reference { return array.front(); }
    constexpr auto front() const -> const_reference { return array.front(); }

    constexpr auto back() -> reference { return array.back(); }
    constexpr auto back() const -> const_reference { return array.back(); }

    constexpr auto data() noexcept -> pointer { return array.data(); }
    constexpr auto data() const noexcept -> const_pointer { return array.data(); }

    constexpr auto begin() noexcept -> iterator { return iterator{array.begin()}; }
    constexpr auto begin() const noexcept -> const_iterator { return const_iterator{array.begin()}; }
    constexpr auto cbegin() const noexcept -> const_iterator { return begin(); }

    constexpr auto end() noexcept -> iterator { return iterator{array.end()}; }
    constexpr auto end() const noexcept -> const_iterator { return const_iterator{array.end()}; }
    constexpr auto cend() const noexcept -> const_iterator { return end(); }

    constexpr auto rbegin() noexcept -> reverse_iterator { return array.rbegin(); }
    constexpr auto rbegin() const noexcept -> const_reverse_iterator { return array.rbegin(); }
    constexpr auto crbegin() const noexcept -> const_reverse_iterator { return rbegin(); }

    constexpr auto rend() noexcept -> reverse_iterator { return array.rend(); }
    constexpr auto rend() const noexcept -> const_reverse_iterator { return array.rend(); }
    constexpr auto crend() const noexcept -> const_reverse_iterator { return rend(); }

    [[nodiscard]] constexpr auto empty() const noexcept -> bool { return array.empty(); }
    [[nodiscard]] constexpr auto size() const noexcept -> size_type { return array.size(); }
    [[nodiscard]] constexpr auto max_size() const noexcept -> size_type { return array.max_size(); }

    template<typename... Ts>
    constexpr void construct_at(size_type pos, Ts&&... values)
    {
        construct_at(begin() + pos, std::forward<Ts>(values)...);
    }
    template<typename... Ts>
    constexpr void construct_at(const_iterator pos, Ts&&... values)
    {
        *(begin() + (pos - begin())) = value_type{std::forward<Ts>(values)...};
    }

    constexpr void destroy_at(size_type pos) { destroy_at(begin() + pos); }
    constexpr void destroy_at(const_iterator pos) {}

    std::array<value_type, N> array{};
};
} // namespace structural

#endif // STRUCTURAL_UNINITIALIZED_ARRAY_HPP
