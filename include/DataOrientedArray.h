//#pragma once

//#include "ContiguousPoolAllocator.h"
//#include "ContiguousPoolDeallocator.h"

//template<size_t FieldNr, typename T>
//class DataOrientedArray {
//private:
//    T * const element;
//    static /*thread_local*/ ContiguousPoolAllocator<T> allocator;
//    static /*thread_local*/ ContiguousPoolDeallocator<T> deallocator;
//public:
//    template<typename ... Args>
//    _always_inline
//    DataOrientedArray(Args&& ... args):
//        element(allocator.New(std::forward<Args>(args)...))
//    {}

//    _always_inline ~DataOrientedArray() {
//        deallocator.Delete(element);
//    }

//    _always_inline T& operator=(const T& value) {
//        *element = value;
//        return *element;
//    }

//    _always_inline T* operator&() {
//        return element;
//    }

//    _always_inline T* operator*() {
//        return element;
//    }
//};

//template<size_t FieldNr, typename T>
///*thread_local*/ ContiguousPoolAllocator<T> DataOrientedArray<FieldNr, T>::allocator;

//template<size_t FieldNr, typename T>
///*thread_local*/ ContiguousPoolDeallocator<T> DataOrientedArray<FieldNr, T>::deallocator;
