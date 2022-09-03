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

#ifndef STRUCTURAL_BASIC_STATIC_STRING_HPP
#define STRUCTURAL_BASIC_STATIC_STRING_HPP

#include "structural/detail/string_view_like.hpp"
#include "structural/static_vector.hpp"

#include <string_view>

#include <cstddef>

namespace structural
{
template<typename CharT, std::size_t Capacity, typename Traits = std::char_traits<CharT>>
struct basic_static_string
{
    using traits_type            = Traits;
    using value_type             = static_vector<CharT, Capacity>::value_type;
    using size_type              = static_vector<CharT, Capacity>::size_type;
    using difference_type        = static_vector<CharT, Capacity>::difference_type;
    using reference              = static_vector<CharT, Capacity>::reference;
    using const_reference        = static_vector<CharT, Capacity>::const_reference;
    using pointer                = static_vector<CharT, Capacity>::pointer;
    using const_pointer          = static_vector<CharT, Capacity>::const_pointer;
    using iterator               = static_vector<CharT, Capacity>::iterator;
    using const_iterator         = static_vector<CharT, Capacity>::const_iterator;
    using reverse_iterator       = static_vector<CharT, Capacity>::reverse_iterator;
    using const_reverse_iterator = static_vector<CharT, Capacity>::const_reverse_iterator;

    constexpr basic_static_string() = default;
    template<std::size_t C>
        requires(C < Capacity)
    constexpr basic_static_string(basic_static_string<CharT, C, Traits> const& s)
        : basic_static_string(s.begin(), s.end())
    {
    }
    constexpr basic_static_string(CharT ch)
        : basic_static_string({ch})
    {
    }
    constexpr basic_static_string(CharT const* s, size_type count)
        : storage(s, s + count)
    {
        storage.push_back(CharT('\0'));
    }
    constexpr basic_static_string(CharT const* s)
        : basic_static_string(std::basic_string_view<CharT, Traits>{s})
    {
    }
    template<std::input_iterator Iter, std::sentinel_for<Iter> Sentinel>
    constexpr basic_static_string(Iter begin, Sentinel end)
        : storage(begin, end)
    {
        storage.push_back(CharT('\0'));
    }
    constexpr basic_static_string(std::initializer_list<CharT> ilist)
        : basic_static_string(ilist.begin(), ilist.end())
    {
    }
    constexpr basic_static_string(detail::string_view_like<CharT, Traits> auto const& t)
        : storage(
            [&]
            {
                std::basic_string_view<CharT, Traits> sv = t;
                return static_vector<CharT, Capacity + 1>(sv.begin(), sv.end());
            }())
    {
        storage.push_back(CharT('\0'));
    }
    constexpr basic_static_string(std::nullptr_t) = delete;

    template<std::size_t C>
        requires(C < Capacity)
    constexpr auto operator=(basic_static_string<CharT, C, Traits> const& s) -> basic_static_string&
    {
        *this = basic_static_string(s);
        return *this;
    }
    constexpr auto operator=(CharT const* s) -> basic_static_string&
    {
        *this = basic_static_string(s);
        return *this;
    }
    constexpr auto operator=(CharT ch) -> basic_static_string&
    {
        *this = basic_static_string(ch);
        return *this;
    }
    constexpr auto operator=(std::initializer_list<CharT> ilist) -> basic_static_string&
    {
        *this = basic_static_string(ilist);
        return *this;
    }
    constexpr auto operator=(detail::string_view_like<CharT, Traits> auto const& t) -> basic_static_string&
    {
        *this = basic_static_string(t);
        return *this;
    }
    constexpr auto operator=(std::nullptr_t) -> basic_static_string& = delete;

