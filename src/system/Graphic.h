#pragma once

#include "SystemInfo.h"

namespace Chip8System
{
    class Graphic
    {
        unsigned char gfx[CHIP_8_SCREEN_WIDTH * CHIP_8_SCREEN_HEIGHT];
    };
}