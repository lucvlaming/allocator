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
    
    __always_inline void* NewArray(size_t count) noexcept {
        if (ElementsLeft < count) {
            if (CurrAddr) {
                ChangeMemoryBlockCount(*Block(), 1 + (ElementCount - ElementsLeft));
            }
            if (ElementCount <= count) {
                //we have to allocate several blocks; allocate them at once, contiguously
                NewContiguousBlocks(count);
            } else {
                NewBlock();
//                ElementsLeft -= 
            }
        }
        return 0;
    }

private:
    __always_inline void NewBlock() noexcept {
        CurrAddr = reinterpret_cast<char*>(aligned_alloc(BlockSize, BlockSize));
        ElementsLeft = ElementCount;
        new (Block()) MemoryBlock();
    }
    
    __always_inline void NewContiguousBlocks(size_t count) noexcept {
        auto blockCount = count / ElementCount;
        auto StartAddr = reinterpret_cast<char*>(aligned_alloc(BlockSize, BlockSize * blockCount));
        CurrAddr = StartAddr;
        ElementsLeft = count % ElementCount;
        for (size_t i=1; i<blockCount; ++i) {
            new (Block()) MemoryBlock();
            ChangeMemoryBlockCount(*Block(), 1 + ElementCount);
            CurrAddr += BlockSize;
        }
    }

    __always_inline MemoryBlock* Block() noexcept {
        return reinterpret_cast<MemoryBlock*>(reinterpret_cast<size_t>(CurrAddr) & BlockMask);
    }

    char* CurrAddr;
    uint_fast32_t ElementsLeft;
};