    template<std::size_t C>
        requires(C <= Capacity)
    constexpr auto assign(basic_static_string<CharT, C, Traits> const& str) -> basic_static_string&
    {
        storage.assign(str.storage.begin(), str.storage.end());
        return *this;
    }
    template<std::size_t C>
        requires(C <= Capacity)
    constexpr auto assign(basic_static_string<CharT, C, Traits>&& str) noexcept -> basic_static_string&
    {
        if constexpr (C == Capacity)
            storage = std::move(str.storage);
        else
            storage.assign(str.storage.begin(), str.storage.end());
        return *this;
    }
    constexpr auto assign(CharT const* s, size_type count) -> basic_static_string&
    {
        return assign(std::basic_string_view<CharT, Traits>(s, count));
    }
    constexpr auto assign(CharT const* s) -> basic_static_string&
    {
        return assign(std::basic_string_view<CharT, Traits>(s));
    }
    template<std::input_iterator Iter, std::sentinel_for<Iter> Sentinel>
    constexpr auto assign(Iter first, Sentinel last) -> basic_static_string&
    {
        storage.assign(first, last);
        storage.push_back(CharT('\0'));
        return *this;
    }
    constexpr auto assign(std::initializer_list<CharT> ilist) -> basic_static_string&
    {
        return assign(ilist.begin(), ilist.end());
    }
    constexpr auto assign(detail::string_view_like<CharT, Traits> auto const& t) -> basic_static_string&
    {
        std::basic_string_view<CharT, Traits> const sv = t;
        return assign(sv.begin(), sv.end());
    }

    constexpr auto at(size_type pos) -> reference
    {
        if (pos >= size())
            throw std::invalid_argument{"basic_static_string::at"};
        return storage.at(pos);
    }
    constexpr auto at(size_type pos) const -> const_reference
    {
        if (pos >= size())
            throw std::invalid_argument{"basic_static_string::at"};
        return storage.at(pos);
    }

    constexpr auto operator[](size_type pos) -> reference { return storage[pos]; }
    constexpr auto operator[](size_type pos) const -> const_reference { return storage[pos]; }

    constexpr auto front() -> reference { return storage.front(); }
    constexpr auto front() const -> const_reference { return storage.front(); }

    constexpr auto back() -> reference { return *(end() - 1); };
    constexpr auto back() const -> const_reference { return *(end() - 1); };

    constexpr auto data() const noexcept -> CharT const* { return storage.array.data(); }
    constexpr auto data() noexcept -> pointer { return storage.array.data(); }

    constexpr auto c_str() const noexcept -> CharT const* { return data(); };

    constexpr operator std::basic_string_view<CharT, Traits>() const noexcept
    {
        return std::basic_string_view<CharT, Traits>(data(), size());
    }

    constexpr auto begin() noexcept -> iterator { return storage.begin(); }
    constexpr auto begin() const noexcept -> const_iterator { return storage.begin(); }
    constexpr auto cbegin() const noexcept -> const_iterator { return storage.cbegin(); }

    constexpr auto rbegin() noexcept -> reverse_iterator { return storage.rbegin() + 1; }
    constexpr auto rbegin() const noexcept -> const_reverse_iterator { return storage.rbegin() + 1; }
    constexpr auto crbegin() const noexcept -> const_reverse_iterator { return storage.crbegin() + 1; }

    constexpr auto end() noexcept -> iterator { return storage.end() - 1; }
    constexpr auto end() const noexcept -> const_iterator { return storage.end() - 1; }
    constexpr auto cend() const noexcept -> const_iterator { return storage.cend() - 1; }

    constexpr auto rend() noexcept -> reverse_iterator { return storage.rend(); }
    constexpr auto rend() const noexcept -> const_reverse_iterator { return storage.rend(); }
    constexpr auto crend() const noexcept -> const_reverse_iterator { return storage.crend(); }

    [[nodiscard]] constexpr auto empty() const noexcept -> bool { return size() == 0; }

    constexpr auto size() const noexcept -> size_type { return storage.size() - 1; }
    constexpr auto length() const noexcept -> size_type { return size(); }

    constexpr auto capacity() const noexcept -> size_type { return Capacity; }

    constexpr void clear() noexcept
    {
        storage.clear();
        storage.push_back(CharT('\0'));
    }

