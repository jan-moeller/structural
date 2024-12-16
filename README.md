# structural

[![GCC](https://github.com/jan-moeller/structural/actions/workflows/gcc.yml/badge.svg)](https://github.com/jan-moeller/structural/actions/workflows/gcc.yml)
[![CLANG](https://github.com/jan-moeller/structural/actions/workflows/clang.yml/badge.svg)](https://github.com/jan-moeller/structural/actions/workflows/clang.yml)

This repository contains [structural](https://en.cppreference.com/w/cpp/language/template_parameters) types that can be
used as NTTP or `constexpr` variables.

- inplace_vector\<T, Capacity>
- inplace_string\<Capacity>
- inplace_set\<T, Capacity, Compare>
- inplace_map\<Key, Value, Capacity, Compare>
- inplace_unordered_set\<T, Capacity, Hash, Equals>
- inplace_unordered_map\<Key, T, Capacity, Hash, Equals>
- pair\<T, U>
- tuple\<Ts...>
- bitset\<Size>

All containers are structural as long as they are templated on structural types.
