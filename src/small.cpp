#include <iostream>
#include <vector>

#include "Parent.h"
#include "Field.h"


struct Test: Parent<Test> {
    Test(): first(10) {}

    Field(1, int64_t) first;
//    Field(2, int32_t) second;
//    Field(3, int8_t) third;
};

int main(int, char*[]) {
    static constexpr size_t N = 10;
    std::vector<Test*> array;
    array.reserve(N);
    for (size_t i=0; i<N; ++i) {
        array.emplace_back(new Test());
    }
    return 0;
}