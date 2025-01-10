# structural

[![GCC](https://github.com/jan-moeller/structural/actions/workflows/gcc.yml/badge.svg)](https://github.com/jan-moeller/structural/actions/workflows/gcc.yml)
[![CLANG](https://github.com/jan-moeller/structural/actions/workflows/clang.yml/badge.svg)](https://github.com/jan-moeller/structural/actions/workflows/clang.yml)

## Containers

This repository contains [structural](https://en.cppreference.com/w/cpp/language/template_parameters) types that can be used as NTTP or `constexpr` variables.

- inplace_vector\<T, Capacity>
- inplace_string\<Capacity>
- inplace_set\<T, Capacity, Compare>
- inplace_map\<Key, Value, Capacity, Compare>
- inplace_unordered_set\<T, Capacity, Hash, Equals>
- inplace_unordered_map\<Key, T, Capacity, Hash, Equals>
- pair\<T, U>
- tuple\<Ts...>
- bitset\<Size>

All containers are structural as long as they are templated on structural types. They generally
have the same API as their standard library equivalents, but all elements are allocated within
automatic storage. This means that these containers are generally pointer-stable on insertion (at
the end; inserting in the middle results in some elements being shifted, so pointers into the
container might now point to a different element), but not on move.

## Utilities

There are two macros to make non-structural types usable in contexts where structural types are
required:

### STRUCTURAL_WRAP

   This macros serializes a type and wraps the resulting array<byte, N> in a structural type.
   The wrapper API allows to "unwrap" (i.e. deserialize) and restore the original type.

   ```c++
   template<typename T, std::size_t N>
   struct wrapper
   {
       using wrapped_type = T;
       constexpr auto unwrap() const -> T;
   };
   ```
   
   Usage is for example like this:
   ```c++
   constexpr auto fun() -> std::vector<int>; // Definitely not structural
   constexpr auto something = STRUCTURAL_WRAP(fun());
   
   template<structural::wrapper WrappedValue>
   class my_class{
       static constexpr auto foo = do_something(WrappedValue.unwrap())
   };
   my_class<STRUCTURAL_WRAP(fun())> or_other{};
   ```
   
   By default, the following types are wrappable:
   - Arithmetic types
   - Enums, including std::byte
   - Specializations of std::chrono::duration and std::chrono::time_point
   - Aggregates with up to 10 members of wrappable types
   - Specializations of std::optional and std::expected of wrappable types
   - Forward ranges of wrappable types, if they aren't views, and can be constructed from a
     pair of iterators
   - Tuple-like types (including std::array, std::tuple etc.)
   - Specializations of std::unique_ptr of wrappable types
   - Specializations of std::variant of wrappable types

   To make custom types wrappable, it is permitted to specialize `structural::serializer` with
   the following API:
   ```c++
   namespace structural
   {
   struct serializer<CustomType> {
       static constexpr void do_serialize(CustomType const& value, std::output_iterator<std::byte> auto& out_iter);
       static constexpr auto do_deserialize(std::in_place_type_t<CustomType>, std::input_iterator auto& in_iter) -> CustomType;
   };
   } // namespace structural
   ```

### STRUCTURALIZE
   
   This macro converts a wrappable type into a similar structural type. For example:
   ```c++
   constexpr auto fun() -> std::vector<int>; // Definitely not structural
   constexpr auto foo = STRUCTURALIZE(fun()); // foo is std::array<int, N>
   ```
   
   By default, the following wrappable types are structuralizable:
   - Structural types (are converted to themselves)
   - Aggregates with up to 10 members of structuralizable types (converted to tuple)
   - Specializations of std::optional of structuralizable types (converted to nullopt or T)
   - Forward ranges of structuralizable types (converted to array if all elements have the same type,
     or tuple otherwise)
   - Tuple-like types of structuralizable types (converted to tuple)
   - Specializations of std::unique_ptr of structuralizable type (converted to nullptr_t or T)
   - Specializations of std::variant of structuralizable type (converted to T)

   To make custom types structuralizable, it is permitted to specialize `structural::structuralizer`
   with the following API:
   ```c++
   namespace structural
   {
   template<wrapper WrappedValue>
   struct structuralizer<CustomType, WrappedValue> {
       static consteval auto do_structuralize(); // Must return a structural type
   };
   } // namespace structural
   ```
   
## Limitations

- Some compilers don't accept the container types as constexpr constants or NTTPs if they contain
  excess capacity.
- The containers don't provide a data() member if their `value_type` is not structural. This
  is because given the implementation strategy chosen, that member would expose UB. I am not aware
  of any implementation strategy in C++20 or C++23 that doesn't expose that UB.
- Structuralization would benefit from reflection, since it could then generate structural
  aggregates rather than having to use tuple everywhere.
- Due to structural requirements, all member variables are public. Messing with them from outside
  the library is considered UB.