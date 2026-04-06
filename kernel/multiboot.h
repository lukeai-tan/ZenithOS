#pragma once
#include <stdint.h>

struct multiboot_info {
    uint32_t flags;
    uint32_t mem_lower;    // KB of lower memory
    uint32_t mem_upper;    // KB of upper memory
};
