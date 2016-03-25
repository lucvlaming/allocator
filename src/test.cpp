#define CATCH_CONFIG_MAIN

//#include "DataOrientedArray.h"
#include "catch.hpp"

//#include <vector>
//#include <future>
//#include <iostream>

//static const size_t NumElements = 100;

//template<size_t Stride = 1>
//struct DataOrientedElement {
//    DataOrientedArray<1, int32_t> first;
//    DataOrientedArray<2, int32_t> second[Stride];
//};

//TEST_CASE("Addresses continuous", "[memory]") {
//    std::vector<DataOrientedElement<4>> storage;
//    storage.reserve(NumElements);

//    //first create elements
//    for (size_t i=0; i<NumElements; ++i) {
//        storage.emplace_back();
//    }

//    auto* addr = reinterpret_cast<char*>(&storage[0].first);
//    std::cout << addr << std::endl;
//    for(size_t i=1; i<NumElements; ++i) {
//        auto* next = reinterpret_cast<char*>(&storage[i].first);
//        ptrdiff_t diff = next - addr;
//        REQUIRE(next - addr == sizeof(int32_t));
//        addr = next;
//    }

//    storage.clear();
//}
