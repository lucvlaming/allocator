#pragma once

#include <limits>

#include "field_count.hpp"

namespace dod
{
namespace detail
{

template <size_t Index, class Field>
constexpr std::size_t field_offset(const Field*) noexcept
{
    return std::numeric_limits<std::size_t>::max();
}

template <size_t Index, class Field, class Field2, class... Args>
constexpr std::size_t field_offset(const Field* offset, const Field2* offset2, Args... offsets)  noexcept
{
    if constexpr(std::is_same<Field, Field2>::value)
        if (offset == offset2)
            return Index;
    return field_offset<Index+1>(offset, offsets...);
}

template<class T, class U>
constexpr auto field_index(const T& instance, const U* field, std::integral_constant<std::size_t, 1>) noexcept
{
    auto& [a1] = const_cast<T&>(instance);
    return field_offset<0>(field, &a1);
}
template<class T, class U>
constexpr auto field_index(const T& instance, const U* field, std::integral_constant<std::size_t, 2>) noexcept
{
    auto& [a1, a2] = const_cast<T&>(instance);
    return field_offset<0>(field, &a1, &a2);
}
template<class T, class U>
constexpr auto field_index(const T& instance, const U* field, std::integral_constant<std::size_t, 3>) noexcept
{
    auto& [a1, a2, a3] = const_cast<T&>(instance);
    return field_offset<0>(field, &a1, &a2, &a3);
}
template<class T, class U>
constexpr auto field_index(const T& instance, const U* field, std::integral_constant<std::size_t, 4>) noexcept
{
    auto& [a1, a2, a3, a4] = const_cast<T&>(instance);
    return field_offset<0>(field, &a1, &a2, &a3, &a4);
}
template<class T, class U>
constexpr auto field_index(const T& instance, const U* field, std::integral_constant<std::size_t, 5>) noexcept
{
    auto& [a1, a2, a3, a4, a5] = const_cast<T&>(instance);
    return field_offset<0>(field, &a1, &a2, &a3, &a4, &a5);
}
template<class T, class U>
constexpr auto field_index(const T& instance, const U* field, std::integral_constant<std::size_t, 6>) noexcept
{
    auto& [a1, a2, a3, a4, a5, a6] = const_cast<T&>(instance);
    return field_offset<0>(field, &a1, &a2, &a3, &a4, &a5, &a6);
}
template<class T, class U>
constexpr auto field_index(const T& instance, const U* field, std::integral_constant<std::size_t, 7>) noexcept
{
    auto& [a1, a2, a3, a4, a5, a6, a7] = const_cast<T&>(instance);
    return field_offset<0>(field, &a1, &a2, &a3, &a4, &a5, &a6, &a7);
}
template<class T, class U>
constexpr auto field_index(const T& instance, const U* field, std::integral_constant<std::size_t, 8>) noexcept
{
    auto& [a1, a2, a3, a4, a5, a6, a7, a8] = const_cast<T&>(instance);
    return field_offset<0>(field, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8);
}
template<class T, class U>
constexpr auto field_index(const T& instance, const U* field, std::integral_constant<std::size_t, 9>) noexcept
{
    auto& [a1, a2, a3, a4, a5, a6, a7, a8, a9] = const_cast<T&>(instance);
    return field_offset<0>(field, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8, &a9);
}

template <class T, class U>
constexpr auto field_index(const T &instance, const U* field) noexcept
{
    return field_index(instance, field, std::integral_constant<std::size_t, field_count<T>()>());
}

}
}
