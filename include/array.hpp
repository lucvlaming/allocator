#pragma once

#include <iostream>

#include <array>
#include <cassert>
#include <type_traits>

#include "field_index.hpp"
#include "field_sizes.hpp"
#include "struct_as_tuple.hpp"
#include "utility.hpp"

namespace dod
{

template <class Type, size_t Count>
class array;

template <class Type, class Storage, size_t Count>
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

template <class Type, class Storage, size_t Count>
class object
{
public:
    ~object()
    {
        if
            constexpr(!std::is_const<Type>::value)
                    detail::scatter_struct<Count, 0>(_index, detail::struct_as_tuple(_object),
                                                     _storage, array<Type, Count>::_field_sizes);
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
    object(Storage *storage, std::size_t index) : _storage(storage), _index(index)
    {
        detail::gather_struct<Count, 0>(_index, detail::struct_as_tuple(_object), _storage,
                                        array<Type, Count>::_field_sizes);
    }

    Type     _object;
    Storage *_storage;
    size_t   _index;

    friend class array<std::remove_const_t<Type>, Count>;
    friend class object_container<Type, Storage, Count>;
    friend class object_container<const Type, const Storage, Count>;
};

template <class Type, class Storage, size_t Count>
class object_container
{
public:
    auto begin()
    {
        return object_container<Type, Storage, Count>(_storage, 0);
    }

    auto begin() const
    {
        return object_container<const Type, const Storage, Count>(_storage, 0);
    }

    auto end()
    {
        return object_container<Type, Storage, Count>(_storage, Count);
    }

    auto end() const
    {
        return object_container<const Type, const Storage, Count>(_storage, Count);
    }

    auto operator*()
    {
        return object<Type, Storage, Count>(_storage, _index);
    }

    auto operator*() const
    {
        return object<const Type, const Storage, Count>(_storage, _index);
    }

    auto &operator++()
    {
        ++_index;
        return *this;
    }

    auto operator==(const object_container &other) const
    {
        return _index == other._index && _storage == other._storage;
    }

    auto operator!=(const object_container &other) const
    {
        return !(operator==(other));
    }

private:
    object_container(Storage *storage, size_t index) : _storage(storage), _index(index)
    {
    }

    Storage *_storage;
    size_t   _index;

    friend class array<std::remove_const_t<Type>, Count>;
};

template <class Type, size_t Count>
class array
{
private:
    static_assert(std::is_literal_type<Type>::value, "Type needs to be a literal type");
    static_assert(Count >= 1, "Empty array does not make sense");

    static constexpr Type _instance    = {};
    static constexpr auto _field_sizes = detail::field_sizes(_instance);
    static constexpr auto _packed_size = detail::fold_sum(_field_sizes);

public:
    static constexpr Type fields = {};

    constexpr auto size() const
    {
        return Count;
    }

    array()
    {
        detail::initialize_struct<Count, 0>(detail::struct_as_tuple(fields), _storage.data(),
                                            _field_sizes);
    }

    template <class FieldType>
    auto operator[](FieldType &_field)
    {
        using FT = typename std::remove_cv<FieldType>::type;
        return get_field<Type, FT>(_field);
    }

    template <class FieldType>
    auto operator[](FieldType &_field) const
    {
        using FT = typename std::remove_cv<FieldType>::type;
        return get_field<const Type, const FT>(_field);
    }

    auto transposed()
    {
        return object_container<Type, std::uint8_t, Count>(_storage.data(), 0);
    }

    auto transposed() const
    {
        return object_container<const Type, const std::uint8_t, Count>(_storage.data(), 0);
    }

    auto transposed(std::size_t index)
    {
        return *object_container<Type, std::uint8_t, Count>(_storage.data(), index);
    }

    auto transposed(std::size_t index) const
    {
        return *object_container<const Type, const std::uint8_t, Count>(_storage.data(), index);
    }

private:
    template <class T, class FT, class FieldType>
    auto get_field(FieldType &_field) const
    {
        const auto index = detail::field_index(fields, &_field);
        assert(index != std::numeric_limits<size_t>::max());
        const auto        offset      = detail::sum_n_parts(_field_sizes, index);
        const std::size_t begin       = reinterpret_cast<const std::size_t>(_storage.data());
        FT *              fieldOffset = reinterpret_cast<FT *>(begin + offset * Count);
        return field<T, Count, FT>{fieldOffset};
    }

    std::array<uint8_t, Count * _packed_size> _storage;

    friend class object<Type, std::uint8_t, Count>;
    friend class object<const Type, const std::uint8_t, Count>;
};
}
