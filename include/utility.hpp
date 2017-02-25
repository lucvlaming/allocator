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

template <auto Count>
void           initialize_struct(const std::uint8_t *, std::uint8_t *, std::index_sequence<>)
{
}

template <auto Count, auto I, auto... J>
void initialize_struct(const std::uint8_t *element,
                       std::uint8_t *      array,
                       std::index_sequence<I, J...>)
{
    for (std::size_t i = 0; i < Count; ++i, array += I)
        std::memcpy(array, element, I);

    initialize_struct<Count>(element + I, array, std::index_sequence<J...>());
}

template <auto Size>
void gather_struct(std::size_t, std::uint8_t *, const std::uint8_t *, std::index_sequence<>)
{
}

template <auto Count, auto I, auto... J>
void gather_struct(std::size_t         index,
                   std::uint8_t *      element,
                   const std::uint8_t *array,
                   std::index_sequence<I, J...>)
{
    std::memcpy(element, array + index * I, I);
    gather_struct<Count>(index, element + I, array + Count * I, std::index_sequence<J...>());
}

template <auto Size>
void scatter_struct(std::size_t, const std::uint8_t *, std::uint8_t *, std::index_sequence<>)
{
}

template <auto Count, auto I, auto... J>
void scatter_struct(std::size_t         index,
                    const std::uint8_t *element,
                    std::uint8_t *      array,
                    std::index_sequence<I, J...>)
{
    std::memcpy(array + index * I, element, I);
    scatter_struct<Count>(index, element + I, array + Count * I, std::index_sequence<J...>());
}
}
