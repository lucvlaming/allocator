#include "Common.h"
#include "Parent.h"
#include "Field.h"

#include <benchmark/benchmark.h>
#include <iostream>

static void escape(const void* p) {
    asm volatile ( "" : : "g" (const_cast<void*>(p)) : "memory" );
}

//TODO: mark functions no exceptions


struct DataOrientedElement1Field: Parent<DataOrientedElement1Field, 1*4> {
    DataOrientedElement1Field(int32_t f): first(f) {}
    Field(1, int32_t) first;
};

struct DataOrientedElement2Fields: Parent<DataOrientedElement2Fields, 2*4> {
    DataOrientedElement2Fields(int32_t f): first(f) {}
    Field(1, int32_t) first;
    Field(2, int32_t) second;
};

struct DataOrientedElement3Fields: Parent<DataOrientedElement3Fields, 3*4> {
    DataOrientedElement3Fields(int32_t f): first(f) {}
    Field(1, int32_t) first;
    Field(2, int32_t) second;
    Field(3, int32_t) third;
};

struct DataOrientedElement4Fields: Parent<DataOrientedElement4Fields, 4*4> {
    DataOrientedElement4Fields(int32_t f): first(f) {}
    Field(1, int32_t) first;
    Field(2, int32_t) second;
    Field(3, int32_t) third;
    Field(4, int32_t) fourth;
};

