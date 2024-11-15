#pragma once

#include "SystemInfo.h"

namespace Chip8System
{
    struct Graphic
    {
        // Each unsigned char represents 1 bit
        // But in C++ the smallest allocatable size is 1 byte
        unsigned char gfx[CHIP_8_SCREEN_WIDTH * CHIP_8_SCREEN_HEIGHT] = {0};
    };
}