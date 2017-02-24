#include <cstddef>
#include <type_traits>
#include <utility>

namespace dod
{
struct any_type
{
    std::size_t ignore;
    template <class T>
    constexpr operator T() const noexcept;
};

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-field-initializers"
#endif

template <class T, auto... I>
constexpr auto is_constructable(std::index_sequence<I...>) noexcept
        -> decltype(T{any_type{I}...}, std::true_type{})
{
    return {};
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

template <class T>
constexpr auto is_constructable(...) noexcept -> std::false_type
{
    return {};
}

template <class T, std::size_t Min = 0, std::size_t Max = 100>
constexpr std::size_t fields_count()
{
    if
        constexpr(Min + 1 >= Max) return Min;
    constexpr auto Mid = (Max + Min) / 2;
    if
        constexpr(is_constructable<T>(std::make_index_sequence<Mid>()))
        {
            return fields_count<T, Mid, Max>();
        }
    else
    {
        return fields_count<T, 0, Mid>();
    }
}
}
