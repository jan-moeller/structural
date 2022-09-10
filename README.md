# structural

[![GCC](https://github.com/jan-moeller/structural/actions/workflows/gcc.yml/badge.svg)](https://github.com/jan-moeller/structural/actions/workflows/gcc.yml)

This repository contains [structural](https://en.cppreference.com/w/cpp/language/template_parameters) types that can be
used as NTTP or `constexpr` variables.

- static_vector\<T, Capacity>
- static_string\<Capacity>
- static_set\<T, Capacity, Compare>
- static_map\<Key, Value, Capacity, Compare>
- static_unordered_set\<T, Capacity, Hash, Equals>
- pair\<T, U>
- tuple\<Ts...>
- bitset\<Size>

All containers are structural as long as they are templated on structural types.
