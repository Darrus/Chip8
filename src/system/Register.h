#pragma once

#include "SystemInfo.h"

namespace Chip8System
{
    struct Register
    {
        unsigned char V[CHIP_8_REGISTER_MAX];
        unsigned short I;  // Index register
        unsigned short PC; // Program Counter

        unsigned short stack[CHIP_8_STACK_MAX];
        unsigned short SP; // Stack pointer
    };
}