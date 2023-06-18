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
/// Stores and allows manipulation of contiguous character sequences.
///
/// # Notes
/// All characters are stored within the object, no dynamic memory is allocated.
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

    /// Constructs an empty string
    constexpr basic_static_string() = default;

    /// Constructs a copy of the given string
    ///
    /// # Parameters:
    /// - s: string with lesser capacity than the string to construct.
    template<std::size_t C>
        requires(C < Capacity)
    constexpr basic_static_string(basic_static_string<CharT, C, Traits> const& s)
        : basic_static_string(s.begin(), s.end())
    {
    }

    /// Constructs a string of length 1
    constexpr basic_static_string(CharT ch)
        : basic_static_string({ch})
    {
    }

    /// Constructs a string from a legacy pointer and size combination
    ///
    /// # Requires
    /// `s` must point to a valid character string of length `count` or greater.
    ///
    /// # Notes
    /// Independent of whether `s` contains null characters, the resulting string has length `count`.
    constexpr basic_static_string(CharT const* s, size_type count)
        : storage(s, s + count)
    {
        storage.push_back(CharT('\0'));
    }

    /// Constructs a string from the passed pointer
    ///
    /// # Requires
    /// The argument must point to a null-terminated string.
    constexpr basic_static_string(CharT const* s)
        : basic_static_string(std::basic_string_view<CharT, Traits>{s})
    {
    }

    /// Constructs a string from a pair of iterators
    ///
    /// # Requires
    /// The distance between `begin` and `end` must not be greater than `Capacity`.
    template<std::input_iterator Iter, std::sentinel_for<Iter> Sentinel>
    constexpr basic_static_string(Iter begin, Sentinel end)
        : storage(begin, end)
    {
        storage.push_back(CharT('\0'));
    }

    /// Constructs a string from an initializer list
    ///
    /// # Requires
    /// The initializer list must not be longer than `Capacity`.
    constexpr basic_static_string(std::initializer_list<CharT> ilist)
        : basic_static_string(ilist.begin(), ilist.end())
    {
    }

    /// Constructs a string from a string_view or similar type
    ///
    /// # Requires
    /// The argument's length must not be greater than `Capacity`.
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

    /// Replaces the content of this string with the content of another string of lesser capacity
    template<std::size_t C>
        requires(C < Capacity)
    constexpr auto operator=(basic_static_string<CharT, C, Traits> const& s) -> basic_static_string&
    {
        *this = basic_static_string(s);
        return *this;
    }

    /// Replaces the content of this string with the characters from a null-terminated legacy string
    ///
    /// # Requires
    /// `s` must be a valid pointer to a character array that contains a null character within at most `Capacity` from
    /// the beginning.
    constexpr auto operator=(CharT const* s) -> basic_static_string&
    {
        *this = basic_static_string(s);
        return *this;
    }

    /// Replaces the content of this string with a single character
    constexpr auto operator=(CharT ch) -> basic_static_string&
    {
        *this = basic_static_string(ch);
        return *this;
    }

    /// Replaces the content of this string with the characters from an initializer list
    ///
    /// # Requires
    /// The length of the initializer list must not be greater than `Capacity`.
    constexpr auto operator=(std::initializer_list<CharT> ilist) -> basic_static_string&
    {
        *this = basic_static_string(ilist);
        return *this;
    }

    /// Replaces the content of this string with the content of a string_view-like type
    ///
    /// # Requires
    /// The argument's length must not be greater than `Capacity`.
    constexpr auto operator=(detail::string_view_like<CharT, Traits> auto const& t) -> basic_static_string&
    {
        *this = basic_static_string(t);
        return *this;
    }

    constexpr auto operator=(std::nullptr_t) -> basic_static_string& = delete;

    /// Replaces the content of this string with the content of another string of lesser or equal capacity
    template<std::size_t C>
        requires(C <= Capacity)
    constexpr auto assign(basic_static_string<CharT, C, Traits> const& str) -> basic_static_string&
    {
        storage.assign(str.storage.begin(), str.storage.end());
        return *this;
    }

    /// Replaces the content of this string with the content of another string of lesser or equal capacity
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

    /// Replaces the content of this string with the content of a legacy pointer and size pair
    ///
    /// # Requires
    /// `s` must be a valid pointer to a character array of size `count` or greater.
    constexpr auto assign(CharT const* s, size_type count) -> basic_static_string&
    {
        return assign(std::basic_string_view<CharT, Traits>(s, count));
    }

    /// Replaces the content of this string with the characters from a null-terminated legacy string
    ///
    /// # Requires
    /// `s` must be a valid pointer to a character array that contains a null character within at most `Capacity` from
    /// the beginning.
    constexpr auto assign(CharT const* s) -> basic_static_string&
    {
        return assign(std::basic_string_view<CharT, Traits>(s));
    }

    /// Replaces the content of this string with the content defined by a pair of iterators
    ///
    /// # Requires
    /// The distance between `first` and `last` must not be greater than `Capacity`.
    template<std::input_iterator Iter, std::sentinel_for<Iter> Sentinel>
    constexpr auto assign(Iter first, Sentinel last) -> basic_static_string&
    {
        storage.assign(first, last);
        storage.push_back(CharT('\0'));
        return *this;
    }

    /// Replaces the content of this string with the characters from an initializer list
    ///
    /// # Requires
    /// The length of the initializer list must not be greater than `Capacity`.
    constexpr auto assign(std::initializer_list<CharT> ilist) -> basic_static_string&
    {
        return assign(ilist.begin(), ilist.end());
    }

    /// Replaces the content of this string with the content of a string_view-like type
    ///
    /// # Requires
    /// The argument's length must not be greater than `Capacity`.
    constexpr auto assign(detail::string_view_like<CharT, Traits> auto const& t) -> basic_static_string&
    {
        std::basic_string_view<CharT, Traits> const sv = t;
        return assign(sv.begin(), sv.end());
    }

    /// Returns a reference to the character at the specified position
    ///
    /// # Exceptions
    /// Throws `std::out_of_range` if `pos` > size().
    constexpr auto at(size_type pos) -> reference
    {
        if (pos >= size())
            throw std::out_of_range{"basic_static_string::at"};
        return storage.at(pos);
    }

    /// Returns a reference to the character at the specified position
    ///
    /// # Exceptions
    /// Throws `std::out_of_range` if `pos` > size().
    constexpr auto at(size_type pos) const -> const_reference
    {
        if (pos >= size())
            throw std::out_of_range{"basic_static_string::at"};
        return storage.at(pos);
    }

    /// Returns a reference to the character at the specified position
    ///
    /// # Requires
    /// size() > `pos`
    constexpr auto operator[](size_type pos) -> reference { return storage[pos]; }

    /// Returns a reference to the character at the specified position
    ///
    /// # Requires
    /// size() > `pos`
    constexpr auto operator[](size_type pos) const -> const_reference { return storage[pos]; }

    /// Returns a reference to the first character in the string
    ///
    /// # Requires
    /// size() > 0
    constexpr auto front() -> reference { return storage.front(); }

    /// Returns a reference to the first character in the string
    ///
    /// # Requires
    /// size() > 0
    constexpr auto front() const -> const_reference { return storage.front(); }

    /// Returns a reference to the last character in the string
    ///
    /// # Requires
    /// size() > 0
    constexpr auto back() -> reference { return *(end() - 1); };

    /// Returns a reference to the last character in the string
    ///
    /// # Requires
    /// size() > 0
    constexpr auto back() const -> const_reference { return *(end() - 1); };

    /// Returns a pointer to the underlying array
    ///
    /// # Return value
    /// The returned pointer points to a null-terminated character array.
    ///
    /// # Notes
    /// If the string is empty, the returned pointer points to a single null terminator.
    constexpr auto data() const noexcept -> CharT const* { return storage.array.data(); }

    /// Returns a pointer to the underlying array
    ///
    /// # Return value
    /// The returned pointer points to a null-terminated character array.
    ///
    /// # Notes
    /// If the string is empty, the returned pointer points to a single null terminator.
    constexpr auto data() noexcept -> pointer { return storage.array.data(); }

    /// Returns a pointer to the underlying array
    ///
    /// # Notes
    /// Same functionality as data().
    constexpr auto c_str() const noexcept -> CharT const* { return data(); };

    /// Returns a string_view representing the entire string
    constexpr operator std::basic_string_view<CharT, Traits>() const noexcept
    {
        return std::basic_string_view<CharT, Traits>(data(), size());
    }

    /// Returns an iterator to the first character of the string
    constexpr auto begin() noexcept -> iterator { return storage.begin(); }
    /// Returns an iterator to the first character of the string
    constexpr auto begin() const noexcept -> const_iterator { return storage.begin(); }
    /// Returns an iterator to the first character of the string
    constexpr auto cbegin() const noexcept -> const_iterator { return storage.cbegin(); }

    /// Returns a reverse iterator to the first character of the reversed string
    constexpr auto rbegin() noexcept -> reverse_iterator { return storage.rbegin() + 1; }
    /// Returns a reverse iterator to the first character of the reversed string
    constexpr auto rbegin() const noexcept -> const_reverse_iterator { return storage.rbegin() + 1; }
    /// Returns a reverse iterator to the first character of the reversed string
    constexpr auto crbegin() const noexcept -> const_reverse_iterator { return storage.crbegin() + 1; }

    /// Returns an iterator past the last character of the string
    constexpr auto end() noexcept -> iterator { return storage.end() - 1; }
    /// Returns an iterator past the last character of the string
    constexpr auto end() const noexcept -> const_iterator { return storage.end() - 1; }
    /// Returns an iterator past the last character of the string
    constexpr auto cend() const noexcept -> const_iterator { return storage.cend() - 1; }

    /// Returns a reverse iterator past the last character of the reversed string
    constexpr auto rend() noexcept -> reverse_iterator { return storage.rend(); }
    /// Returns a reverse iterator past the last character of the reversed string
    constexpr auto rend() const noexcept -> const_reverse_iterator { return storage.rend(); }
    /// Returns a reverse iterator past the last character of the reversed string
    constexpr auto crend() const noexcept -> const_reverse_iterator { return storage.crend(); }

    /// Checks whether the string has no content
    [[nodiscard]] constexpr auto empty() const noexcept -> bool { return size() == 0; }

    /// Returns the size of the string
    constexpr auto size() const noexcept -> size_type { return storage.size() - 1; }
    /// Returns the size of the string
    constexpr auto length() const noexcept -> size_type { return size(); }

    /// Returns `Capacity`
    constexpr auto capacity() const noexcept -> size_type { return Capacity; }

    /// Removes all characters from this string
    ///
    /// # Notes
    /// All pointers, references and iterators to elements of this string are invalidated.
    constexpr void clear() noexcept
    {
        storage.clear();
        storage.push_back(CharT('\0'));
    }

    /// Inserts a character at the specified position
    ///
    /// # Requires
    /// The size() must be less than `Capacity`
    ///
    /// # Return value
    /// An iterator to the inserted character.
    ///
    /// # Notes
    /// Doesn't invalidate any iterators, references or pointers into the string, but they may point to different
    /// characters after insertion.
    constexpr auto insert(const_iterator pos, CharT ch) -> iterator { return storage.insert(pos, ch); }

    /// Inserts a null terminated string at the specified position
    ///
    /// # Requires
    /// - `s` must be a valid pointer to a null-terminated character string
    /// - The combined length of this string and `s` must not exceed `Capacity`.
    ///
    /// # Return value
    /// An iterator to the first inserted character.
    ///
    /// # Notes
    /// Doesn't invalidate any iterators, references or pointers into the string, but they may point to different
    /// characters after insertion.
    constexpr auto insert(const_iterator pos, CharT const* s) -> iterator
    {
        return insert(pos, std::basic_string_view<CharT, Traits>(s));
    }

    /// Inserts characters from a pointer and length at the specified position
    ///
    /// # Requires
    /// - `s` must be a valid pointer to a character string of length `count` or greater
    /// - The combined length of this string and `count` must not exceed `Capacity`.
    ///
    /// # Return value
    /// An iterator to the first inserted character.
    ///
    /// # Notes
    /// Doesn't invalidate any iterators, references or pointers into the string, but they may point to different
    /// characters after insertion.
    constexpr auto insert(const_iterator pos, CharT const* s, size_type count) -> iterator
    {
        return insert(pos, std::basic_string_view<CharT, Traits>(s, count));
    }

    /// Inserts a string at the specified position
    ///
    /// # Requires
    /// The combined length of this string and `str` must not exceed `Capacity`.
    ///
    /// # Return value
    /// An iterator to the first inserted character.
    ///
    /// # Notes
    /// Doesn't invalidate any iterators, references or pointers into the string, but they may point to different
    /// characters after insertion.
    constexpr auto insert(const_iterator pos, basic_static_string const& str) -> iterator
    {
        return insert(pos, str.begin(), str.end());
    }

    /// Inserts the content of a string_view like object at the specified position
    ///
    /// # Requires
    /// The combined length of this string and `t` must not exceed `Capacity`.
    ///
    /// # Return value
    /// An iterator to the first inserted character.
    ///
    /// # Notes
    /// Doesn't invalidate any iterators, references or pointers into the string, but they may point to different
    /// characters after insertion.
    constexpr auto insert(const_iterator pos, detail::string_view_like<CharT, Traits> auto const& t) -> iterator
    {
        std::basic_string_view<CharT, Traits> sv = t;
        return insert(pos, sv.begin(), sv.end());
    }

    /// Inserts the characters specified by two iterators at the specified position
    ///
    /// # Requires
    /// The combined length of this string and the distance between `first` and `last` must not exceed `Capacity`.
    ///
    /// # Return value
    /// An iterator to the first inserted character.
    ///
    /// # Notes
    /// Doesn't invalidate any iterators, references or pointers into the string, but they may point to different
    /// characters after insertion.
    template<std::input_iterator Iter, std::sentinel_for<Iter> Sentinel>
    constexpr auto insert(const_iterator pos, Iter first, Sentinel last) -> iterator
    {
        return storage.insert(pos, first, last);
    }

    /// Inserts the characters from an initializer_list at the specified position
    ///
    /// # Requires
    /// The combined length of this string and `ilist` must not exceed `Capacity`.
    ///
    /// # Return value
    /// An iterator to the first inserted character.
    ///
    /// # Notes
    /// Doesn't invalidate any iterators, references or pointers into the string, but they may point to different
    /// characters after insertion.
    constexpr auto insert(const_iterator pos, std::initializer_list<CharT> ilist) -> iterator
    {
        return insert(pos, ilist.begin(), ilist.end());
    }

    /// Removes the specified character from the string
    ///
    /// # Return value
    /// Returns an iterator to the character immediately after the erased character, or end() if none.
    ///
    /// # Notes
    /// Invalidates any iterators, references or pointers to the last character of the string. Iterators, references or
    /// pointers to characters between `position` and the last character may point to different characters after
    /// erasure.
    constexpr auto erase(const_iterator position) -> iterator { return storage.erase(position); }

    /// Removes all character in the specified range from the string
    ///
    /// # Return value
    /// Returns an iterator to the character immediately after the last erased character, or end() if none.
    ///
    /// # Notes
    /// Invalidates any iterators, references or pointers to the last n characters of the string, where n is the
    /// distance between `first` and `last`. Iterators, references or pointers to characters between `first` and the
    /// size()-n character may point to different characters after erasure.
    constexpr auto erase(const_iterator first, const_iterator last) -> iterator { return storage.erase(first, last); }

    /// Appends the given character to the end of the string
    ///
    /// # Requires
    /// size() < `Capacity`.
    ///
    /// # Notes
    /// Doesn't invalidate any iterators, references or pointers into this string.
    constexpr void push_back(CharT ch)
    {
        storage.back() = ch;
        storage.push_back(CharT('\0'));
    }

    /// Removes the last character from the end of the string
    ///
    /// # Requires
    /// size() > 0.
    ///
    /// # Notes
    /// Invalidate any iterators, references or pointers to the last character of this string.
    constexpr void pop_back()
    {
        storage.pop_back();
        storage.back() = CharT('\0');
    }

    /// Appends the given string to the end of the string
    ///
    /// # Requires
    /// The combined size of this string and `str` must not be greater than `Capacity`.
    ///
    /// # Notes
    /// Doesn't invalidate any iterators, references or pointers into this string.
    constexpr auto append(basic_static_string const& str) -> basic_static_string&
    {
        storage.insert(end(), str.begin(), str.end());
        return *this;
    }

    /// Appends the given character to the end of the string
    ///
    /// # Requires
    /// size() < `Capacity`
    ///
    /// # Notes
    /// Doesn't invalidate any iterators, references or pointers into this string.
    constexpr auto append(CharT ch) -> basic_static_string&
    {
        storage.insert(end(), ch);
        return *this;
    }

    /// Appends the first `count` characters of given c style string the end of the string
    ///
    /// # Requires
    /// - The combined size of this string and `count` must not be greater than `Capacity`.
    /// - `s` must point be a valid pointer to a character array of length `count` or greater.
    ///
    /// # Notes
    /// Doesn't invalidate any iterators, references or pointers into this string.
    constexpr auto append(CharT const* s, size_type count) -> basic_static_string&
    {
        std::basic_string_view<CharT, Traits> sv(s, count);
        return append(sv);
    }

    /// Appends the given c style string to the end of the string
    ///
    /// # Requires
    /// - The combined size of this string and `s` must not be greater than `Capacity`.
    /// - `s` must be a null terminated string.
    ///
    /// # Notes
    /// Doesn't invalidate any iterators, references or pointers into this string.
    constexpr auto append(CharT const* s) -> basic_static_string&
    {
        std::basic_string_view<CharT, Traits> sv(s);
        return append(sv);
    }

    /// Appends the characters from the given range to the end of the string
    ///
    /// # Requires
    /// The combined size of this string and the distance between `first` and `last` must not be greater than
    /// `Capacity`.
    ///
    /// # Notes
    /// Doesn't invalidate any iterators, references or pointers into this string.
    template<std::input_iterator Iter, std::sentinel_for<Iter> Sentinel>
    constexpr auto append(Iter first, Sentinel last) -> basic_static_string&
    {
        storage.insert(end(), first, last);
        return *this;
    }

    /// Appends the characters from the given initializer list to the end of the string
    ///
    /// # Requires
    /// The combined size of this string and `ilist` must not be greater than `Capacity`.
    ///
    /// # Notes
    /// Doesn't invalidate any iterators, references or pointers into this string.
    constexpr auto append(std::initializer_list<CharT> ilist) -> basic_static_string&
    {
        storage.insert(end(), ilist);
        return *this;
    }

    /// Appends the characters from the given string_view-like type to the end of the string
    ///
    /// # Requires
    /// The combined size of this string and `t` must not be greater than `Capacity`.
    ///
    /// # Notes
    /// Doesn't invalidate any iterators, references or pointers into this string.
    constexpr auto append(detail::string_view_like<CharT, Traits> auto const& t) -> basic_static_string&
    {
        std::basic_string_view<CharT, Traits> sv = t;
        return append(sv.begin(), sv.end());
    }

    /// Equivalent to append(str)
    constexpr auto operator+=(basic_static_string const& str) -> basic_static_string& { return append(str); }

    /// Equivalent to append(ch)
    constexpr auto operator+=(CharT ch) -> basic_static_string& { return append(ch); };

    /// Equivalent to append(s)
    constexpr auto operator+=(CharT const* s) -> basic_static_string& { return append(s); }

    /// Equivalent to append(ilist)
    constexpr auto operator+=(std::initializer_list<CharT> ilist) -> basic_static_string& { return append(ilist); }

    /// Equivalent to append(t)
    constexpr auto operator+=(detail::string_view_like<CharT, Traits> auto const& t) -> basic_static_string&
    {
        return append(t);
    }

    /// Compares the strings lexicographically
    ///
    /// # Return value
    /// Returns a negative value, if `*this` is lexicographically ordered before `str`; zero, if `*this` and `str`
    /// compare equivalent; a positive value, if `*this` is ordered after `str`.
    constexpr auto compare(basic_static_string const& str) const noexcept -> int
    {
        std::basic_string_view<CharT, Traits> const sv = str;
        return compare(sv);
    }

    /// Compares the strings lexicographically
    ///
    /// # Requires
    /// `s` must be a valid pointer to a null terminated character array.
    ///
    /// # Return value
    /// Returns a negative value, if `*this` is lexicographically ordered before `s`; zero, if `*this` and `s`
    /// compare equivalent; a positive value, if `*this` is ordered after `s`.
    constexpr auto compare(CharT const* s) const -> int
    {
        std::basic_string_view<CharT, Traits> const sv = s;
        return compare(sv);
    }

    /// Compares the strings lexicographically
    ///
    /// # Return value
    /// Returns a negative value, if `*this` is lexicographically ordered before `t`; zero, if `*this` and `t`
    /// compare equivalent; a positive value, if `*this` is ordered after `t`.
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

    /// Checks if this string has a specific prefix
    constexpr auto starts_with(std::basic_string_view<CharT, Traits> sv) const noexcept -> bool
    {
        std::basic_string_view<CharT, Traits> const self(data(), size());
        return self.starts_with(sv);
    }

    /// Checks if this string has a specific prefix
    constexpr auto starts_with(CharT c) const noexcept -> bool
    {
        std::basic_string_view<CharT, Traits> const self(data(), size());
        return self.starts_with(c);
    }

    /// Checks if this string has a specific prefix
    ///
    /// # Requires
    /// `s` must be a valid pointer to a null terminated character array.
    constexpr auto starts_with(CharT const* s) const -> bool
    {
        std::basic_string_view<CharT, Traits> const self(data(), size());
        return self.starts_with(s);
    }

    /// Checks if this string has a specific postfix
    constexpr auto ends_with(std::basic_string_view<CharT, Traits> sv) const noexcept -> bool
    {
        std::basic_string_view<CharT, Traits> const self(data(), size());
        return self.ends_with(sv);
    }

    /// Checks if this string has a specific postfix
    constexpr auto ends_with(CharT c) const noexcept -> bool
    {
        std::basic_string_view<CharT, Traits> const self(data(), size());
        return self.ends_with(c);
    }

    /// Checks if this string has a specific postfix
    ///
    /// # Requires
    /// `s` must be a valid pointer to a null terminated character array.
    constexpr auto ends_with(CharT const* s) const -> bool
    {
        std::basic_string_view<CharT, Traits> const self(data(), size());
        return self.ends_with(s);
    }

    /// Replaces a section of this string with another string
    ///
    /// # Requires
    /// - The distance between `first` and `last` must not be negative.
    /// - size() - distance(first, last) + str.size() <= Capacity.
    constexpr auto replace(const_iterator first, const_iterator last, basic_static_string const& str)
        -> basic_static_string&
    {
        return replace(first, last, std::basic_string_view<CharT, Traits>(str));
    }

    /// Replaces a section of this string with another string
    ///
    /// # Requires
    /// - The distance between `first` and `last` must not be negative.
    /// - size() - distance(first, last) + distance(first2, last2) <= Capacity.
    template<std::input_iterator Iter, std::sentinel_for<Iter> Sentinel>
    constexpr auto replace(const_iterator first, const_iterator last, Iter first2, Sentinel last2)
        -> basic_static_string&
    {
        insert(erase(first, last), first2, last2);
        return *this;
    }

    /// Replaces a section of this string with another string
    ///
    /// # Requires
    /// - The distance between `first` and `last` must not be negative.
    /// - `cstr` must be a valid pointer to a character array of size `count2` or greater.
    /// - size() - distance(first, last) + count2 <= Capacity.
    constexpr auto replace(const_iterator first, const_iterator last, CharT const* cstr, size_type count2)
        -> basic_static_string&
    {
        return replace(first, last, std::basic_string_view<CharT, Traits>(cstr, count2));
    }

    /// Replaces a section of this string with another string
    ///
    /// # Requires
    /// - The distance between `first` and `last` must not be negative.
    /// - `cstr` must be a valid pointer to a null terminated character array.
    /// - size() - distance(first, last) + strlen(cstr) <= Capacity.
    constexpr auto replace(const_iterator first, const_iterator last, CharT const* cstr) -> basic_static_string&
    {
        return replace(first, last, std::basic_string_view<CharT, Traits>(cstr));
    }

    /// Replaces a section of this string with another string
    ///
    /// # Requires
    /// - The distance between `first` and `last` must not be negative.
    /// - size() - distance(first, last) + t.size() <= Capacity.
    constexpr auto replace(const_iterator                                      first,
                           const_iterator                                      last,
                           detail::string_view_like<CharT, Traits> auto const& t) -> basic_static_string&
    {
        std::basic_string_view<CharT, Traits> const sv = t;
        return replace(first, last, sv.begin(), sv.end());
    }

    /// Returns a view to a substring
    ///
    /// # Return value
    /// A view of the substring [pos, pos + c], where c is min(count, size() - pos).
    ///
    /// # Exceptions
    /// std::out_of_range if `pos` > `size()`
    constexpr auto substr(size_type pos = 0, size_type count = npos) const -> std::basic_string_view<CharT, Traits>
    {
        std::basic_string_view<CharT, Traits> sv(*this);
        return sv.substr(pos, count);
    }

    /// Special value equal to the maximum representable value of size_type. Generally used as an end of string
    /// indicator.
    constexpr static size_type npos = -1;

    // -- internal API

    static_vector<CharT, Capacity + 1> storage{CharT{'\0'}};
};

