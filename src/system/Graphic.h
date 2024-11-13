#pragma once

#include "SystemInfo.h"

namespace Chip8System
{
    struct Graphic
    {
        unsigned char gfx[CHIP_8_SCREEN_WIDTH * CHIP_8_SCREEN_HEIGHT];
    };
}