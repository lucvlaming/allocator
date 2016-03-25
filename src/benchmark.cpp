#include <benchmark/benchmark.h>

#include "Common.h"
#include "Parent.h"
#include "Field.h"

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

//template<typename T>
//static void CreateDataOriented(benchmark::State& state) {
//    do {
//        std::vector<T> storage;
//        storage.reserve(state.range_x());

//        state.ResumeTiming();
//        for (int i=0; i<state.range_x(); ++i) {
//            storage.emplace_back(i);
//        }
//        storage.clear();
//        state.PauseTiming();
//        escape(&storage);
//    } while (state.KeepRunning());
//}

//template<size_t Stride>
//static void CreateStridedUsingAllocator(benchmark::State& state) {
//    typedef StridedElement<Stride> Elem;
//    ContiguousPoolAllocator<Elem> allocator;
//    ContiguousPoolDeallocator<Elem> deallocator;
//    do {
//        std::vector<Elem*> storage;
//        storage.reserve(state.range_x());

//        state.ResumeTiming();
//        for (int i=0; i<state.range_x(); ++i) {
//            storage.emplace_back(allocator.New(i));
//        }
//        for (int i=0; i<state.range_x(); ++i) {
//            deallocator.Delete(storage[i]);
//        }
//        state.PauseTiming();
//        escape(&storage);
//    } while (state.KeepRunning());
//}

//template<size_t Stride>
//static void CreateStridedUsingNew(benchmark::State& state) {
//    typedef StridedElement<Stride> Elem;
//    do {
//        std::vector<Elem*> storage;
//        storage.reserve(state.range_x());

//        state.ResumeTiming();
//        for (int i=0; i<state.range_x(); ++i) {
//            storage.emplace_back(new Elem(i));
//        }
//        for (int i=0; i<state.range_x(); ++i) {
//            delete storage[i];
//        }
//        state.PauseTiming();

//        escape(&storage);
//    } while (state.KeepRunning());
//}

template<typename T>
static void AccessDataOriented(benchmark::State& state) {
    do {
        std::vector<T*> storage;
        storage.reserve(state.range_x());
        for (int i=0; i<state.range_x(); ++i) {
            storage.emplace_back(new T(i));
        }

        state.ResumeTiming();
        for (int i=0; i<state.range_x(); ++i) {
            storage[i]->first = 5;
        }
        state.PauseTiming();

        escape(&storage);

        for (int i=0; i<state.range_x(); ++i) {
            delete storage[i];
        }
        storage.clear();
    } while (state.KeepRunning());
}

//BENCHMARK_TEMPLATE(CreateDataOriented, DataOrientedElement1Field)->Arg(10240);
//BENCHMARK_TEMPLATE(CreateDataOriented, DataOrientedElement2Fields)->Arg(10240);
//BENCHMARK_TEMPLATE(CreateDataOriented, DataOrientedElement3Fields)->Arg(10240);
//BENCHMARK_TEMPLATE(CreateDataOriented, DataOrientedElement4Fields)->Arg(10240);
//BENCHMARK_TEMPLATE(CreateDataOriented, DataOrientedElement8Fields)->Arg(10240);
//BENCHMARK_TEMPLATE(CreateStridedUsingAllocator, 1)->Arg(10240);
//BENCHMARK_TEMPLATE(CreateStridedUsingAllocator, 2)->Arg(10240);
//BENCHMARK_TEMPLATE(CreateStridedUsingAllocator, 4)->Arg(10240);
//BENCHMARK_TEMPLATE(CreateStridedUsingAllocator, 8)->Arg(10240);
//BENCHMARK_TEMPLATE(CreateStridedUsingNew, 1)->Arg(10240);
//BENCHMARK_TEMPLATE(CreateStridedUsingNew, 2)->Arg(10240);
//BENCHMARK_TEMPLATE(CreateStridedUsingNew, 4)->Arg(10240);
//BENCHMARK_TEMPLATE(CreateStridedUsingNew, 8)->Arg(10240);


BENCHMARK_TEMPLATE(AccessDataOriented, DataOrientedElement1Field )->Arg(10);
//BENCHMARK_TEMPLATE(AccessDataOriented, DataOrientedElement2Fields)->Arg(1024*100);
//BENCHMARK_TEMPLATE(AccessDataOriented, DataOrientedElement3Fields)->Arg(1024*100);
//BENCHMARK_TEMPLATE(AccessDataOriented, DataOrientedElement4Fields)->Arg(1024*100);
//BENCHMARK_TEMPLATE(AccessDataOriented, DataOrientedElement8Fields)->Arg(1024*100);

BENCHMARK_MAIN()
