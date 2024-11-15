#pragma once

#include "SystemInfo.h"

namespace Chip8System
{
    struct Register
    {
        unsigned char V[CHIP_8_REGISTER_MAX] = {0};
        unsigned short I = 0;  // Index register
        unsigned short PC = 0; // Program Counter

        unsigned short stack[CHIP_8_STACK_MAX] = {0};
        unsigned short SP = 0; // Stack pointer
    };
}