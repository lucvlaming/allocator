#pragma once

#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#include "Common.h"
#include "Allocator.h"
#include "Deallocator.h"

template<typename T>
class Parent {
private:
    #define MaxMembers 64

    static constexpr uint_fast32_t CalculateTotalStructSize() {
        uint_fast32_t total = 0;
        #define AddMember(z, n, name) total += BOOST_PP_CAT(T::name, n);
        BOOST_PP_REPEAT(MaxMembers, AddMember, FieldSize);
        #undef AddMember
        return total;
    }

    static constexpr uint_fast32_t CalculateFieldCount() {
        uint_fast32_t count = 0;
        #define CountFieldUsed(z, n, name) if (BOOST_PP_CAT(T::name, n) > 0) ++count;
        BOOST_PP_REPEAT(MaxMembers, CountFieldUsed, FieldSize);
        #undef CountFieldUsed
        return count;
    }

    static constexpr uint_fast32_t FieldAlignment = 16;
    static constexpr uint_fast32_t TotalStructSize = CalculateTotalStructSize();
    static constexpr uint_fast32_t FieldCount = CalculateFieldCount();
    static constexpr uint_fast32_t ReservedSpace = FieldAlignment > sizeof(MemoryBlock) ? FieldAlignment : sizeof(MemoryBlock);
    static constexpr uint_fast32_t BlockSize = FieldCount * (1<<20); //1MB per field, based on 1-2MB LLC per core on average on newer generations
    static constexpr uint_fast32_t BlockMask = ~(BlockSize - 1);
    static constexpr uint_fast32_t UsableSpace = BlockSize - ReservedSpace;
    static constexpr uint_fast32_t ElementCount = uint_fast32_t(UsableSpace / TotalStructSize / FieldAlignment) * FieldAlignment;

    //helper functions for Field<>
    #define SwitchCase(z, n, name) case n: return BOOST_PP_CAT(T::name, n);
    static constexpr uint_fast32_t FieldOffset(uint_fast32_t index) {
        switch(index) {
        BOOST_PP_REPEAT(MaxMembers, SwitchCase, FieldOffset);
        }
        return 0;
    }

    static constexpr uint_fast32_t FieldSize(uint_fast32_t index) {
        switch(index) {
        BOOST_PP_REPEAT(MaxMembers, SwitchCase, FieldSize);
        }
        return 0;
    }
    #undef SwitchCase

    static constexpr uint_fast32_t BlockOffset(uint_fast32_t index) {
        return FieldOffset(index) * ElementCount + ReservedSpace;
    }

    template<uint_fast32_t, typename, typename> friend class Field;

    char MakeEmpty[0];

public:
    //typedefs for Field()
    typedef T ParentType;
    #define InitialConst(z, n, name) static constexpr uint_fast32_t BOOST_PP_CAT(name, n) = 0;
    BOOST_PP_REPEAT(MaxMembers, InitialConst, FieldSize)
    BOOST_PP_REPEAT(MaxMembers, InitialConst, FieldOffset)
    #undef InitialConst

    #undef MaxMembers

    //allocators

    __always_inline void* operator new(size_t) {
        static Allocator<BlockSize, BlockMask, ElementCount> allocator;
        static_assert(sizeof(T) == sizeof(Parent<T>), "DOD struct should only contain Field() fields.");
        return allocator.New();
    }

    __always_inline void* operator new[](size_t) {
//        std::cout << "New 2" << std::endl;
        return 0;
    }

    __always_inline void operator delete(void* ptr) {
        static Deallocator<BlockMask> deallocator;
        deallocator.Delete(ptr);
    }
    __always_inline void operator delete[](void* ptr) {
//        std::cout << "Delete 2" << std::endl;
    }
};