struct DataOrientedElement8Fields: Parent<DataOrientedElement8Fields, 8*4> {
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

struct DataOrientedElement16Fields: Parent<DataOrientedElement16Fields, 16*4> {
    DataOrientedElement16Fields(int32_t f): first(f) {}
    Field(1, int32_t) first;
    Field(2, int32_t) second;
    Field(3, int32_t) third;
    Field(4, int32_t) fourth;
    Field(5, int32_t) fifth;
    Field(6, int32_t) sixth;
    Field(7, int32_t) seventh;
    Field(8, int32_t) eigth;
    Field(9, int32_t) ninth;
    Field(10, int32_t) tenth;
    Field(11, int32_t) eleventh;
    Field(12, int32_t) twelfth;
    Field(13, int32_t) thirteenth;
    Field(14, int32_t) fourteenth;
    Field(15, int32_t) fifteenth;
    Field(16, int32_t) sixteenth;
};

struct DataOrientedElement32Fields: Parent<DataOrientedElement32Fields, 32*4> {
    DataOrientedElement32Fields(int32_t f): first(f) {}
    Field(1, int32_t) first;
    Field(2, int32_t) second;
    Field(3, int32_t) third;
    Field(4, int32_t) fourth;
    Field(5, int32_t) fifth;
    Field(6, int32_t) sixth;
    Field(7, int32_t) seventh;
    Field(8, int32_t) eigth;
    Field(9, int32_t) ninth;
    Field(10, int32_t) tenth;
    Field(11, int32_t) eleventh;
    Field(12, int32_t) twelfth;
    Field(13, int32_t) thirteenth;
    Field(14, int32_t) fourteenth;
    Field(15, int32_t) fifteenth;
    Field(16, int32_t) sixteenth;
    Field(17, int32_t) f17;
    Field(18, int32_t) f18;
    Field(19, int32_t) a19;
    Field(20, int32_t) a20;
    Field(21, int32_t) a21;
    Field(22, int32_t) a22;
    Field(23, int32_t) a23;
    Field(24, int32_t) a24;
    Field(25, int32_t) a25;
    Field(26, int32_t) a26;
    Field(27, int32_t) a27;
    Field(28, int32_t) a28;
    Field(29, int32_t) a29;
    Field(30, int32_t) a30;
    Field(31, int32_t) a31;
    Field(32, int32_t) a32;
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
    typedef StridedElement<Stride-1> Elem;

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

static void SetUsingMemset(benchmark::State& state) {
    std::vector<int_fast32_t> storage;
    storage.resize(state.range_x());

    while (state.KeepRunning()) {
        __builtin_memset(&storage[0], 5, state.range_x() * sizeof(int_fast32_t));
    }
}

template<size_t Stride>
static void SetUsingArray(benchmark::State& state) {
    std::vector<int_fast32_t> storage;
    storage.resize(state.range_x() * Stride);

    while (state.KeepRunning()) {
        for (int i=0; i<state.range_x(); ++i) {
            storage[i*Stride] = 5;
        }
    }
}

template<typename T>
static void SetUsingDataOrientedPattern(benchmark::State& state) {
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

template<size_t Stride>
static void SetUsingNew(benchmark::State& state) {
    typedef StridedElement<Stride-1> Elem;

    std::vector<Elem*> storage;
    storage.reserve(state.range_x());

    for (int i=0; i<state.range_x(); ++i) {
        storage.emplace_back(new Elem(i));
    }

    while (state.KeepRunning()) {
        for (int i=0; i<state.range_x(); ++i) {
            storage[i]->first = 5;
        }        
    }

    for (int i=0; i<state.range_x(); ++i) {
        delete storage[i];
    }
}


BENCHMARK_TEMPLATE(CreateDataOriented, DataOrientedElement1Field )->Arg(1024*100);
BENCHMARK_TEMPLATE(CreateDataOriented, DataOrientedElement2Fields)->Arg(1024*100);
BENCHMARK_TEMPLATE(CreateDataOriented, DataOrientedElement3Fields)->Arg(1024*100);
BENCHMARK_TEMPLATE(CreateDataOriented, DataOrientedElement4Fields)->Arg(1024*100);
BENCHMARK_TEMPLATE(CreateDataOriented, DataOrientedElement8Fields)->Arg(1024*100);
BENCHMARK_TEMPLATE(CreateStrided, 1)->Arg(1024*100);
BENCHMARK_TEMPLATE(CreateStrided, 2)->Arg(1024*100);
BENCHMARK_TEMPLATE(CreateStrided, 3)->Arg(1024*100);
BENCHMARK_TEMPLATE(CreateStrided, 4)->Arg(1024*100);
BENCHMARK_TEMPLATE(CreateStrided, 8)->Arg(1024*100);


BENCHMARK_TEMPLATE(SetUsingDataOrientedPattern, DataOrientedElement1Field )->Arg(1024*100);
BENCHMARK_TEMPLATE(SetUsingDataOrientedPattern, DataOrientedElement2Fields)->Arg(1024*100);
BENCHMARK_TEMPLATE(SetUsingDataOrientedPattern, DataOrientedElement3Fields)->Arg(1024*100);
BENCHMARK_TEMPLATE(SetUsingDataOrientedPattern, DataOrientedElement4Fields)->Arg(1024*100);
BENCHMARK_TEMPLATE(SetUsingDataOrientedPattern, DataOrientedElement8Fields)->Arg(1024*100);
BENCHMARK_TEMPLATE(SetUsingDataOrientedPattern, DataOrientedElement16Fields)->Arg(1024*100);
BENCHMARK_TEMPLATE(SetUsingDataOrientedPattern, DataOrientedElement32Fields)->Arg(1024*100);

BENCHMARK(SetUsingMemset)->Arg(1024*100);

BENCHMARK_TEMPLATE(SetUsingArray, 1)->Arg(1024*100);
BENCHMARK_TEMPLATE(SetUsingArray, 2)->Arg(1024*100);
BENCHMARK_TEMPLATE(SetUsingArray, 3)->Arg(1024*100);
BENCHMARK_TEMPLATE(SetUsingArray, 4)->Arg(1024*100);
BENCHMARK_TEMPLATE(SetUsingArray, 8)->Arg(1024*100);
BENCHMARK_TEMPLATE(SetUsingArray, 16)->Arg(1024*100);
BENCHMARK_TEMPLATE(SetUsingArray, 32)->Arg(1024*100);

BENCHMARK_TEMPLATE(SetUsingNew, 1)->Arg(1024*100);
BENCHMARK_TEMPLATE(SetUsingNew, 2)->Arg(1024*100);
BENCHMARK_TEMPLATE(SetUsingNew, 3)->Arg(1024*100);
BENCHMARK_TEMPLATE(SetUsingNew, 4)->Arg(1024*100);
BENCHMARK_TEMPLATE(SetUsingNew, 8)->Arg(1024*100);
BENCHMARK_TEMPLATE(SetUsingNew, 16)->Arg(1024*100);
BENCHMARK_TEMPLATE(SetUsingNew, 32)->Arg(1024*100);

BENCHMARK_MAIN()
//base (without any computation) = 51k - 52k
//basic computation = 60k-62k
