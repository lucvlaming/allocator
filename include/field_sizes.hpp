#pragma once

#include <array>
#include <cstddef>
#include <utility>

#include "field_count.hpp"
#include <boost/preprocessor/comma_if.hpp>
#include <boost/preprocessor/repeat_from_to.hpp>

namespace dod
{
namespace detail
{

    template <class T>
    constexpr auto field_sizes(...) noexcept
    {
        return 0;
    }

#define PACKED_FIELD(z, n, data) BOOST_PP_COMMA_IF(n) a##n
#define FIELD_SIZE(z, n, data) BOOST_PP_COMMA_IF(n) sizeof(a##n)

#define FIELD_SIZES(z, N, data)                                                                    \
    template <class T>                                                                             \
    constexpr auto field_sizes(const T &instance,                                             \
                                    std::integral_constant<std::size_t, N>) noexcept               \
    {                                                                                              \
        auto[BOOST_PP_REPEAT_FROM_TO_##z(0, N, PACKED_FIELD, _)] = instance;                       \
        return std::integer_sequence<size_t, BOOST_PP_REPEAT_FROM_TO_##z(0, N, FIELD_SIZE, _)>();  \
    }

    BOOST_PP_REPEAT_FROM_TO(1, DOD_MAX_FIELDS, FIELD_SIZES, _)

    template <class T>
    constexpr auto field_sizes(const T &instance) noexcept
    {
        return field_sizes(instance, std::integral_constant<std::size_t, field_count<T>()>());
    }
}
}
