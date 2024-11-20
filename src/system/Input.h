#pragma once

#include "SystemInfo.h"

namespace Chip8System
{
    struct Input
    {
        uint8_t key[CHIP_8_INPUT_MAX];
    };
}