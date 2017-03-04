#include <array>
#include <cassert>
#include <type_traits>

#include "field_sizes.hpp"
#include "utility.hpp"

namespace dod
{

template <class Type, size_t Count>
class array;

template <class Type, size_t Count, class FieldType>
class array_iterator
{
public:
    FieldType *begin() const
    {
        return ptr;
    }
    FieldType *end() const
    {
        return ptr + Count;
    }
    array_iterator &operator*() const
    {
        return *ptr;
    }
    array_iterator &operator++()
    {
        ++ptr;
        return *this;
    }

private:
    array_iterator(FieldType *p) : ptr(p)
    {
    }
    FieldType *ptr;

    friend class array<Type, Count>;
};

template <class Type, size_t Count>
class array_object
{
public:
    ~array_object()
    {
        scatter_struct<Count>(index, reinterpret_cast<const std::uint8_t *>(&object), data,
                              array<Type, Count>::field_sizes);
    }

    Type *operator->()
    {
        return &object;
    }

private:
    array_object(uint8_t *d, std::size_t i) : data(d), index(i)
    {
        gather_struct<Count>(index, reinterpret_cast<std::uint8_t *>(&object), data,
                             array<Type, Count>::field_sizes);
    }

    Type        object;
    uint8_t *   data;
    std::size_t index;

    friend class array<Type, Count>;
};

template <class Type, size_t Count>
class array
{
private:
    static_assert(std::is_literal_type<Type>::value, "Type needs to be a literal type");

    static constexpr Type instance    = {};
    static constexpr auto field_sizes = get_field_sizes(instance);
    static constexpr auto packed_size = fold_sum(field_sizes);

public:
    static constexpr Type fields = {};

    array()
    {
        auto *prototype = reinterpret_cast<const uint8_t *>(&fields);
        initialize_struct<Count>(prototype, data.data(), field_sizes);
    }

    template <class FieldType>
    auto operator[](FieldType &field)
    {
        using FT = typename std::remove_cv<FieldType>::type;
        const std::size_t fieldMemberOffset =
                reinterpret_cast<size_t>(&field) - reinterpret_cast<size_t>(&fields);
        assert(fieldMemberOffset < packed_size);
        std::cout << "TEST: " << fieldMemberOffset << std::endl;
        const std::size_t begin       = reinterpret_cast<std::size_t>(data.data());
        FT *              fieldOffset = reinterpret_cast<FT *>(begin + fieldMemberOffset * Count);
        return array_iterator<Type, Count, FT>{fieldOffset};
    }

    auto operator[](std::size_t index)
    {
        return array_object<Type, Count>(data.data(), index);
    }

private:
    std::array<uint8_t, Count * packed_size> data;

    friend class array_object<Type, Count>;
};
}
