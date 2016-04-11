#include <iostream>
//#include <vector>

#include "Parent.h"
#include "Field.h"

struct InliningTest: Parent<InliningTest> {
    Field(1, int64_t) first;  //offset = 16
    Field(2, int32_t) second; //offset = 1935760
    Field(3, int8_t ) third;  //offset = 2903632

    InliningTest() {
        //note: do not set values for the fields to keep assembly clean
        //somehow the constructor adds a test that cannot be optimized away
    }
};

struct Test: Parent<Test> {
    Test(): first(-1), second(-2), third(-3) {}

    Field(1, int64_t) first;
    Field(2, int32_t) second;
    Field(3, int8_t) third;
};

int main(int, char*[]) {
//    static constexpr size_t N = 1024*1024*4-100;
//    std::vector<Test*> array;
//    array.reserve(N);
//    for (size_t i=0; i<N; ++i) {
//        array.emplace_back(new Test());
//    }
//    for (size_t i=0; i<N; ++i) {
//        array[i]->first = 5;
//    }
//    for (size_t i=0; i<N; ++i) {
//        delete array[i];
//    }
    auto test = new InliningTest();
    std::cout << "first-field  " << reinterpret_cast<void*>(&test->first) << std::endl;
    std::cout << "second-field " << reinterpret_cast<void*>(&test->second) << std::endl;
    std::cout << "third-field  " << reinterpret_cast<void*>(&test->third) << std::endl;

    new Test[10];
    return 0;
}
