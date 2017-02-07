#include <array>
#include <iostream>
#include <type_traits>
#include <vector>

struct DataInner {
  int32_t second;
  int32_t third;
};

struct Data {
  int64_t first;
  DataInner bla;
  int64_t fourth;
};

namespace dod {

template <typename Type, size_t Size, typename FieldType>
struct array_iterator {
  FieldType *begin() const { return ptr; }
  FieldType *end() const { return ptr + Size; }
  array_iterator &operator*() const { return *ptr; }
  array_iterator &operator++() {
    ++ptr;
    return *this;
  }

  FieldType *ptr;
};

template <typename Type, typename FieldType> struct vector_iterator {
  FieldType *begin() const { return ptr; }
  FieldType *end() const { return stop; }
  vector_iterator &operator*() const { return *ptr; }
  vector_iterator &operator++() {
    ++ptr;
    return *this;
  }

  FieldType *ptr;
  FieldType *stop;
};

template <typename Type, size_t Size> class array {
public:
  static_assert(std::is_pod<Type>::value, "Type needs to be POD");

  template <typename FieldType>
  array_iterator<Type, Size, FieldType> iterate(FieldType Type::*field) {
    Type instance;
    const size_t fieldMemberOffset =
        reinterpret_cast<size_t>(&(instance.*field)) -
        reinterpret_cast<size_t>(&instance);
    size_t begin = reinterpret_cast<size_t>(data.data());
    FieldType *fieldOffset =
        reinterpret_cast<FieldType *>(begin + (fieldMemberOffset * Size));
    return array_iterator<Type, Size, FieldType>{fieldOffset};
  }

private:
  std::array<Type, Size> data;
};

template <typename Type> class vector {
public:
  static_assert(std::is_pod<Type>::value, "Type needs to be POD");

  vector(size_t size) : data(size) {}

  template <typename FieldType>
  vector_iterator<Type, FieldType> iterate(FieldType Type::*field) {
    Type instance;
    const size_t fieldMemberOffset =
        reinterpret_cast<size_t>(&(instance.*field)) -
        reinterpret_cast<size_t>(&instance);
    size_t begin = reinterpret_cast<size_t>(data.data());
    FieldType *fieldOffset = reinterpret_cast<FieldType *>(
        begin + (fieldMemberOffset * data.size()));
    return vector_iterator<Type, FieldType>{fieldOffset,
                                            fieldOffset + data.size()};
  }

  void resize(size_t size) { data.resize(size); }

private:
  std::vector<Type> data;
};
}

static void escape(const void *p) {
  asm volatile("" : : "g"(const_cast<void *>(p)) : "memory");
}

int main(int, char *[]) {
  dod::array<Data, 16> elements;

  escape(&elements);
  uint8_t count = 0;
  for (auto &first : elements.iterate(&Data::first)) {
    first = count++;
  }
  std::cout << " set" << std::endl;
  escape(&elements);
  for (auto &bla : elements.iterate(&Data::bla)) {
    bla.second = count;
    bla.third = count;
    count++;
  }
  std::cout << " set" << std::endl;
  escape(&elements);
  for (auto &fourth : elements.iterate(&Data::fourth)) {
    fourth = count++;
  }
  std::cout << " set" << std::endl;

  dod::vector<Data> elements2(16);
  escape(&elements);
  count = 0;
  for (auto &first : elements2.iterate(&Data::first)) {
    first = count++;
  }
  std::cout << " set2" << std::endl;
  escape(&elements);
  for (auto &bla : elements2.iterate(&Data::bla)) {
    bla.second = count;
    bla.third = count;
    count++;
  }
  std::cout << " set2" << std::endl;
  escape(&elements);
  for (auto &fourth : elements2.iterate(&Data::fourth)) {
    fourth = count++;
  }
  std::cout << " set2" << std::endl;

  // how to use std:sort now?

  return 0;
}
