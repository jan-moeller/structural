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

#ifndef STRUCTURAL_STATIC_UNORDERED_SET_HPP
#define STRUCTURAL_STATIC_UNORDERED_SET_HPP

#include "structural/detail/static_hash_table.hpp"

namespace structural
{
/// An associative container with average constant-time complexity for search, insertion and removal.
///
/// # Notes
/// Performance degrades towards linear if size() is close to Capacity. It is therefore advisable to chose a Capacity at
/// least twice of the expected number of contained elements.
template<typename Key, std::size_t Capacity, typename Hash = hash<Key>, typename Equal = std::equal_to<Key>>
struct static_unordered_set
{
    using key_type        = typename detail::static_hash_table<Key, Capacity, Hash, Equal>::key_type;
    using value_type      = typename detail::static_hash_table<Key, Capacity, Hash, Equal>::value_type;
    using size_type       = typename detail::static_hash_table<Key, Capacity, Hash, Equal>::size_type;
    using difference_type = typename detail::static_hash_table<Key, Capacity, Hash, Equal>::difference_type;
    using hasher          = typename detail::static_hash_table<Key, Capacity, Hash, Equal>::hasher;
    using key_equal       = typename detail::static_hash_table<Key, Capacity, Hash, Equal>::key_equal;
    using reference       = typename detail::static_hash_table<Key, Capacity, Hash, Equal>::reference;
    using const_reference = typename detail::static_hash_table<Key, Capacity, Hash, Equal>::const_reference;
    using pointer         = typename detail::static_hash_table<Key, Capacity, Hash, Equal>::pointer;
    using const_pointer   = typename detail::static_hash_table<Key, Capacity, Hash, Equal>::const_pointer;
    using iterator        = typename detail::static_hash_table<Key, Capacity, Hash, Equal>::const_iterator;
    using const_iterator  = typename detail::static_hash_table<Key, Capacity, Hash, Equal>::const_iterator;

    /// Constructs an empty set
    constexpr static_unordered_set() = default;

    /// Constructs an empty set with a certain hash function and equality function
    constexpr static_unordered_set(Hash const& hash, key_equal const& equal = key_equal())
        : data(hash, equal)
    {
    }

    /// Constructs a set containing the elements from a range
    ///
    /// # Requires
    /// distance(first, last) < Capacity
    template<std::input_iterator First, std::sentinel_for<First> Last>
    constexpr static_unordered_set(First            first,
                                   Last             last,
                                   Hash const&      hash  = Hash(),
                                   key_equal const& equal = key_equal())
        : static_unordered_set(hash, equal)
    {
        insert(first, last);
    }

    /// Constructs a set containing the elements from an initializer list
    ///
    /// # Requires
    /// distance(first, last) < Capacity
    constexpr static_unordered_set(std::initializer_list<value_type> init,
                                   Hash const&                       hash  = Hash(),
                                   key_equal const&                  equal = key_equal())
        : static_unordered_set(init.begin(), init.end(), hash, equal)
    {
    }

    /// Returns an iterator to the beginning
    constexpr auto begin() const noexcept -> const_iterator { return data.begin(); }
    /// Returns an iterator to the beginning
    constexpr auto cbegin() const noexcept -> const_iterator { return data.begin(); }
    /// Returns an iterator to the end
    constexpr auto end() const noexcept -> const_iterator { return data.end(); }
    /// Returns an iterator to the end
    constexpr auto cend() const noexcept -> const_iterator { return data.end(); }

    /// Checks whether the container is empty
    [[nodiscard]] constexpr auto empty() const noexcept -> bool { return data.empty(); }
    /// Returns the number of elements
    [[nodiscard]] constexpr auto size() const noexcept -> size_type { return data.size(); }
    /// Returns Capacity
    [[nodiscard]] static constexpr auto capacity() noexcept -> size_type { return Capacity; }

