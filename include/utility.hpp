#include <cstdint>
#include <cstring>
#include <utility>

namespace dod
{
template <auto... I>
constexpr auto fold_sum(std::index_sequence<I...>)
{
    return std::integral_constant<std::size_t, (I + ...)>();
}

template <typename Func>
constexpr void iterate_sequence(std::index_sequence<>, Func)
{
}

template <auto I, auto... J, typename Func>
constexpr void iterate_sequence(std::index_sequence<I, J...>, Func callback)
{
    callback(std::integral_constant<std::size_t, I>());
    iterate_sequence(std::index_sequence<J...>(), callback);
}

template <auto Size>
void initialize_struct(const std::uint8_t *, std::uint8_t *, std::index_sequence<>, std::size_t)
{
}

template <auto Count, auto I, auto... J>
void initialize_struct(const std::uint8_t *prototype,
                       std::uint8_t *      data,
                       std::index_sequence<I, J...>,
                       std::size_t offset = 0)
{
    for (std::size_t i = 0; i < Count; ++i, data += I)
        std::memcpy(data, prototype, I);

    initialize_struct<Count>(prototype + I, data, std::index_sequence<J...>(), offset + I);
}
}
