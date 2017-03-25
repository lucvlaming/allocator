#include <xmmintrin.h>
#include <array>
#include <cstddef>

#include "catch.hpp"

#include "array.hpp"

struct DataInner
{
    int8_t prePadding = 1;
    int32_t first = 15;
    int32_t second = 8;
    int8_t postPadding = 5;
};

struct Data
{
    int8_t    first = 2;
    int64_t   second = 5;
    DataInner third;
    int64_t   fourth = 9;
    std::array<int8_t, 9> fifth = {{2,3,5,7,11,13,19,23,29}};
    int16_t sixth = 109;
    __m128 seventh = {1.3, 2.2, 3.1, 4.5};
    __m128i eighth = {11, 13};
    __m128d ninth = {11.1, 13.1};
};

auto CompareAllByField = [](const auto& array, const auto& truth)
{
    for (const auto& m : array[array.fields.first])
        REQUIRE(m == truth.first);
    for (const auto& m : array[array.fields.second])
        REQUIRE(m == truth.second);
    for (const auto& m : array[array.fields.third])
    {
        REQUIRE(m.prePadding == truth.third.prePadding);
        REQUIRE(m.first == truth.third.first);
        REQUIRE(m.second == truth.third.second);
        REQUIRE(m.postPadding == truth.third.postPadding);
    }
    for (const auto& m : array[array.fields.fourth])
        REQUIRE(m == truth.fourth);
    for (const auto& m : array[array.fields.fifth])
        REQUIRE(m == truth.fifth);
    for (const auto& m : array[array.fields.sixth])
        REQUIRE(m == truth.sixth);
    for (const auto& m : array[array.fields.seventh])
        REQUIRE(memcmp(&m, &truth.seventh, sizeof(truth.seventh)) == 0);
    for (const auto& m : array[array.fields.eighth])
        REQUIRE(memcmp(&m, &truth.eighth, sizeof(truth.eighth)) == 0);
    for (const auto& m : array[array.fields.ninth])
        REQUIRE(memcmp(&m, &truth.ninth, sizeof(truth.ninth)) == 0);
};

auto CompareAllUsingTranspose = [](const auto& elem, const auto& truth)
{
    REQUIRE(elem->first == truth.first);
    REQUIRE(elem->second == truth.second);
    REQUIRE(elem->third.prePadding == truth.third.prePadding);
    REQUIRE(elem->third.first == truth.third.first);
    REQUIRE(elem->third.second == truth.third.second);
    REQUIRE(elem->third.postPadding == truth.third.postPadding);
    REQUIRE(elem->fourth == truth.fourth);
    REQUIRE(elem->fifth == truth.fifth);
    REQUIRE(elem->sixth == truth.sixth);
    REQUIRE(memcmp(&elem->seventh, &truth.seventh, sizeof(truth.seventh)) == 0);
    REQUIRE(memcmp(&elem->eighth, &truth.eighth, sizeof(truth.eighth)) == 0);
    REQUIRE(memcmp(&elem->ninth, &truth.ninth, sizeof(truth.ninth)) == 0);
};


TEST_CASE("Construction honors default member initializers")
{
    Data truth;
    dod::array<Data, 16> array;

    SECTION("Test each field")
    {
        CompareAllByField(array, truth);
    }

    SECTION("Test transposed access")
    {
        for (size_t i=0; i<array.size(); ++i)
        {
            const auto elem = array.transposed(i);
            CompareAllUsingTranspose(elem, truth);
        }
    }
}

TEST_CASE("Transposed functionality")
{
    Data truth;
    dod::array<Data, 16> array;

    SECTION("Test transposed access")
    {
        for (size_t i=0; i<array.size(); ++i)
        {
            const auto elem = array.transposed(i);
            CompareAllUsingTranspose(elem, truth);
        }
    }

    SECTION("Set value")
    {
        Data newObject{3,6,{2,16,9,6},10,{{3,4,6,8,12,14,20,24,30}},110,{1.4,2.3,3.2,4.6},{12,14},{11.2,13.2}};
        for (size_t i=0; i<array.size(); ++i)
        {
            auto elem = array.transposed(i);
            std::memcpy(*elem, &newObject, sizeof(newObject));
        }
        SECTION("Check value")
        {
            for (size_t i=0; i<array.size(); ++i)
                CompareAllUsingTranspose(array.transposed(i), newObject);
        }
    }
}