    constexpr auto insert(const_iterator pos, CharT ch) -> iterator { return storage.insert(pos, ch); }
    constexpr auto insert(const_iterator pos, CharT const* s) -> iterator
    {
        return insert(pos, std::basic_string_view<CharT, Traits>(s));
    }
    constexpr auto insert(const_iterator pos, CharT const* s, size_type count) -> iterator
    {
        return insert(pos, std::basic_string_view<CharT, Traits>(s, count));
    }
    constexpr auto insert(const_iterator pos, basic_static_string const& str) -> iterator
    {
        return insert(pos, str.begin(), str.end());
    }
    constexpr auto insert(const_iterator pos, detail::string_view_like<CharT, Traits> auto const& t) -> iterator
    {
        std::basic_string_view<CharT, Traits> sv = t;
        return insert(pos, t.begin(), t.end());
    }
    template<std::input_iterator Iter, std::sentinel_for<Iter> Sentinel>
    constexpr auto insert(const_iterator pos, Iter first, Sentinel last) -> iterator
    {
        return storage.insert(pos, first, last);
    }
    constexpr auto insert(const_iterator pos, std::initializer_list<CharT> ilist) -> iterator
    {
        return insert(pos, ilist.begin(), ilist.end());
    }

    constexpr auto erase(const_iterator position) -> iterator { return storage.erase(position); }
    constexpr auto erase(const_iterator first, const_iterator last) -> iterator { return storage.erase(first, last); }

    constexpr void push_back(CharT ch)
    {
        storage.back() = ch;
        storage.push_back(CharT('\0'));
    }
    constexpr void pop_back()
    {
        storage.pop_back();
        storage.back() = CharT('\0');
    }

    constexpr auto append(basic_static_string const& str) -> basic_static_string&
    {
        storage.insert(end(), str.begin(), str.end());
        return *this;
    }
    constexpr auto append(CharT ch) -> basic_static_string&
    {
        storage.insert(end(), ch);
        return *this;
    }
    constexpr auto append(CharT const* s, size_type count) -> basic_static_string&
    {
        std::basic_string_view<CharT, Traits> sv(s, count);
        return append(sv);
    }
    constexpr auto append(CharT const* s) -> basic_static_string&
    {
        std::basic_string_view<CharT, Traits> sv(s);
        return append(sv);
    }
    template<std::input_iterator Iter, std::sentinel_for<Iter> Sentinel>
    constexpr auto append(Iter first, Sentinel last) -> basic_static_string&
    {
        storage.insert(end(), first, last);
        return *this;
    }
    constexpr auto append(std::initializer_list<CharT> ilist) -> basic_static_string&
    {
        storage.insert(end(), ilist);
        return *this;
    }
    constexpr auto append(detail::string_view_like<CharT, Traits> auto const& t) -> basic_static_string&
    {
        std::basic_string_view<CharT, Traits> sv = t;
        return append(sv.begin(), sv.end());
    }

    constexpr auto operator+=(basic_static_string const& str) -> basic_static_string& { return append(str); }
    constexpr auto operator+=(CharT ch) -> basic_static_string& { return append(ch); };
    constexpr auto operator+=(CharT const* s) -> basic_static_string& { return append(s); }
    constexpr auto operator+=(std::initializer_list<CharT> ilist) -> basic_static_string& { return append(ilist); }
    constexpr auto operator+=(detail::string_view_like<CharT, Traits> auto const& t) -> basic_static_string&
    {
        return append(t);
    }

    constexpr auto compare(basic_static_string const& str) const noexcept -> int
    {
        std::basic_string_view<CharT, Traits> const sv = str;
        return compare(sv);
    }
    constexpr auto compare(CharT const* s) const -> int
    {
        std::basic_string_view<CharT, Traits> const sv = s;
        return compare(sv);
    }
    constexpr auto compare(detail::string_view_like<CharT, Traits> auto const& t) const
        noexcept(std::is_nothrow_convertible_v<const_reference, std::basic_string_view<CharT, Traits>>) -> int
    {
        std::basic_string_view<CharT, Traits> const sv = t;

        size_type const rlen = std::min(size(), sv.size());
        auto const      cmp  = Traits::compare(data(), sv.data(), rlen);
        if (cmp < 0)
            return -1;
        if (cmp > 0)
            return 1;

        if (size() < sv.size())
            return -1;
        if (size() == sv.size())
            return 0;
        return 1;
    }

