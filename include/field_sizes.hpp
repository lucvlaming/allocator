#include <array>
#include <cstddef>
#include <utility>

#include "fields_count.hpp"

namespace dod
{
template <class T>
constexpr auto field_sizes_impl(...) noexcept
{
    return 0;
}

template <class T>
constexpr auto field_sizes_impl(const T &instance, std::integral_constant<std::size_t, 4>) noexcept
{
    auto[a, b, c, d] = instance;
    return std::integer_sequence<size_t, sizeof(a), sizeof(b), sizeof(c), sizeof(d)>();
}

template <class T>
constexpr auto field_sizes(const T &instance) noexcept
{
    constexpr auto field_count = fields_count<T>();
    return field_sizes_impl<T>(instance, std::integral_constant<std::size_t, field_count>());
}
}
