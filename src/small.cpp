#include <iostream>
#include <cstddef>
#include <xmmintrin.h>
#include <array>

#include "array.hpp"

struct DataInner
{
    int32_t second = 6;
    int32_t third  = 3;
};

struct Data
{
    int8_t screwup = 2;
    int64_t first = 10;
    int64_t second = 9;
    DataInner inner;
    int64_t   fourth = 5;
    int16_t   fifth  = 8;
};

static void escape(const void* p) {
    asm volatile ( "" : : "g" (const_cast<void*>(p)) : "memory" );
}
int main(int, char *[])
{
    dod::array<Data, 16> elements;

    //uint8_t count = 0;

    // -------------------
    // interface 1
    // loop over all elements for specific field
    for (auto &fourth: elements[elements.fields.fourth])
        fourth = 31;

//    escape(&elements);
//    for (auto &first: elements[elements.fields.first])
//        std::cerr << "TEST: " << first << std::endl;

    escape(&elements);
    for (auto &first: elements[elements.fields.first])
        first = 15;

//    escape(&elements);
//    for (auto &first: elements[elements.fields.first])
//        std::cerr << "TEST: " << first << std::endl;

    // WRONG: initialize_struct etc should not use sizes but offsets
    // ALSO: rewrite code to have a "walk_struct" function that stores all metadata at once: offset, sizes


//    // access using gather & scatter by index
//    for (size_t i          = 0; i < 16; ++i)
//        elements.transposed(i)->first = count++;
//    // iterate using gather & scatter with iterator
//    for (auto object : elements.transposed())
//        object->first = count++;

    return 0;
}