    constexpr auto starts_with(std::basic_string_view<CharT, Traits> sv) const noexcept -> bool
    {
        std::basic_string_view<CharT, Traits> const self(data(), size());
        return self.starts_with(sv);
    }
    constexpr auto starts_with(CharT c) const noexcept -> bool
    {
        std::basic_string_view<CharT, Traits> const self(data(), size());
        return self.starts_with(c);
    }
    constexpr auto starts_with(CharT const* s) const -> bool
    {
        std::basic_string_view<CharT, Traits> const self(data(), size());
        return self.starts_with(s);
    }

    constexpr auto ends_with(std::basic_string_view<CharT, Traits> sv) const noexcept -> bool
    {
        std::basic_string_view<CharT, Traits> const self(data(), size());
        return self.ends_with(sv);
    }
    constexpr auto ends_with(CharT c) const noexcept -> bool
    {
        std::basic_string_view<CharT, Traits> const self(data(), size());
        return self.ends_with(c);
    }
    constexpr auto ends_with(CharT const* s) const -> bool
    {
        std::basic_string_view<CharT, Traits> const self(data(), size());
        return self.ends_with(s);
    }

    constexpr auto replace(const_iterator first, const_iterator last, basic_static_string const& str)
        -> basic_static_string&
    {
        return replace(first, last, std::basic_string_view<CharT, Traits>(str));
    }
    template<std::input_iterator Iter, std::sentinel_for<Iter> Sentinel>
    constexpr auto replace(const_iterator first, const_iterator last, Iter first2, Sentinel last2)
        -> basic_static_string&
    {
        insert(erase(first, last), first2, last2);
        return *this;
    }
    constexpr auto replace(const_iterator first, const_iterator last, CharT const* cstr, size_type count2)
        -> basic_static_string&
    {
        return replace(first, last, std::basic_string_view<CharT, Traits>(cstr, count2));
    }
    constexpr auto replace(const_iterator first, const_iterator last, CharT const* cstr) -> basic_static_string&
    {
        return replace(first, last, std::basic_string_view<CharT, Traits>(cstr));
    }
    constexpr auto replace(const_iterator                                      first,
                           const_iterator                                      last,
                           detail::string_view_like<CharT, Traits> auto const& t) -> basic_static_string&
    {
        std::basic_string_view<CharT, Traits> const sv = t;
        return replace(first, last, sv.begin(), sv.end());
    }

    constexpr auto substr(size_type pos = 0, size_type count = npos) const -> std::basic_string_view<CharT, Traits>
    {
        std::basic_string_view<CharT, Traits> sv(*this);
        return sv.substr(pos, count);
    }

    constexpr static size_type npos = -1;

    // -- internal API

