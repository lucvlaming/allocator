#pragma once

#include <iostream>

#include <array>
#include <cassert>
#include <type_traits>

#include "field_sizes.hpp"
#include "field_index.hpp"
#include "struct_as_tuple.hpp"
#include "utility.hpp"

namespace dod
{

template <class Type, size_t Count>
class array;

template <class Type, size_t Count>
class object_container;

template <class Type, size_t Count, class FieldType>
class field
{
public:
    FieldType *begin()
    {
        return _ptr;
    }

    const FieldType *begin() const
    {
        return _ptr;
    }

    FieldType *end()
    {
        return _ptr + Count;
    }

    const FieldType *end() const
    {
        return _ptr + Count;
    }

private:
    field(FieldType *ptr) : _ptr(ptr)
    {
    }
    FieldType *_ptr;

    friend class array<std::remove_const_t<Type>, Count>;
};

template <class Type, size_t Count>
class object
{
public:
    ~object()
    {
        if constexpr(std::is_same<Type, std::remove_const_t<Type>>::value)
            detail::scatter_struct<Count, 0>(_index, detail::struct_as_tuple(_object), _data,
                                          array<Type, Count>::field_sizes);
    }

    Type *operator->()
    {
        return &_object;
    }

    const Type *operator->() const
    {
        return &_object;
    }

    Type *operator*()
    {
        return &_object;
    }

    const Type *operator*() const
    {
        return &_object;
    }

private:
    object(uint8_t *data, std::size_t index) : _data(data), _index(index)
    {
        detail::gather_struct<Count, 0>(_index, detail::struct_as_tuple(_object), _data,
                                     array<Type, Count>::field_sizes);
    }

    Type        _object;
    uint8_t *   _data;
    size_t  _index;

    friend class array<std::remove_const_t<Type>, Count>;
    friend class object_container<std::remove_const_t<Type>, Count>;
};

template <class Type, size_t Count>
class object_container
{
public:
    auto begin()
    {
        return object_container<Type, Count>(data, 0);
    }

    auto begin() const
    {
        return object_container<const Type, Count>(data, 0);
    }

    auto end()
    {
        return object_container<Type, Count>(data, Count);
    }

    auto end() const
    {
        return object_container<const Type, Count>(data, Count);
    }

    auto operator*()
    {
        return object<Type, Count>(data, index);
    }

    auto operator*() const
    {
        return object<const Type, Count>(data, index);
    }

    auto &operator++()
    {
        ++index;
        return *this;
    }

    auto operator==(const object_container& other) const
    {
        return index == other.index && data == other.data;
    }

    auto operator!=(const object_container& other)  const
    {
        return !(operator==(other));
    }

private:
    object_container(uint8_t* d, size_t i) : data(d), index(i)
    {
    }
    uint8_t *data;
    size_t index;

    friend class array<std::remove_const_t<Type>, Count>;
};

template <class Type, size_t Count>
class array
{
private:
    static_assert(std::is_literal_type<Type>::value, "Type needs to be a literal type");
    static_assert(Count >= 1, "Empty array does not make sense");

    static constexpr Type instance    = {};
    static constexpr auto field_sizes = detail::field_sizes(instance);
    static constexpr auto packed_size = detail::fold_sum(field_sizes);

public:
    static constexpr Type fields = {};

    constexpr auto size()
    {
        return Count;
    }

    array()
    {
        detail::initialize_struct<Count, 0>(detail::struct_as_tuple(fields), data.data(), field_sizes);
    }

    template <class FieldType>
    auto operator[](FieldType &_field)
    {
        using FT = typename std::remove_cv<FieldType>::type;
        const auto index = detail::field_index(fields, &_field);
        assert(index != std::numeric_limits<size_t>::max());
        const auto offset = detail::sum_n_parts(field_sizes, index);
        const std::size_t begin       = reinterpret_cast<const std::size_t>(data.data());
        FT *              fieldOffset = reinterpret_cast<FT *>(begin + offset * Count);
        return field<Type, Count, FT>{fieldOffset};
    }

    template <class FieldType>
    auto operator[](FieldType &_field) const
    {
        using FT = typename std::remove_cv<FieldType>::type;
        const auto index = detail::field_index(fields, &_field);
        assert(index != std::numeric_limits<size_t>::max());
        const auto offset = detail::sum_n_parts(field_sizes, index);
        const std::size_t begin       = reinterpret_cast<const std::size_t>(data.data());
        const FT *              fieldOffset = reinterpret_cast<const FT *>(begin + offset * Count);
        return field<const Type, Count, const FT>{fieldOffset};
    }

    auto transposed()
    {
        return object_container<Type, Count>(data.data(), 0);
    }

    auto transposed() const
    {
        return object_container<const Type, Count>(data.data(), 0);
    }

    auto transposed(std::size_t index)
    {
        return *object_container<Type, Count>(data.data(), index);
    }

    auto transposed(std::size_t index) const
    {
        return *object_container<const Type, Count>(data.data(), index);
    }

private:
    std::array<uint8_t, Count * packed_size> data;

    friend class object<Type, Count>;
};
}
