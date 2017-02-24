#include <array>
#include <type_traits>

#include "field_sizes.hpp"
#include "utility.hpp"

namespace dod
{

template <class Type, size_t Size, class FieldType>
struct array_iterator
{
    FieldType *begin() const
    {
        return ptr;
    }
    FieldType *end() const
    {
        return ptr + Size;
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

    FieldType *ptr;
};

template <class Type>
struct array_object : Type
{
    array_object(std::size_t i) : Type{}, index(i)
    {
    }

    ~array_object()
    {
        std::cerr << "index was " << index << std::endl;
    }
    std::size_t index;
};

template <class Type, size_t Size>
class array
{
public:
    static_assert(std::is_literal_type<Type>::value, "Type needs to be a literal type");

    static constexpr Type fields      = {};
    static constexpr auto field_sizes = ::dod::field_sizes(fields);
    static constexpr auto packed_size = ::dod::fold_sum(field_sizes);

    array()
    {
        ::dod::initialize_struct<Size>(reinterpret_cast<const uint8_t *>(&fields), data.data(),
                                       field_sizes);
    }

    template <typename FieldType>
    auto operator[](FieldType &field)
    {
        using FT = typename std::remove_cv<FieldType>::type;
        const std::size_t fieldMemberOffset =
                reinterpret_cast<size_t>(&field) - reinterpret_cast<size_t>(&fields);
        const std::size_t begin       = reinterpret_cast<std::size_t>(data.data());
        FT *              fieldOffset = reinterpret_cast<FT *>(begin + fieldMemberOffset * Size);
        return array_iterator<Type, Size, FT>{fieldOffset};
    }

    auto operator[](std::size_t index)
    {
        return array_object<Type>(index);
    }

private:
    std::array<uint8_t, Size * packed_size> data;
};
}
