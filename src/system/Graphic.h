#pragma once

#include "SystemInfo.h"

namespace Chip8System
{
    struct Graphic
    {
        // Each uint8_t represents 1 bit
        // But in C++ the smallest allocatable size is 1 byte
        uint8_t gfx[CHIP_8_SCREEN_WIDTH * CHIP_8_SCREEN_HEIGHT] = {0};
    };
}