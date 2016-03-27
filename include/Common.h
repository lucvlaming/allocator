#pragma once

#include <cstdint>
#include <memory>

#define _always_inline __attribute__((always_inline))
#define EXPECT_TRUE(x) __builtin_expect(!!(x), 1)
#define EXPECT_FALSE(x) __builtin_expect(!!(x), 0)
