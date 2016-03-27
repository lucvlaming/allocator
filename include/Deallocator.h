#pragma once

#include "Common.h"
#include "MemoryBlock.h"

template<uint_fast32_t BlockMask>
class Deallocator {
public:
    Deallocator():
        LastBlock(0),
        ElementsDeleted(0)
    {}

    ~Deallocator() {
        if (LastBlock) {
            ChangeMemoryBlockCount(*LastBlock, -ElementsDeleted);
        }
    }

    __always_inline void Delete(void* ptr) {
        auto* thisBlock = Block(ptr);
        if (EXPECT_FALSE(thisBlock != LastBlock)) {
            if (LastBlock) {
                //block changed and we had a previous block, change element count
                ChangeMemoryBlockCount(*LastBlock, -ElementsDeleted);
            }
            ElementsDeleted = 0;
            LastBlock = thisBlock;
        }
        ElementsDeleted++;
    }

private:
    __always_inline MemoryBlock* Block(void* ptr) {
        return reinterpret_cast<MemoryBlock*>(reinterpret_cast<size_t>(ptr) & BlockMask);
    }

    MemoryBlock* LastBlock;
    int_fast32_t ElementsDeleted;
};
