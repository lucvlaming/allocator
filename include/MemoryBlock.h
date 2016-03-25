#pragma once

#include <atomic>

struct MemoryBlock {
    MemoryBlock(): Alive(-1) {}
    std::atomic<int_fast32_t> Alive;
};
