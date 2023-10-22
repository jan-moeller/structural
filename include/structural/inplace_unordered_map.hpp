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

#ifndef STRUCTURAL_INPLACE_UNORDERED_MAP_HPP
#define STRUCTURAL_INPLACE_UNORDERED_MAP_HPP

#include "structural/detail/inplace_hash_table.hpp"
#include "structural/detail/inplace_unordered_map_details.hpp"

namespace structural
{
/// An associative container with average constant-time complexity for search, insertion and removal.
///
/// # Notes
/// Performance degrades towards linear if size() is close to Capacity. It is therefore advisable to chose a Capacity at
/// least twice of the expected number of contained elements.
template<typename Key, typename T, std::size_t Capacity, typename Hash = hash<Key>, typename Equal = std::equal_to<Key>>
struct inplace_unordered_map
{
    using key_type        = Key;
    using mapped_type     = T;
    using value_type      = detail::inplace_unordered_map::hashmap_t<Key, T, Capacity, Hash, Equal>::value_type;
    using size_type       = detail::inplace_unordered_map::hashmap_t<Key, T, Capacity, Hash, Equal>::size_type;
    using difference_type = detail::inplace_unordered_map::hashmap_t<Key, T, Capacity, Hash, Equal>::difference_type;
    using hasher          = Hash;
    using key_equal       = Equal;
    using reference       = detail::inplace_unordered_map::hashmap_t<Key, T, Capacity, Hash, Equal>::reference;
    using const_reference = detail::inplace_unordered_map::hashmap_t<Key, T, Capacity, Hash, Equal>::const_reference;
    using pointer         = detail::inplace_unordered_map::hashmap_t<Key, T, Capacity, Hash, Equal>::pointer;
    using const_pointer   = detail::inplace_unordered_map::hashmap_t<Key, T, Capacity, Hash, Equal>::const_pointer;
    using iterator        = detail::inplace_unordered_map::hashmap_t<Key, T, Capacity, Hash, Equal>::iterator;
    using const_iterator  = detail::inplace_unordered_map::hashmap_t<Key, T, Capacity, Hash, Equal>::const_iterator;

    /// Constructs an empty map
    constexpr inplace_unordered_map()
        : inplace_unordered_map(Hash())
    {
    }

    /// Constructs an empty map with a certain hash function and equality function
    constexpr inplace_unordered_map(Hash const& hash, key_equal const& equal = key_equal())
        : data(detail::inplace_unordered_map::hasher_t<Key, T, Hash>{hash},
               detail::inplace_unordered_map::equals_t<Key, T, Equal>{equal})
    {
    }

    /// Constructs a map containing the elements from a range
    ///
    /// # Requires
    /// distance(first, last) < Capacity
    template<std::input_iterator First, std::sentinel_for<First> Last>
    constexpr inplace_unordered_map(First            first,
                                   Last             last,
                                   Hash const&      hash  = Hash(),
                                   key_equal const& equal = key_equal())
        : inplace_unordered_map(hash, equal)
    {
        insert(first, last);
    }

    /// Constructs a map containing the elements from an initializer list
    ///
    /// # Requires
    /// distance(first, last) < Capacity
    constexpr inplace_unordered_map(std::initializer_list<value_type> init,
                                   Hash const&                       hash  = Hash(),
                                   key_equal const&                  equal = key_equal())
        : inplace_unordered_map(init.begin(), init.end(), hash, equal)
    {
    }

    /// Returns an iterator to the beginning
    constexpr auto begin() noexcept -> iterator { return data.begin(); }
    /// Returns an iterator to the beginning
    constexpr auto begin() const noexcept -> const_iterator { return data.begin(); }
    /// Returns an iterator to the beginning
    constexpr auto cbegin() const noexcept -> const_iterator { return data.begin(); }
    /// Returns an iterator to the end
    constexpr auto end() noexcept -> iterator { return data.end(); }
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

    /// Inserts a value with the specified key, if no equivalent key exists. Otherwise, replaces the value of that key.
    template<typename M>
    constexpr auto insert_or_assign(Key const& k, M&& value) -> pair<iterator, bool>
    {
        static_assert(std::is_assignable_v<mapped_type&, M&&>);
        auto r = insert(value_type(k, std::forward<M>(value)));
        if (!r.second)
            r.first->second = std::forward<M>(value);
        return r;
    }

    /// Inserts a value with the specified key, if no equivalent key exists. Otherwise, replaces the value of that key.
    template<typename M>
    constexpr auto insert_or_assign(Key&& k, M&& value) -> pair<iterator, bool>
    {
        static_assert(std::is_assignable_v<mapped_type&, M&&>);
        auto r = insert(value_type(std::move(k), std::forward<M>(value)));
        if (!r.second)
            r.first->second = std::forward<M>(value);
        return r;
    }

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
    constexpr auto try_emplace(Key const& k, Args&&... args) -> pair<iterator, bool>
    {
        return data.emplace(std::piecewise_construct,
                            forward_as_tuple(k),
                            forward_as_tuple(std::forward<Args>(args)...));
    }

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
    constexpr auto try_emplace(Key&& k, Args&&... args) -> pair<iterator, bool>
    {
        return data.emplace(std::piecewise_construct,
                            forward_as_tuple(std::move(k)),
                            forward_as_tuple(std::forward<Args>(args)...));
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
        return iterator{&data, last.idx, last.node_idx};
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

    /// Returns a reference to the mapped type. If no such element exists, throws std::out_of_range.
    constexpr auto at(Key const& key) -> T& { return const_cast<T&>(std::as_const(*this).at(key)); }
    /// Returns a reference to the mapped type. If no such element exists, throws std::out_of_range.
    constexpr auto at(Key const& key) const -> T const&
    {
        auto iter = find(key);
        if (iter == end())
            throw std::out_of_range("inplace_unordered_map::at");
        return iter->second;
    }

    /// Returns a reference to the mapped type, inserting a default-constructed one if necessary
    constexpr auto operator[](Key const& key) -> T& { return this->try_emplace(key).first->second; }
    /// Returns a reference to the mapped type, inserting a default-constructed one if necessary
    constexpr auto operator[](Key&& key) -> T& { return this->try_emplace(std::move(key)).first->second; }

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

    /// Returns whether the contents of two maps are identical
    friend constexpr auto operator==(inplace_unordered_map const& lhs, inplace_unordered_map const& rhs) -> bool
    {
        return lhs.data == rhs.data;
    }

    // -- internal API

    detail::inplace_unordered_map::hashmap_t<Key, T, Capacity, Hash, Equal> data{};
};

/// Erases all elements satisfying a predicate.
///
/// # Return value
/// Number of erased elements
///
/// # Complexity
/// Linear in c.size()
template<class Key, class T, std::size_t Capacity, class Hash, class KeyEqual, class Pred>
constexpr auto erase_if(inplace_unordered_map<Key, T, Capacity, Hash, KeyEqual>& c, Pred pred) ->
    typename inplace_unordered_map<Key, T, Capacity, Hash, KeyEqual>::size_type
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

#endif // STRUCTURAL_INPLACE_UNORDERED_MAP_HPP
