#include <array>
#include <cstddef>
#include <xmmintrin.h>

#include "catch.hpp"

#include "array.hpp"

struct DataInner
{
    int8_t  prePadding  = 1;
    int32_t first       = 15;
    int32_t second      = 8;
    int8_t  postPadding = 5;
};

struct Data
{
    int8_t    first  = 2;
    int64_t   second = 5;
    DataInner third;
    int64_t   fourth = 9;
    std::array<int8_t, 9> fifth = {{2, 3, 5, 7, 11, 13, 19, 23, 29}};
    int16_t sixth   = 109;
    __m128  seventh = {1.3, 2.2, 3.1, 4.5};
    __m128i eighth  = {11, 13};
    __m128d ninth   = {11.1, 13.1};
};

auto check_field = [](const auto &array, const auto &field, const auto &check) {
    size_t count = 0;
    for (const auto &m : array[field])
    {
        const bool ok = check(m);
        REQUIRE(ok);
        count++;
    }
    REQUIRE(count == array.size());
};

auto compare_all_by_field =
        [](const auto &array, const auto &truth, const size_t &size, const void *fieldException) {
            const auto check_field_if = [&](const auto &field, const auto &check) {
                if (&field != fieldException)
                    check_field(array, field, check);
            };

            REQUIRE(array.size() == size);

            size_t count = 0;

            check_field_if(array.fields.first, [&](const auto &m) { return m == truth.first; });
            check_field_if(array.fields.second, [&](const auto &m) { return m == truth.second; });
            check_field_if(array.fields.third, [&](const auto &m) {
                return m.prePadding == truth.third.prePadding && m.first == truth.third.first
                        && m.second == truth.third.second
                        && m.postPadding == truth.third.postPadding;
            });
            check_field_if(array.fields.fourth, [&](const auto &m) { return m == truth.fourth; });
            check_field_if(array.fields.fifth, [&](const auto &m) { return m == truth.fifth; });
            check_field_if(array.fields.sixth, [&](const auto &m) { return m == truth.sixth; });
            check_field_if(array.fields.seventh, [&](const auto &m) {
                return memcmp(&m, &truth.seventh, sizeof(truth.seventh)) == 0;
            });
            check_field_if(array.fields.eighth, [&](const auto &m) {
                return memcmp(&m, &truth.eighth, sizeof(truth.eighth)) == 0;
            });
            check_field_if(array.fields.ninth, [&](const auto &m) {
                return memcmp(&m, &truth.ninth, sizeof(truth.ninth)) == 0;
            });
        };

auto CompareAllUsingTranspose = [](const auto &elem, const auto &truth) {
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
    const Data truth;
    auto *     noFieldException = reinterpret_cast<void *>(0);

    SECTION("1 element")
    {
        const dod::array<Data, 1> array;
        compare_all_by_field(array, truth, 1, noFieldException);
    }
    SECTION("3 elements")
    {
        const dod::array<Data, 3> array;
        compare_all_by_field(array, truth, 3, noFieldException);
    }
    SECTION("16 elements")
    {
        const dod::array<Data, 16> array;
        compare_all_by_field(array, truth, 16, noFieldException);
    }
    SECTION("1023 elements")
    {
        const dod::array<Data, 1023> array;
        compare_all_by_field(array, truth, 1023, noFieldException);
    }
    SECTION("1025 elements")
    {
        const dod::array<Data, 1025> array;
        compare_all_by_field(array, truth, 1025, noFieldException);
    }
}

TEST_CASE("Set fields")
{
    const Data original;
    dod::array<Data, 251> array;
    size_t i = 0;
    size_t j = 0;

    const auto generate = []() {};

    const auto set_field = [&](const auto &field, const auto &setter, const auto &checker) {
        for (auto &m : array[field])
            setter(m);

        check_field(array, field, checker);
        compare_all_by_field(array, original, 251, &array.fields.first);
    };

    set_field(array.fields.first, [&](auto &m) { m = static_cast<int8_t>(i++); },
              [&](const auto &m) { return m == static_cast<int8_t>(j++); });

    for (auto &second : array[array.fields.second])
        second = i++;
    check_field(array, array.fields.first,
                [&](const auto &m) { return m == static_cast<int8_t>(j++); });
    compare_all_by_field(array, original, 251, &array.fields.first);

    for (auto &third : array[array.fields.third])
        third = {static_cast<int8_t>(i++), static_cast<int32_t>(i++), static_cast<int32_t>(i++),
                 static_cast<int8_t>(i++)};
    for (auto &fourth : array[array.fields.fourth])
        fourth = i++;
}

TEST_CASE("Transposed functionality")
{
    Data truth;
    dod::array<Data, 16> array;

    SECTION("Test transposed access")
    {
        REQUIRE(array.size() == 16);

        for (size_t i = 0; i < array.size(); ++i)
        {
            const auto elem = array.transposed(i);
            CompareAllUsingTranspose(elem, truth);
        }
    }

    SECTION("Set value")
    {
        Data newObject{3,
                       6,
                       {2, 16, 9, 6},
                       10,
                       {{3, 4, 6, 8, 12, 14, 20, 24, 30}},
                       110,
                       {1.4, 2.3, 3.2, 4.6},
                       {12, 14},
                       {11.2, 13.2}};
        for (size_t i = 0; i < array.size(); ++i)
        {
            auto elem = array.transposed(i);
            std::memcpy(*elem, &newObject, sizeof(newObject));
        }
        SECTION("Check value")
        {
            for (size_t i = 0; i < array.size(); ++i)
                CompareAllUsingTranspose(array.transposed(i), newObject);
        }
    }
}
