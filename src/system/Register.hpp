#pragma once

#include "SystemInfo.hpp"

namespace Chip8System
{
    struct Register
    {
        uint8_t V[CHIP_8_REGISTER_MAX] = {0};
        uint16_t I = 0;  // Index register
        uint16_t PC = 0; // Program Counter

        uint16_t stack[CHIP_8_STACK_MAX] = {0};
        uint16_t SP = 0; // Stack pointer
    };
}