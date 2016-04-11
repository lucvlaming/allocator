#pragma once

#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#include "Common.h"
#include "Allocator.h"
#include "Deallocator.h"

#define MaxMembers 64

#include <iostream>

//TODO: force alignment of this struct to be same as BlockSize so even though it might be allocated on the stack it still works
//__attribute__((__aligned__((1<<22)))) 
template<typename T>
class Parent {
private:
    static constexpr uint_fast32_t CalculateTotalStructSize() noexcept {
        uint_fast32_t total = 0;
        #define AddMember(z, n, name) total += BOOST_PP_CAT(T::name, n);
        BOOST_PP_REPEAT(MaxMembers, AddMember, FieldSize);
        #undef AddMember
        return total;
    }

    static constexpr uint_fast32_t CalculateFieldCount() noexcept {
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
    //1MB per field (on average), based on 1-2MB LLC per core on average on newer generations
    static constexpr uint_fast32_t FieldSpace = 1<<20;
    static constexpr uint_fast32_t BlockSize = FieldCount * FieldSpace;
    static constexpr uint_fast32_t ItemMask = BlockSize-1;
    static constexpr uint_fast32_t BlockMask = ~ItemMask;
    static constexpr uint_fast32_t UsableSpace = BlockSize - ReservedSpace;
    static constexpr uint_fast32_t ElementCount = uint_fast32_t(UsableSpace / TotalStructSize / FieldAlignment) * FieldAlignment;
    //itemmask should be calculated on elementcount for blockmask to work correctly
    static constexpr uint_fast32_t ItemBlockMask = ~ItemMask;
    static constexpr uint_fast32_t ItemBlockMaskMultiplier = BlockSize / (ItemMask+1);

    //helper functions for Field<>
    #define SwitchCase(z, n, name) case n: return BOOST_PP_CAT(T::name, n);
    static constexpr uint_fast32_t FieldOffset(uint_fast32_t index) noexcept {
        switch(index) {
        BOOST_PP_REPEAT(MaxMembers, SwitchCase, FieldOffset);
        }
        return 0;
    }

    static constexpr uint_fast32_t FieldSize(uint_fast32_t index) noexcept {
        switch(index) {
        BOOST_PP_REPEAT(MaxMembers, SwitchCase, FieldSize);
        }
        return 0;
    }
    #undef SwitchCase

    static constexpr uint_fast32_t FieldOffsetInBlock(uint_fast32_t fieldIndex) noexcept {
        return ReservedSpace + FieldOffset(fieldIndex) * ElementCount;
    }
    
    static constexpr void* FieldAddress(void* fieldInputAddress, uint_fast32_t fieldIndex) {
        auto addr      = reinterpret_cast<size_t>(fieldInputAddress) - sizeof(ForceSize);  //we have to adjust for parent size since this offsets the pointer
        auto base      = addr & Parent::BlockMask;
        auto itemIndex = (addr & Parent::ItemMask);

//        base += (itemIndex & ItemBlockMask) * ItemBlockMaskMultiplier;
//        itemIndex &= ItemMask;

//        std::cout << "base=" << (void*)base << " itemIndex=" << itemIndex << " fieldOffsetInBlock=" << FieldOffsetInBlock(fieldIndex) << " field-size=" << FieldSize(fieldIndex) << " " 
//                  << "ElementCount=" << ElementCount << " ItemMask=" << ItemMask << " ItemBlockMask=" << ItemBlockMask 
//                  << " ItemBlockMaskMultiplier=" << ItemBlockMaskMultiplier << " "
//                  << " base-addr-incr=" << (itemIndex & ItemBlockMask) * ItemBlockMaskMultiplier << " "
//                  << reinterpret_cast<void*>(base + FieldOffsetInBlock(fieldIndex)) << " "
//                  << reinterpret_cast<void*>(base + FieldOffsetInBlock(fieldIndex) + itemIndex * FieldSize(fieldIndex)) << std::endl;
        
        return reinterpret_cast<void*>(base + FieldOffsetInBlock(fieldIndex) + itemIndex * FieldSize(fieldIndex));
    }
    
    template<uint_fast32_t, typename, typename> friend class Field;

    //have it size 1 so that the offsets for a stl allocator also work
    char ForceSize[1];

public:
    //typedefs for Field()
    typedef T ParentType;
    #define InitialConst(z, n, name) static constexpr uint_fast32_t BOOST_PP_CAT(name, n) = 0;
    BOOST_PP_REPEAT(MaxMembers, InitialConst, FieldSize)
    BOOST_PP_REPEAT(MaxMembers, InitialConst, FieldOffset)
    #undef InitialConst

    #undef MaxMembers

    //allocators

    __always_inline void* operator new(size_t) noexcept {
        static thread_local Allocator<BlockSize, BlockMask, ElementCount> allocator;
        static_assert(sizeof(T) == sizeof(Parent<T>), "DOD struct should only contain Field() fields.");
        return allocator.New();
    }

    __always_inline void* operator new[](size_t size) noexcept {
        
//        std::cout << "New 2: " << size << std::endl;
        return 0;
    }

    __always_inline void operator delete(void* ptr) noexcept {
        static thread_local Deallocator<BlockMask> deallocator;
        deallocator.Delete(ptr);
    }
    __always_inline void operator delete[](void* ptr) noexcept {
//        std::cout << "Delete 2" << std::endl;
    }
};
