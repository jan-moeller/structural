//
// MIT License
//
// Copyright (c) 2023 Jan Möller
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

#ifndef STRUCTURAL_CONSTEXPR_PARAM_HPP
#define STRUCTURAL_CONSTEXPR_PARAM_HPP

#include <concepts>
#include <type_traits>

#define STRUCTURAL_HAS_STATIC_CALL_OPERATOR (__cpp_static_call_operator >= 202207L)
#define STRUCTURAL_HAS_STATIC_SUBSCRIPT_OPERATOR (__cpp_multidimensional_subscript >= 202111L)
#define STRUCTURAL_HAS_MULTIDIM_SUBSCRIPT_OPERATOR (__cpp_multidimensional_subscript >= 202110L)

#if STRUCTURAL_HAS_STATIC_CALL_OPERATOR
#define STRUCTURAL_STATIC_CALL static
#define STRUCTURAL_STATIC_CALL_CONST
#else
#define STRUCTURAL_STATIC_CALL
#define STRUCTURAL_STATIC_CALL_CONST const
#endif

#if STRUCTURAL_HAS_STATIC_SUBSCRIPT_OPERATOR
#define STRUCTURAL_STATIC_SUBSCRIPT static
#define STRUCTURAL_STATIC_SUBSCRIPT_CONST
#else
#define STRUCTURAL_STATIC_SUBSCRIPT
#define STRUCTURAL_STATIC_SUBSCRIPT_CONST const
#endif

namespace structural
{
template<typename T, T Value>
struct structural_constant;

namespace detail
{
template<typename T>
struct is_structural_constant : std::false_type
{
};
template<typename U, U Value>
struct is_structural_constant<structural_constant<U, Value>> : std::true_type
{
};
template<typename T>
inline constexpr bool is_structural_constant_v = is_structural_constant<T>::value;
} // namespace detail

/// If U is void, this concept returns true if T is an instantiation of structural_constant, and false otherwise.
/// If U is not void, it additionally checks if the structural_constant's value_type is convertible to U, and only
/// returns true if it is.
/// The primary usecase is functions that take structural_constants as arguments, i.e.:
///   void f(compiletime auto arg);      // Accepts any instantiation of structural_constant.
///   void g(compiletime<int> auto arg); // Accepts any instantiation of structural_constant that is convertible to int.
template<typename T, typename U = void>
concept compiletime = detail::is_structural_constant_v<T>
                      && (std::same_as<U, void> || std::convertible_to<typename T::value_type, U>);

/// A variable template to simplify creating structural_constants. E.g.:
///   f(constant<42>); // f is called with structural_constant<int, 42>
template<auto Value>
inline constexpr structural_constant<decltype(Value), Value> constant = {};

/// A type holding a single structural compile-time value.
///
/// This type is analogous to std::integral_constant, but for any structural type, and it also provides most operators.
/// Unlike integral_constant, structural_constant does its best not to decay to a runtime value. This is why most
/// operators are available if the wrapped type provides them:
///   Unary: +, -, !, ~, *
///   Binary: +, -, *, /, %, ==, !=, <, <=, >, >=, &&, ||, &, |, ^, <<, >>, ->*
///   N-ary: (), []
/// Notably, -> is never provided. This is because the only sensible way to define it would result in a runtime value,
/// which structural_constant is avoiding. If this is the desired behavior, call s.value-> instead.
/// All operators return new structural_constants wrapping the result of the underlying operator. To facilitate this,
/// only structural_constants are accepted as arguments.
///
/// The primary purpose of this type is to facilitate functions with "constexpr" parameters, without having to directly
/// use NTTPs. For example:
///   template<int I> void f();        // Has to be called as f<42>() and cannot easily be overloaded.
///   void g(compiletime<int> auto i); // Can be called as f(constant<42>) and allows overloading with runtime types.
template<typename T, T Value>
struct structural_constant
{
    using value_type            = T;
    static constexpr auto value = Value;

    constexpr operator T() const { return value; } // NOLINT(google-explicit-constructor)

    // ----- unary operators -----

#define STRUCTURAL_GENERATE_UNARY_PREFIX_OP(OP)                                                                        \
    friend constexpr auto operator OP(structural_constant<T, Value> const& /*unused*/)                                 \
        requires requires { OP Value; }                                                                                \
    {                                                                                                                  \
        return constant<OP Value>;                                                                                     \
    }

