#include "array.hpp"

struct DataInner
{
    int32_t second = 6;
    int32_t third  = 3;
    int8_t  bla    = 11;
};

struct Data
{
    int64_t   first = 10;
    DataInner inner;
    int64_t   fourth = 5;
    int16_t   fifth  = 8;
};

int main(int, char *[])
{
    dod::array<Data, 16> elements;

    uint8_t count = 0;

    // -------------------
    // interface 1
    // loop over all elements for specific field
    for (auto &first : elements[elements.fields.first])
        first = count++;
    // access as object (gather & scatter)
    for (size_t i          = 0; i < 16; ++i)
        elements[i]->first = count++;

    // -------------------
    // interface 2: explicitly name iteration 'type'
    // loop over all elements for specific field
    for (auto &first : elements.field[elements.fields.first])
        first = count++;
    // access as object (gather & scatter)
    for (size_t i                 = 0; i < 16; ++i)
        elements.object[i]->first = count++;

    // -------------------
    // extra interface
    for (auto &object : elements.objects())
        object.first = count++;
    return 0;
}
