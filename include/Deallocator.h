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
        LastBlock->~MemoryBlock();
        free(LastBlock);
    }

    __always_inline void Delete(void* ptr) {
        auto* thisBlock = Block(ptr);
        if (EXPECT_FALSE(thisBlock != LastBlock)) {
            if (LastBlock) {
                //could not allocate; start next block; add elements and cleanup
                LastBlock->Alive -= ElementsDeleted;
                if (!LastBlock->Alive) {
                    LastBlock->~MemoryBlock();
                    free(LastBlock);
                }
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
