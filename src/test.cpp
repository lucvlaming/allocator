#include <array>
#include <iostream>
#include <type_traits>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

struct DataInner {
  int32_t second;
  int8_t third;
};

struct Data {
  int64_t first;
  DataInner bla;
  int64_t fourth;
};

template <typename Type, size_t Size, typename FieldType>
struct struct_array_iterator {
  FieldType *begin() const { return ptr; }
  FieldType *end() const { return ptr + Size; }
  struct_array_iterator &operator*() const { return *ptr; }
  struct_array_iterator &operator++() {
    ++ptr;
    return *this;
  }

  FieldType *ptr;
};

template <typename Type, size_t Size> struct struct_array {
  static_assert(std::is_pod<Type>::value, "Type needs to be POD");

  template <typename FieldType>
  struct_array_iterator<Type, Size, FieldType> iterate(FieldType Type::*field) {
    Type instance;
    const size_t fieldMemberOffset =
        reinterpret_cast<size_t>(&(instance.*field)) -
        reinterpret_cast<size_t>(&instance);
    size_t begin = reinterpret_cast<size_t>(data.data());
    FieldType *fieldOffset =
        reinterpret_cast<FieldType *>(begin + (fieldMemberOffset * Size));
    return struct_array_iterator<Type, Size, FieldType>{fieldOffset};
  }

  std::array<Type, Size> data;
};

static void escape(const void *p) {
  asm volatile("" : : "g"(const_cast<void *>(p)) : "memory");
}

TEST_CASE("Addresses continuous", "[memory]") {
  static constexpr size_t Count = 16;
  struct_array<Data, Count> elements;

  uint8_t count = 0;
  for (auto &first : elements.iterate(&Data::first)) {
    first = count++;
  }
  for (auto &bla : elements.iterate(&Data::bla)) {
    bla.second = count++;
    bla.third = count++;
  }
  for (auto &fourth : elements.iterate(&Data::fourth)) {
    fourth = count++;
  }

  count = 0;
  for (auto &first : elements.iterate(&Data::first)) {
    REQUIRE(first == count);
    ++count;
  }
  for (auto &bla : elements.iterate(&Data::bla)) {
    REQUIRE(bla.second == count);
    ++count;
    REQUIRE(bla.third == count);
    ++count;
  }
  for (auto &fourth : elements.iterate(&Data::fourth)) {
    REQUIRE(fourth == count);
    ++count;
  }
}
