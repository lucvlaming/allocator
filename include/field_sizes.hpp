#include <array>
#include <cstddef>
#include <utility>

#include "fields_count.hpp"
#include <boost/preprocessor/comma_if.hpp>
#include <boost/preprocessor/repeat_from_to.hpp>

namespace dod
{
namespace detail
{

    template <class T>
    constexpr auto field_sizes_impl(...) noexcept
    {
        return 0;
    }

#define PACKED_FIELD(z, n, data) BOOST_PP_COMMA_IF(n) a##n
#define FIELD_SIZE(z, n, data) BOOST_PP_COMMA_IF(n) sizeof(a##n)

#define field_sizes(z, N, data)                                                                    \
    template <class T>                                                                             \
    constexpr auto field_sizes_impl(const T &instance,                                             \
                                    std::integral_constant<std::size_t, N>) noexcept               \
    {                                                                                              \
        auto[BOOST_PP_REPEAT_FROM_TO_##z(0, N, PACKED_FIELD, _)] = instance;                       \
        return std::integer_sequence<size_t, BOOST_PP_REPEAT_FROM_TO_##z(0, N, FIELD_SIZE, _)>();  \
    }

    BOOST_PP_REPEAT_FROM_TO(1, DOD_MAX_FIELDS, field_sizes, _)
}

template <class T>
constexpr auto get_field_sizes(const T &instance) noexcept
{
    constexpr std::size_t fields_count = detail::fields_count<T>();
    return detail::field_sizes_impl<T>(instance,
                                       std::integral_constant<std::size_t, fields_count>());
}
}
