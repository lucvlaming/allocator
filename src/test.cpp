#include "Common.h"
#include "Parent.h"
#include "Field.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

static const size_t NumElements = 10000;

//TODO: somehow support templated elements
struct DataOrientedElement: Parent<DataOrientedElement> {
    Field(1, int32_t) first;
    Field(2, int32_t) second;
    Field(3, int32_t) third;
    Field(4, int32_t) fourth;
    Field(5, int32_t) fifth;
};

TEST_CASE("Addresses continuous", "[memory]") {
    std::vector<DataOrientedElement*> storage;
    storage.reserve(NumElements);

    //first create elements
    for (size_t i=0; i<NumElements; ++i) {
        storage.emplace_back(new DataOrientedElement());
    }

    auto* addr = reinterpret_cast<char*>(&storage[0]->first);
    std::cout << addr << std::endl;
    for(size_t i=1; i<NumElements; ++i) {
        auto* next = reinterpret_cast<char*>(&storage[i]->first);
        ptrdiff_t diff = next - addr;
        REQUIRE(next - addr == sizeof(int32_t));
        addr = next;
    }

    storage.clear();
}
