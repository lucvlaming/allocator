#pragma once

#include "Common.h"
#include "MemoryBlock.h"

template<uint_fast32_t BlockSize, uint_fast32_t BlockMask, uint_fast32_t ElementCount>
class Allocator {
public:
    Allocator() noexcept:
        CurrAddr(0),
        ElementsLeft(0)
    {}

    ~Allocator() noexcept {
        if (CurrAddr) {
            ChangeMemoryBlockCount(*Block(), 1 + (ElementCount - ElementsLeft));
        }
    }

    __always_inline void* New() noexcept {
        if (EXPECT_FALSE(!ElementsLeft)) {
            //block empty; start next block; add elements and cleanup
            if (CurrAddr) {
                ChangeMemoryBlockCount(*Block(), 1 + (ElementCount - ElementsLeft));
            }
            NewBlock();
        }
        --ElementsLeft;
        return CurrAddr++;
    }

private:
    __always_inline void NewBlock() noexcept {
        CurrAddr = reinterpret_cast<char*>(aligned_alloc(BlockSize, BlockSize));
        ElementsLeft = ElementCount;
        new (Block()) MemoryBlock();
    }

    __always_inline MemoryBlock* Block() noexcept {
        return reinterpret_cast<MemoryBlock*>(reinterpret_cast<size_t>(CurrAddr) & BlockMask);
    }

    char* CurrAddr;
    uint_fast32_t ElementsLeft;
};