    /// Clears the contents
    ///
    /// # Notes
    /// Invalidates all iterators, pointers and references into the container.
    constexpr void clear() noexcept { data.clear(); }

    /// Inserts an element, if no element with equivalent key is already contained
    ///
    /// # Requires
    /// size() < Capacity
    ///
    /// # Return value
    /// A pair consisting of an iterator to the inserted element (or the element preventing insertion) and a bool
    /// indicating whether insertion took place.
    constexpr auto insert(value_type const& value) -> pair<iterator, bool> { return data.insert(value); }

    /// Inserts an element, if no element with equivalent key is already contained
    ///
    /// # Requires
    /// size() < Capacity
    ///
    /// # Return value
    /// A pair consisting of an iterator to the inserted element (or the element preventing insertion) and a bool
    /// indicating whether insertion took place.
    constexpr auto insert(value_type&& value) -> pair<iterator, bool> { return data.insert(std::move(value)); }

    /// Inserts elements from a range, if no element with equivalent key is already contained
    ///
    /// # Requires
    /// - first and last must be a valid range
    /// - size() + distance(first, last) < Capacity
    template<std::input_iterator First, std::sentinel_for<First> Last>
    constexpr void insert(First first, Last last)
    {
        for (auto iter = first; iter != last; ++iter)
            data.insert(*iter);
    }

    /// Inserts elements from a initializer_list, if no element with equivalent key is already contained
    ///
    /// # Requires
    /// size() + init.size() < Capacity
    constexpr void insert(std::initializer_list<value_type> init) { insert(init.begin(), init.end()); }

    /// Inserts an element constructed from args, if no element with equivalent key is already contained
    ///
    /// # Requires
    /// - size() < Capacity
    ///
    /// # Return value
    /// A pair consisting of an iterator to the inserted element (or the element preventing insertion) and a bool
    /// indicating whether insertion took place.
    ///
    /// # Notes
    /// The element will be constructed even if another element with equivalent key already exists in the container. In
    /// this case, the constructed element will be destroyed before this function exits.
    template<typename... Args>
    constexpr auto emplace(Args&&... args) -> pair<iterator, bool>
    {
        return data.emplace(std::forward<Args>(args)...);
    }

    /// Removes an element from the container
    ///
    /// # Requires
    /// pos is an iterator to a valid element (i.e. not the end iterator).
    ///
    /// # Return value
    /// Iterator to the following element, or end() if there is none.
    ///
    /// # Notes
    /// Invalidates all iterators, pointers or references to the erased element.
    constexpr auto erase(const_iterator pos) -> iterator { return data.erase(pos); }

    /// Removes al element in the specified range from the container
    ///
    /// # Requires
    /// first and last are a valid range.
    ///
    /// # Return value
    /// Iterator to the following element, or end() if there is none.
    ///
    /// # Notes
    /// Invalidates all iterators, pointers or references to the erased elements.
    constexpr auto erase(const_iterator first, const_iterator last) -> iterator
    {
        for (auto iter = first; iter != last; iter = data.erase(iter))
        {
        }
        return last;
    }

    /// Removes an element from the container
    ///
    /// # Return value
    /// 1 if an element with this key was removed from the container, 0 otherwise.
    ///
    /// # Notes
    /// Invalidates all iterators, pointers or references to the erased element.
    constexpr auto erase(Key const& key) -> size_type { return erase<Key const&>(key); }

    /// Removes an element from the container
    ///
    /// # Return value
    /// 1 if an element with this key was removed from the container, 0 otherwise.
    ///
    /// # Notes
    /// Invalidates all iterators, pointers or references to the erased element.
    template<typename K>
        requires(!(std::convertible_to<K, iterator> || std::convertible_to<K, const_iterator>))
    constexpr auto erase(K&& x) -> size_type
    {
        auto const iter = data.find(std::forward<K>(x));
        if (iter == end())
            return 0;
        data.erase(iter);
        return 1;
    }

