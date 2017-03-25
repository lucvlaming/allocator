#pragma once

#include <cstdint>
#include <cstring>
#include <utility>

namespace dod
{
namespace detail
{
template <auto... I>
constexpr auto fold_sum(std::index_sequence<I...>) noexcept
{
    return std::integral_constant<std::size_t, (I + ...)>();
}

constexpr std::size_t sum_n_parts(std::index_sequence<>, std::size_t)  noexcept
{
    return 0;
}

template <auto I, auto... J>
constexpr std::size_t sum_n_parts(std::index_sequence<I, J...>, std::size_t Count) noexcept
{
    if (Count == 0)
        return 0;
    return I + sum_n_parts(std::index_sequence<J...>(), Count-1);
}


template <auto Count, auto Index, typename Tuple>
void           initialize_struct(const Tuple&, std::uint8_t *, std::index_sequence<>) noexcept
{
}

template <auto Count, auto Index, typename Tuple, auto I, auto... J>
void initialize_struct(const Tuple& tuple,
                       std::uint8_t *      array,
                       std::index_sequence<I, J...>)
{
    const auto value = std::get<Index>(tuple);
    for (std::size_t i = 0; i < Count; ++i, array += I)
        std::memcpy(array, &value, I);

    initialize_struct<Count, Index+1>(tuple, array, std::index_sequence<J...>());
}

template <auto Count, auto FieldIndex, typename Tuple>
void gather_struct(std::size_t, const Tuple&, const std::uint8_t *, std::index_sequence<>)
{
}

template <auto Count, auto FieldIndex, typename Tuple, auto I, auto... J>
void gather_struct(std::size_t arrayIndex,
                   const Tuple& tuple,
                   const std::uint8_t *array,
                   std::index_sequence<I, J...>)
{
    auto& value = std::get<FieldIndex>(tuple);
    std::memcpy(&value, array + arrayIndex * I, I);
    gather_struct<Count, FieldIndex+1>(arrayIndex, tuple, array + Count * I, std::index_sequence<J...>());
}

template <auto Count, auto FieldIndex, typename Tuple>
void scatter_struct(std::size_t, const Tuple&, std::uint8_t *, std::index_sequence<>)
{
}

template <auto Count, auto FieldIndex, typename Tuple, auto I, auto... J>
void scatter_struct(std::size_t         index,
                    const Tuple& tuple,
                    std::uint8_t *      array,
                    std::index_sequence<I, J...>)
{
    const auto value = std::get<FieldIndex>(tuple);
    std::memcpy(array + index * I, &value, I);
    scatter_struct<Count, FieldIndex+1>(index, tuple, array + Count * I, std::index_sequence<J...>());
}
}
}
