#pragma once

#include <tuple>

#include "field_count.hpp"

namespace dod
{
namespace detail
{

template<class T>
constexpr auto struct_as_tuple(const T& instance, std::integral_constant<std::size_t, 1>) noexcept
{
    auto& [m1] = const_cast<T&>(instance);
    return std::forward_as_tuple(m1);
}
template<class T>
constexpr auto struct_as_tuple(const T& instance, std::integral_constant<std::size_t, 2>) noexcept
{
    auto& [m1, m2] = const_cast<T&>(instance);
    return std::forward_as_tuple(m1, m2);
}
template<class T>
constexpr auto struct_as_tuple(const T& instance, std::integral_constant<std::size_t, 3>) noexcept
{
    auto& [m1, m2, m3] = const_cast<T&>(instance);
    return std::forward_as_tuple(m1, m2, m3);
}
template<class T>
constexpr auto struct_as_tuple(const T& instance, std::integral_constant<std::size_t, 4>) noexcept
{
    auto& [m1, m2, m3, m4] = const_cast<T&>(instance);
    return std::forward_as_tuple(m1, m2, m3, m4);
}
template<class T>
constexpr auto struct_as_tuple(const T& instance, std::integral_constant<std::size_t, 5>) noexcept
{
    auto& [m1, m2, m3, m4, m5] = const_cast<T&>(instance);
    return std::forward_as_tuple(m1, m2, m3, m4, m5);
}
template<class T>
constexpr auto struct_as_tuple(const T& instance, std::integral_constant<std::size_t, 6>) noexcept
{
    auto& [m1, m2, m3, m4, m5, m6] = const_cast<T&>(instance);
    return std::forward_as_tuple(m1, m2, m3, m4, m5, m6);
}
template<class T>
constexpr auto struct_as_tuple(const T& instance, std::integral_constant<std::size_t, 7>) noexcept
{
    auto& [m1, m2, m3, m4, m5, m6, m7] = const_cast<T&>(instance);
    return std::forward_as_tuple(m1, m2, m3, m4, m5, m6, m7);
}
template<class T>
constexpr auto struct_as_tuple(const T& instance, std::integral_constant<std::size_t, 8>) noexcept
{
    auto& [m1, m2, m3, m4, m5, m6, m7, m8] = const_cast<T&>(instance);
    return std::forward_as_tuple(m1, m2, m3, m4, m5, m6, m7, m8);
}
template<class T>
constexpr auto struct_as_tuple(const T& instance, std::integral_constant<std::size_t, 9>) noexcept
{
    auto& [m1, m2, m3, m4, m5, m6, m7, m8, m9] = const_cast<T&>(instance);
    return std::forward_as_tuple(m1, m2, m3, m4, m5, m6, m7, m8, m9);
}
template<class T>
constexpr auto struct_as_tuple(const T& instance, std::integral_constant<std::size_t, 10>) noexcept
{
    auto& [m1, m2, m3, m4, m5, m6, m7, m8, m9, m10] = const_cast<T&>(instance);
    return std::forward_as_tuple(m1, m2, m3, m4, m5, m6, m7, m8, m9, m10);
}
template<class T>
constexpr auto struct_as_tuple(const T& instance, std::integral_constant<std::size_t, 11>) noexcept
{
    auto& [m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11] = const_cast<T&>(instance);
    return std::forward_as_tuple(m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11);
}
template<class T>
constexpr auto struct_as_tuple(const T& instance, std::integral_constant<std::size_t, 12>) noexcept
{
    auto& [m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12] = const_cast<T&>(instance);
    return std::forward_as_tuple(m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12);
}
template<class T>
constexpr auto struct_as_tuple(const T& instance, std::integral_constant<std::size_t, 13>) noexcept
{
    auto& [m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13] = const_cast<T&>(instance);
    return std::forward_as_tuple(m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13);
}
template<class T>
constexpr auto struct_as_tuple(const T& instance, std::integral_constant<std::size_t, 14>) noexcept
{
    auto& [m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14] = const_cast<T&>(instance);
    return std::forward_as_tuple(m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14);
}
template<class T>
constexpr auto struct_as_tuple(const T& instance, std::integral_constant<std::size_t, 15>) noexcept
{
    auto& [m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15] = const_cast<T&>(instance);
    return std::forward_as_tuple(m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15);
}
template<class T>
constexpr auto struct_as_tuple(const T& instance, std::integral_constant<std::size_t, 16>) noexcept
{
    auto& [m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16] = const_cast<T&>(instance);
    return std::forward_as_tuple(m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16);
}
template<class T>
constexpr auto struct_as_tuple(const T& instance, std::integral_constant<std::size_t, 17>) noexcept
{
    auto& [m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17] = const_cast<T&>(instance);
    return std::forward_as_tuple(m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17);
}
template<class T>
constexpr auto struct_as_tuple(const T& instance, std::integral_constant<std::size_t, 18>) noexcept
{
    auto& [m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17, m18] = const_cast<T&>(instance);
    return std::forward_as_tuple(m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17, m18);
}
template<class T>
constexpr auto struct_as_tuple(const T& instance, std::integral_constant<std::size_t, 19>) noexcept
{
    auto& [m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17, m18, m19] = const_cast<T&>(instance);
    return std::forward_as_tuple(m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17, m18, m19);
}
template<class T>
constexpr auto struct_as_tuple(const T& instance, std::integral_constant<std::size_t, 20>) noexcept
{
    auto& [m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17, m18, m19, m20] = const_cast<T&>(instance);
    return std::forward_as_tuple(m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17, m18, m19, m20);
}

template <class T>
constexpr auto struct_as_tuple(const T &instance) noexcept
{
    return struct_as_tuple(instance, std::integral_constant<std::size_t, field_count<T>()>());
}

}
}