    /// Returns 1 if this key is in the container, 0 otherwise
    constexpr auto count(Key const& key) const noexcept -> size_type { return count<Key const&>(key); }

    /// Returns 1 if an element that compares equal to x is in the container, 0 otherwise
    template<typename K>
    constexpr auto count(K&& x) const noexcept -> size_type
    {
        if (data.find(std::forward<K>(x)) == end())
            return 0;
        return 1;
    }

    /// Returns an iterator to the element with key, or end() if none.
    constexpr auto find(Key const& key) -> iterator { return find<Key const&>(key); }
    /// Returns an iterator to the element with key, or end() if none.
    constexpr auto find(Key const& key) const -> const_iterator { return find<Key const&>(key); }

    /// Returns an iterator to the element comparing equal with key, or end() if none.
    template<typename K>
    constexpr auto find(K&& key) -> iterator
    {
        return data.find(std::forward<K>(key));
    }
    /// Returns an iterator to the element comparing equal with key, or end() if none.
    template<typename K>
    constexpr auto find(K&& key) const -> const_iterator
    {
        return data.find(std::forward<K>(key));
    }

    /// Returns true if the key was found in the container, otherwise false.
    constexpr auto contains(Key const& key) const noexcept -> bool { return contains<Key const&>(key); }
    /// Returns true if a key equivalent to x was found in the container, otherwise false.
    template<typename K>
    constexpr auto contains(K&& x) const noexcept -> bool
    {
        return count(std::forward<K>(x)) != 0;
    }

    /// Returns the range of elements equal to key
    constexpr auto equal_range(Key const& key) -> pair<iterator, iterator>
    {
        return std::as_const(*this).equal_range(key);
    }
    /// Returns the range of elements equal to key
    constexpr auto equal_range(Key const& key) const -> pair<const_iterator, const_iterator>
    {
        return equal_range<Key const&>(key);
    }
    /// Returns the range of elements equal to key
    template<typename K>
    constexpr auto equal_range(K&& key) -> pair<iterator, iterator>
    {
        return std::as_const(*this).equal_range(std::forward<K>(key));
    }
    /// Returns the range of elements equal to key
    template<typename K>
    constexpr auto equal_range(K&& key) const -> pair<const_iterator, const_iterator>
    {
        auto const iter = find(std::forward<K>(key));
        if (iter == end())
            return {iter, iter};
        return {iter, ++iter};
    }

    /// Returns the average number of elements per bucket
    [[nodiscard]] constexpr auto load_factor() const noexcept -> float { return data.load_factor(); }

    /// Returns the function used to hash the keys
    constexpr auto hash_function() const -> hasher { return data.hash_fn; }
    /// Returns the function used to compare keys for equality
    constexpr auto key_eq() const -> key_equal { return data.equal_fn; }

    /// Returns whether the contents of two sets are identical
    friend constexpr auto operator==(static_unordered_set const& lhs, static_unordered_set const& rhs) -> bool
    {
        return lhs.data == rhs.data;
    }

    // -- internal API

    detail::static_hash_table<Key, Capacity, Hash, Equal> data{};
};

/// Erases all elements satisfying a predicate.
///
/// # Return value
/// Number of erased elements
///
/// # Complexity
/// Linear in c.size()
template<class Key, std::size_t Capacity, class Hash, class KeyEqual, class Pred>
constexpr auto erase_if(static_unordered_set<Key, Capacity, Hash, KeyEqual>& c, Pred pred) ->
    typename static_unordered_set<Key, Capacity, Hash, KeyEqual>::size_type
{
    auto const s = c.size();
    for (auto iter = c.begin(); iter != c.end();)
    {
        if (pred(*iter))
            iter = c.erase(iter);
        else
            ++iter;
    }
    return s - c.size();
}
} // namespace structural

#endif // STRUCTURAL_STATIC_UNORDERED_SET_HPP