    STRUCTURAL_GENERATE_UNARY_PREFIX_OP(+)
    STRUCTURAL_GENERATE_UNARY_PREFIX_OP(-)
    STRUCTURAL_GENERATE_UNARY_PREFIX_OP(!)
    STRUCTURAL_GENERATE_UNARY_PREFIX_OP(~)
    STRUCTURAL_GENERATE_UNARY_PREFIX_OP(*)

#undef STRUCTURAL_GENERATE_UNARY_PREFIX_OP

    // ----- binary operators -----

#define STRUCTURAL_GENERATE_BINARY_OP(OP)                                                                              \
    template<typename U, U Rhs>                                                                                        \
    friend constexpr auto operator OP(structural_constant<T, Value> const& /*unused*/,                                 \
                                      structural_constant<U, Rhs> const& /*unused*/)                                   \
        requires requires { Value OP Rhs; }                                                                            \
    {                                                                                                                  \
        return constant<(Value OP Rhs)>;                                                                               \
    }

    STRUCTURAL_GENERATE_BINARY_OP(+)
    STRUCTURAL_GENERATE_BINARY_OP(-)
    STRUCTURAL_GENERATE_BINARY_OP(*)
    STRUCTURAL_GENERATE_BINARY_OP(/)
    STRUCTURAL_GENERATE_BINARY_OP(%)
    STRUCTURAL_GENERATE_BINARY_OP(==)
    STRUCTURAL_GENERATE_BINARY_OP(!=)
    STRUCTURAL_GENERATE_BINARY_OP(<)
    STRUCTURAL_GENERATE_BINARY_OP(<=)
    STRUCTURAL_GENERATE_BINARY_OP(>)
    STRUCTURAL_GENERATE_BINARY_OP(>=)
    STRUCTURAL_GENERATE_BINARY_OP(&&)
    STRUCTURAL_GENERATE_BINARY_OP(||)
    STRUCTURAL_GENERATE_BINARY_OP(&)
    STRUCTURAL_GENERATE_BINARY_OP(|)
    STRUCTURAL_GENERATE_BINARY_OP(^)
    STRUCTURAL_GENERATE_BINARY_OP(<<)
    STRUCTURAL_GENERATE_BINARY_OP(>>)
    STRUCTURAL_GENERATE_BINARY_OP(->*)

#undef STRUCTURAL_GENERATE_BINARY_OP

    template<compiletime... Args>
    constexpr STRUCTURAL_STATIC_CALL auto
    operator()(Args const&... /*unused*/) STRUCTURAL_STATIC_CALL_CONST->decltype(auto)
        requires requires { value(Args::value...); }
    {
        return constant<value(Args::value...)>;
    }

#if STRUCTURAL_HAS_MULTIDIM_SUBSCRIPT_OPERATOR
    template<compiletime... Args>
    constexpr STRUCTURAL_STATIC_SUBSCRIPT auto
    operator[](Args const&... /*unused*/) STRUCTURAL_STATIC_SUBSCRIPT_CONST->decltype(auto)
        requires requires { value[Args::value...]; }
    {
        return constant<value[Args::value...]>;
    }
#else
    template<compiletime Arg>
    constexpr STRUCTURAL_STATIC_SUBSCRIPT auto operator[](Arg const& /*unused*/) const -> decltype(auto)
        requires requires { value[Arg::value]; }
    {
        return constant<value[Arg::value]>;
    }
#endif
};
} // namespace structural

#undef STRUCTURAL_STATIC_SUBSCRIPT
#undef STRUCTURAL_STATIC_SUBSCRIPT_CONST
#undef STRUCTURAL_STATIC_CALL
#undef STRUCTURAL_STATIC_CALL_CONST
#undef STRUCTURAL_HAS_STATIC_CALL_OPERATOR
#undef STRUCTURAL_HAS_STATIC_SUBSCRIPT_OPERATOR
#undef STRUCTURAL_HAS_MULTIDIM_SUBSCRIPT_OPERATOR

#endif // STRUCTURAL_CONSTEXPR_PARAM_HPP
