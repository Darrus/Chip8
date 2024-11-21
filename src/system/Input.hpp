#pragma once

#include "SystemInfo.hpp"

namespace Chip8System
{
    struct Input
    {
        uint8_t key[CHIP_8_INPUT_MAX];
    };
}