    static_vector<CharT, Capacity + 1> storage{CharT{'\0'}};
};

template<class CharT, std::size_t Capacity>
basic_static_string(CharT const (&c)[Capacity]) -> basic_static_string<CharT, Capacity>;

template<class CharT, std::size_t Capacity1, std::size_t Capacity2, class Traits>
constexpr auto operator+(basic_static_string<CharT, Capacity1, Traits> const& lhs,
                         basic_static_string<CharT, Capacity2, Traits> const& rhs)
    -> basic_static_string<CharT, Capacity1 + Capacity2, Traits>
{
    basic_static_string<CharT, Capacity1 + Capacity2, Traits> result(lhs);
    result.append(rhs);
    return result;
}
template<class CharT, std::size_t Capacity, class Traits>
constexpr auto operator+(basic_static_string<CharT, Capacity, Traits> const& lhs, CharT rhs)
    -> basic_static_string<CharT, Capacity + 1, Traits>
{
    basic_static_string<CharT, Capacity + 1, Traits> result(lhs);
    result.append(rhs);
    return result;
}
template<class CharT, std::size_t Capacity, class Traits>
constexpr auto operator+(CharT lhs, basic_static_string<CharT, Capacity, Traits> const& rhs)
    -> basic_static_string<CharT, Capacity + 1, Traits>
{
    basic_static_string<CharT, Capacity + 1, Traits> result(lhs);
    result.append(rhs);
    return result;
}

template<class CharT, std::size_t Capacity, class Traits>
constexpr auto operator==(basic_static_string<CharT, Capacity, Traits> const& lhs,
                          basic_static_string<CharT, Capacity, Traits> const& rhs) noexcept -> bool
{
    return std::ranges::equal(lhs, rhs);
}

template<class CharT, std::size_t Capacity, class Traits>
constexpr auto operator==(basic_static_string<CharT, Capacity, Traits> const& lhs, CharT const* rhs) -> bool
{
    return std::ranges::equal(lhs, std::basic_string_view(rhs));
}

template<class CharT, std::size_t Capacity, class Traits>
constexpr auto operator==(basic_static_string<CharT, Capacity, Traits> const& lhs,
                          detail::string_view_like<CharT, Traits> auto const& rhs) -> bool
{
    return std::ranges::equal(lhs, rhs);
}

template<class CharT, std::size_t Capacity, class Traits>
constexpr auto operator<=>(basic_static_string<CharT, Capacity, Traits> const& lhs,
                           basic_static_string<CharT, Capacity, Traits> const& rhs) noexcept
{
    return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class CharT, std::size_t Capacity, class Traits>
constexpr auto operator<=>(basic_static_string<CharT, Capacity, Traits> const& lhs, CharT const* rhs) noexcept
{
    return lhs <=> std::basic_string_view<CharT, Traits>(rhs);
}

template<class CharT, std::size_t Capacity, class Traits>
constexpr auto operator<=>(basic_static_string<CharT, Capacity, Traits> const& lhs,
                           detail::string_view_like<CharT, Traits> auto const& rhs) noexcept
{
    std::basic_string_view<CharT, Traits> sv = rhs;
    return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), sv.begin(), sv.end());
}

template<class CharT, std::size_t Capacity, class Traits, class U>
constexpr auto erase(basic_static_string<CharT, Capacity, Traits>& c, U const& value) ->
    typename basic_static_string<CharT, Capacity, Traits>::size_type
{
    auto it = std::remove(c.begin(), c.end(), value);
    auto r  = std::distance(it, c.end());
    c.erase(it, c.end());
    return r;
}

template<class CharT, std::size_t Capacity, class Traits, class Pred>
constexpr auto erase_if(basic_static_string<CharT, Capacity, Traits>& c, Pred pred) ->
    typename basic_static_string<CharT, Capacity, Traits>::size_type
{
    auto it = std::remove_if(c.begin(), c.end(), pred);
    auto r  = std::distance(it, c.end());
    c.erase(it, c.end());
    return r;
}

template<class CharT, std::size_t Capacity, class Traits>
auto operator<<(std::basic_ostream<CharT, Traits>& os, basic_static_string<CharT, Capacity, Traits> const& str)
    -> std::basic_ostream<CharT, Traits>&
{
    return os << std::basic_string_view<CharT, Traits>(str);
}
} // namespace structural

namespace std
{
template<class CharT, std::size_t Capacity, class Traits>
struct hash<structural::basic_static_string<CharT, Capacity, Traits>>
{
    auto operator()(structural::basic_static_string<CharT, Capacity, Traits> const& str) const -> std::size_t
    {
        return hash<std::basic_string_view<CharT, Traits>>{}(str);
    }
};
} // namespace std

#endif // STRUCTURAL_BASIC_STATIC_STRING_HPP
