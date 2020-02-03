# trait_check

A few utility functions designed to facilitate the checking of types against type traits.

**Before:**
```c++
template <typename... Ts> constexpr auto const all_or_none_integral_v = false;
template <typename... Ts>
    requires (std::is_integral<std::remove_reference_t<Ts>>::value and ...)
          or (std::negation<std::is_integral<std::remove_reference_t<Ts>>>::value and ...)
constexpr auto const all_or_none_integral_v<Ts...> = true;
```
**After:**
```c++
template <typename... Ts> constexpr auto const all_or_none_integral_v = false;
template <typename... Ts>
    requires check_f<noneall, std::is_integral, std::remove_reference_t, Ts...>()
constexpr auto const all_or_none_integral_v<Ts...> = true;
```
<br>**More exemples:**

*All same types (regardless of cvref)*
```c++
template <typename... Ts>
    requires check_f<all, std::is_same, PackRead::contiguous, std::remove_cvref_t, Ts...>()
using TestT = std::true_type;
```
*Any adjacent same types with same qualifiers*
```c++
template <typename... Ts>
    requires check_idt_f<any, std::is_same, PackRead::linear, Ts...>()
using TestT = std::true_type;
```
*None same as first type (regardless of qualifiers)*
```c++
template <typename... Ts>
    requires check_unq_f<none, std::is_same, PackRead::pivot_first, Ts...>()
using TestT = std::true_type;
```
