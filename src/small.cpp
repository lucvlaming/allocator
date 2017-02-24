#include <iostream>

#include "array.hpp"

struct DataInner
{
    int32_t second = 6;
    int32_t third  = 3;
};

struct Data
{
    int64_t   first = 10;
    DataInner inner;
    int64_t   fourth = 5;
    int16_t   fifth  = 8;
};

static void escape(const void *p)
{
    asm volatile("" : : "g"(const_cast<void *>(p)) : "memory");
}

int main(int, char *[])
{
    dod::array<Data, 16> elements;

    escape(&elements);
    uint8_t count = 0;
    for (auto &first : elements[elements.fields.first])
        std::cout << "first before=" << first << std::endl;
    for (auto &first : elements[elements.fields.first])
        first = count++;
    for (auto &first : elements[elements.fields.first])
        std::cout << "first after=" << first << std::endl;
    std::cout << elements[10].fourth << std::endl;
    escape(&elements);
    return 0;
}
