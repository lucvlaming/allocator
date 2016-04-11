#include "Common.h"
#include "Parent.h"
#include "Field.h"

#include <benchmark/benchmark.h>
#include <iostream>

static void escape(const void* p) {
    asm volatile ( "" : : "g" (const_cast<void*>(p)) : "memory" );
}

//TODO: mark functions no exceptions


struct DataOrientedElement1Field: Parent<DataOrientedElement1Field> {
    DataOrientedElement1Field(int32_t f): first(f) {}
    Field(1, int32_t) first;
};

struct DataOrientedElement2Fields: Parent<DataOrientedElement2Fields> {
    DataOrientedElement2Fields(int32_t f): first(f) {}
    Field(1, int32_t) first;
    Field(2, int32_t) second;
};

struct DataOrientedElement3Fields: Parent<DataOrientedElement3Fields> {
    DataOrientedElement3Fields(int32_t f): first(f) {}
    Field(1, int32_t) first;
    Field(2, int32_t) second;
    Field(3, int32_t) third;
};

struct DataOrientedElement4Fields: Parent<DataOrientedElement4Fields> {
    DataOrientedElement4Fields(int32_t f): first(f) {}
    Field(1, int32_t) first;
    Field(2, int32_t) second;
    Field(3, int32_t) third;
    Field(4, int32_t) fourth;
};

struct DataOrientedElement8Fields: Parent<DataOrientedElement8Fields> {
    DataOrientedElement8Fields(int32_t f): first(f) {}
    Field(1, int32_t) first;
    Field(2, int32_t) second;
    Field(3, int32_t) third;
    Field(4, int32_t) fourth;
    Field(5, int32_t) fifth;
    Field(6, int32_t) sixth;
    Field(7, int32_t) seventh;
    Field(8, int32_t) eigth;
};

template<size_t Stride = 1>
struct StridedElement {
    StridedElement(int32_t f): first(f) {}
    int32_t first;
    int32_t fill[Stride];
};

template<typename T>
static void CreateDataOriented(benchmark::State& state) {
    std::vector<T*> storage;
    storage.reserve(state.range_x());
    while (state.KeepRunning()) {
        storage.clear();
        storage.reserve(state.range_x());
        for (int i=0; i<state.range_x(); ++i) {
            storage.emplace_back(new T(i));
        }
        for (int i=0; i<state.range_x(); ++i) {
            delete storage[i];
        }
    }
}

template<size_t Stride>
static void CreateStrided(benchmark::State& state) {
    typedef StridedElement<Stride> Elem;

    std::vector<Elem*> storage;
    storage.reserve(state.range_x());

    while (state.KeepRunning()) {
        storage.clear();
        storage.reserve(state.range_x());

        for (int i=0; i<state.range_x(); ++i) {
            storage.emplace_back(new Elem(i));
        }
        for (int i=0; i<state.range_x(); ++i) {
            delete storage[i];
        }
    }
}

template<typename T>
static void AccessDataOriented(benchmark::State& state) {
    std::vector<T*> storage;
    storage.reserve(state.range_x());
    for (int i=0; i<state.range_x(); ++i) {
        storage.emplace_back(new T(i));
    }

    while (state.KeepRunning()) {
        for (int i=0; i<state.range_x(); ++i) {
            storage[i]->first = 5;
        }
    }

    for (int i=0; i<state.range_x(); ++i) {
        delete storage[i];
    }
    storage.clear();
}


BENCHMARK_TEMPLATE(CreateDataOriented, DataOrientedElement1Field )->Arg(1024*100);
BENCHMARK_TEMPLATE(CreateDataOriented, DataOrientedElement2Fields)->Arg(1024*100);
BENCHMARK_TEMPLATE(CreateDataOriented, DataOrientedElement3Fields)->Arg(1024*100);
BENCHMARK_TEMPLATE(CreateDataOriented, DataOrientedElement4Fields)->Arg(1024*100);
BENCHMARK_TEMPLATE(CreateDataOriented, DataOrientedElement8Fields)->Arg(1024*100);
BENCHMARK_TEMPLATE(CreateStrided, 1)->Arg(1024*100);
BENCHMARK_TEMPLATE(CreateStrided, 2)->Arg(1024*100);
BENCHMARK_TEMPLATE(CreateStrided, 4)->Arg(1024*100);
BENCHMARK_TEMPLATE(CreateStrided, 8)->Arg(1024*100);


BENCHMARK_TEMPLATE(AccessDataOriented, DataOrientedElement1Field )->Arg(1024*100);
BENCHMARK_TEMPLATE(AccessDataOriented, DataOrientedElement2Fields)->Arg(1024*100);
BENCHMARK_TEMPLATE(AccessDataOriented, DataOrientedElement3Fields)->Arg(1024*100);
BENCHMARK_TEMPLATE(AccessDataOriented, DataOrientedElement4Fields)->Arg(1024*100);
BENCHMARK_TEMPLATE(AccessDataOriented, DataOrientedElement8Fields)->Arg(1024*100);


BENCHMARK_MAIN()
//was:
//- create = 528k - 577k
//- access =  69k -  71k
