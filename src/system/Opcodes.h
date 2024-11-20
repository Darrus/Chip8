#pragma once

#include <cstdint>
#include "Chip8.h"

namespace Chip8System
{
    class Opcodes
    {
        // typedef void (*Operation)(Chip8 &, uint16_t);
    public:
        static void _0NNN(Chip8 &system, uint16_t opcode);
        static void _00E0(Chip8 &system, uint16_t opcode);
        static void _00EE(Chip8 &system, uint16_t opcode);
        static void _1NNN(Chip8 &system, uint16_t opcode);
        static void _2NNN(Chip8 &system, uint16_t opcode);
        static void _3XNN(Chip8 &system, uint16_t opcode);
        static void _4XNN(Chip8 &system, uint16_t opcode);
        static void _5XY0(Chip8 &system, uint16_t opcode);
        static void _6XNN(Chip8 &system, uint16_t opcode);
        static void _7XNN(Chip8 &system, uint16_t opcode);
        static void _8XY0(Chip8 &system, uint16_t opcode);
        static void _8XY1(Chip8 &system, uint16_t opcode);
        static void _8XY2(Chip8 &system, uint16_t opcode);
        static void _8XY3(Chip8 &system, uint16_t opcode);
        static void _8XY4(Chip8 &system, uint16_t opcode);
        static void _8XY5(Chip8 &system, uint16_t opcode);
        static void _8XY6(Chip8 &system, uint16_t opcode);
        static void _8XY7(Chip8 &system, uint16_t opcode);
        static void _8XYE(Chip8 &system, uint16_t opcode);
        static void _9XY0(Chip8 &system, uint16_t opcode);
        static void _ANNN(Chip8 &system, uint16_t opcode);
        static void _BNNN(Chip8 &system, uint16_t opcode);
        static void _CXNN(Chip8 &system, uint16_t opcode);
        static void _DXYN(Chip8 &system, uint16_t opcode);
        static void _EX9E(Chip8 &system, uint16_t opcode);
        static void _EXA1(Chip8 &system, uint16_t opcode);
        static void _FX07(Chip8 &system, uint16_t opcode);
        static void _FX0A(Chip8 &system, uint16_t opcode);
        static void _FX15(Chip8 &system, uint16_t opcode);
        static void _FX18(Chip8 &system, uint16_t opcode);
        static void _FX1E(Chip8 &system, uint16_t opcode);
        static void _FX29(Chip8 &system, uint16_t opcode);
        static void _FX33(Chip8 &system, uint16_t opcode);
        static void _FX55(Chip8 &system, uint16_t opcode);
        static void _FX65(Chip8 &system, uint16_t opcode);

    private:
        static uint8_t X(uint16_t opcode);
        static uint8_t Y(uint16_t opcode);
    };
}