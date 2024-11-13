#pragma once

#include "SystemInfo.h"

namespace Chip8System
{
    struct Input
    {
        unsigned char key[CHIP_8_INPUT_MAX];
    };
}