template<class CharT, std::size_t Capacity>
basic_static_string(CharT const (&c)[Capacity]) -> basic_static_string<CharT, Capacity>;

/// Returns a string with the combined capacity of the input strings containing the two string appended to each other
template<class CharT, std::size_t Capacity1, std::size_t Capacity2, class Traits>
constexpr auto operator+(basic_static_string<CharT, Capacity1, Traits> const& lhs,
                         basic_static_string<CharT, Capacity2, Traits> const& rhs)
    -> basic_static_string<CharT, Capacity1 + Capacity2, Traits>
{
    basic_static_string<CharT, Capacity1 + Capacity2, Traits> result(lhs);
    result.append(rhs);
    return result;
}

/// Returns a string with the capacity of `lhs`, containing the two string appended to each other
template<class CharT, std::size_t Capacity, class Traits>
constexpr auto operator+(basic_static_string<CharT, Capacity, Traits> const& lhs, CharT rhs)
    -> basic_static_string<CharT, Capacity + 1, Traits>
{
    basic_static_string<CharT, Capacity + 1, Traits> result(lhs);
    result.append(rhs);
    return result;
}

/// Returns a string with the capacity of `rhs`, containing the two string appended to each other
template<class CharT, std::size_t Capacity, class Traits>
constexpr auto operator+(CharT lhs, basic_static_string<CharT, Capacity, Traits> const& rhs)
    -> basic_static_string<CharT, Capacity + 1, Traits>
{
    basic_static_string<CharT, Capacity + 1, Traits> result(lhs);
    result.append(rhs);
    return result;
}

