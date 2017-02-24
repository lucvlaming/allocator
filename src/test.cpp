#include <array>
#include <cstring>
#include <iostream>
#include <type_traits>
#include <vector>

struct DataInner
{
    int32_t second;
    int32_t third;
    int32_t fourth;
};

struct Data
{
    int64_t   first;
    DataInner bla;
    int64_t   fifth;
    std::array<int8_t, 10> sixth;
    int16_t seventh;
};

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

template <class T, std::size_t... I>
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

template <class T>
constexpr auto packed_field_offsets_impl(...) noexcept
{
    return 0;
}

template <class T>
constexpr auto packed_field_offsets_impl(T &instance,
                                         std::integral_constant<std::size_t, 5>) noexcept
{
    auto[a, b, c, d, e] = instance;
    constexpr auto sa = sizeof(a);
    constexpr auto sb = sizeof(b) + sa;
    constexpr auto sc = sizeof(c) + sb;
    constexpr auto sd = sizeof(d) + sc;
    constexpr auto se = sizeof(e) + sd;

    return std::array<size_t, 5>{{sa, sb, sc, sd, se}};
}

template <class T>
constexpr auto packed_field_offsets(T &instance) noexcept
{
    constexpr auto field_count = fields_count<T>();
    return packed_field_offsets_impl<T>(instance,
                                        std::integral_constant<std::size_t, field_count>());
}

template <typename Type, size_t Size, typename FieldType>
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

template <typename Type, typename FieldType>
struct vector_iterator
{
    FieldType *begin() const
    {
        return ptr;
    }
    FieldType *end() const
    {
        return stop;
    }
    vector_iterator &operator*() const
    {
        return *ptr;
    }
    vector_iterator &operator++()
    {
        ++ptr;
        return *this;
    }

    FieldType *ptr;
    FieldType *stop;
};

template <typename Type, size_t Size>
class array
{
public:
    static_assert(std::is_standard_layout<Type>::value, "Type needs to have standard layout");

    array()
    {
        // 0 8 24 32 42
        constexpr Type instance = {};
        auto           offsets  = packed_field_offsets(instance);
        size_t         i        = 10;
        for (auto offset : offsets)
            std::memset(data.data() + offset, i++, 8);
    }

    template <typename FieldType>
    array_iterator<Type, Size, FieldType> iterate(FieldType Type::*field)
    {
        Type         instance;
        const size_t fieldMemberOffset =
                reinterpret_cast<size_t>(&(instance.*field)) - reinterpret_cast<size_t>(&instance);
        size_t     begin       = reinterpret_cast<size_t>(data.data());
        FieldType *fieldOffset = reinterpret_cast<FieldType *>(begin + (fieldMemberOffset * Size));
        return array_iterator<Type, Size, FieldType>{fieldOffset};
    }

private:
    std::array<Type, Size> data;
};

template <typename Type>
class vector
{
public:
    static_assert(std::is_pod<Type>::value, "Type needs to be POD");

    vector(size_t size) : data(size)
    {
    }

    template <typename FieldType>
    vector_iterator<Type, FieldType> iterate(FieldType Type::*field)
    {
        Type         instance;
        const size_t fieldMemberOffset =
                reinterpret_cast<size_t>(&(instance.*field)) - reinterpret_cast<size_t>(&instance);
        size_t     begin = reinterpret_cast<size_t>(data.data());
        FieldType *fieldOffset =
                reinterpret_cast<FieldType *>(begin + (fieldMemberOffset * data.size()));
        return vector_iterator<Type, FieldType>{fieldOffset, fieldOffset + data.size()};
    }

    void resize(size_t size)
    {
        data.resize(size);
    }

private:
    std::vector<Type> data;
};
}

static void escape(const void *p)
{
    asm volatile("" : : "g"(const_cast<void *>(p)) : "memory");
}

int main(int, char *[])
{
    dod::array<Data, 16> elements;

    escape(&elements);
    uint8_t count = 0;
    Data    element;
    for (auto &first : elements.iterate(element.bla.second))
        first = count++;
    std::cout << " set" << std::endl;
    escape(&elements);
    //  for (auto &bla : elements.iterate(&Data::bla)) {
    //    bla.second = count;
    //    bla.third = count;
    //    count++;
    //  }
    //  std::cout << " set" << std::endl;
    //  escape(&elements);
    //  for (auto &fourth : elements.iterate(&Data::fifth)) {
    //    fourth = count++;
    //  }
    //  std::cout << " set" << std::endl;

    //  dod::vector<Data> elements2(16);
    //  escape(&elements);
    //  count = 0;
    //  for (auto &first : elements2.iterate(&Data::first)) {
    //    first = count++;
    //  }
    //  std::cout << " set2" << std::endl;
    //  escape(&elements);
    //  for (auto &bla : elements2.iterate(&Data::bla)) {
    //    bla.second = count;
    //    bla.third = count;
    //    count++;
    //  }
    //  std::cout << " set2" << std::endl;
    //  escape(&elements);
    //  for (auto &fourth : elements2.iterate(&Data::fifth)) {
    //    fourth = count++;
    //  }
    //  std::cout << " set2" << std::endl;

    // how to use std:sort now?

    return 0;
}
