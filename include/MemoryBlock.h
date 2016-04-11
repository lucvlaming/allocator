#pragma once

#include "Common.h"

#include <atomic>

class MemoryBlock {
public:
    MemoryBlock() noexcept: Alive(-1) {}

private:
    friend void ChangeMemoryBlockCount(MemoryBlock&, int_fast32_t) noexcept;
    std::atomic<int_fast32_t> Alive;
};

void ChangeMemoryBlockCount(MemoryBlock& block, int_fast32_t difference) noexcept {
    block.Alive += difference;
    //TODO: check if correct wrt multi threading
    if (!block.Alive) {
        free(&block);
    }
}