/// Compares two strings for equality
template<class CharT, std::size_t Capacity, class Traits>
constexpr auto operator==(basic_static_string<CharT, Capacity, Traits> const& lhs,
                          basic_static_string<CharT, Capacity, Traits> const& rhs) noexcept -> bool
{
    return std::ranges::equal(lhs, rhs);
}

/// Compares two strings for equality
///
/// # Requires
/// `rhs` is a valid pointer to a null terminated character array.
template<class CharT, std::size_t Capacity, class Traits>
constexpr auto operator==(basic_static_string<CharT, Capacity, Traits> const& lhs, CharT const* rhs) -> bool
{
    return std::ranges::equal(lhs, std::basic_string_view(rhs));
}

/// Compares two strings for equality
template<class CharT, std::size_t Capacity, class Traits>
constexpr auto operator==(basic_static_string<CharT, Capacity, Traits> const& lhs,
                          detail::string_view_like<CharT, Traits> auto const& rhs) -> bool
{
    return std::ranges::equal(lhs, rhs);
}

/// Three-way compares two strings lexicographically
template<class CharT, std::size_t Capacity, class Traits>
constexpr auto operator<=>(basic_static_string<CharT, Capacity, Traits> const& lhs,
                           basic_static_string<CharT, Capacity, Traits> const& rhs) noexcept
{
    return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

/// Three-way compares two strings lexicographically
///
/// # Requires
/// `rhs` is a valid pointer to a null terminated character array.
template<class CharT, std::size_t Capacity, class Traits>
constexpr auto operator<=>(basic_static_string<CharT, Capacity, Traits> const& lhs, CharT const* rhs) noexcept
{
    return lhs <=> std::basic_string_view<CharT, Traits>(rhs);
}

/// Three-way compares two strings lexicographically
template<class CharT, std::size_t Capacity, class Traits>
constexpr auto operator<=>(basic_static_string<CharT, Capacity, Traits> const& lhs,
                           detail::string_view_like<CharT, Traits> auto const& rhs) noexcept
{
    std::basic_string_view<CharT, Traits> sv = rhs;
    return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), sv.begin(), sv.end());
}

/// Erases all elements in `c` that compare equal to `value`
template<class CharT, std::size_t Capacity, class Traits, class U>
constexpr auto erase(basic_static_string<CharT, Capacity, Traits>& c, U const& value) ->
    typename basic_static_string<CharT, Capacity, Traits>::size_type
{
    auto it = std::remove(c.begin(), c.end(), value);
    auto r  = std::distance(it, c.end());
    c.erase(it, c.end());
    return r;
}

/// Erases all elements in `c` for which `pred` returns true
template<class CharT, std::size_t Capacity, class Traits, class Pred>
constexpr auto erase_if(basic_static_string<CharT, Capacity, Traits>& c, Pred pred) ->
    typename basic_static_string<CharT, Capacity, Traits>::size_type
{
    auto it = std::remove_if(c.begin(), c.end(), pred);
    auto r  = std::distance(it, c.end());
    c.erase(it, c.end());
    return r;
}

/// Equivalent to `os << str.substr()`
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
