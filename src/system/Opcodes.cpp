#include <random>

#include "Opcodes.hpp"
#include "../KeyState.hpp"
#include "SystemInfo.hpp"

#define BIT_4 4
#define BIT_8 8

namespace Chip8System
{
    void Opcodes::_0NNN(Chip8 &system, uint16_t opcode)
    {
        system.Registers.PC = opcode & 0x0FFF;
    }

    void Opcodes::_00E0(Chip8 &system, uint16_t opcode)
    {
        // Clear screen
        for (int i = 0; i < CHIP_8_SCREEN_WIDTH * CHIP_8_SCREEN_HEIGHT; ++i)
        {
            system.Graphic.gfx[i] = 0;
        }
    }

    void Opcodes::_00EE(Chip8 &system, uint16_t opcode)
    {
        --system.Registers.SP; // Pop the stack
        system.Registers.PC = system.Registers.stack[system.Registers.SP];
    }

    void Opcodes::_1NNN(Chip8 &system, uint16_t opcode)
    {
        system.Registers.PC = opcode & 0x0FFF;
    }

    void Opcodes::_2NNN(Chip8 &system, uint16_t opcode)
    {
        system.Registers.stack[system.Registers.SP] = system.Registers.PC;
        ++system.Registers.SP; // Push to stack
        system.Registers.PC = opcode & 0x0FFF;
    }

    void Opcodes::_3XNN(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);
        if (system.Registers.V[x] == (opcode & 0x00FF))
        {
            system.Registers.PC += 2;
        }
    }

    void Opcodes::_4XNN(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);
        if (system.Registers.V[x] != (opcode & 0x00FF))
        {
            system.Registers.PC += 2;
        }
    }

    void Opcodes::_5XY0(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);
        uint8_t y = Y(opcode);
        if (system.Registers.V[x] == system.Registers.V[y])
        {
            system.Registers.PC += 2;
        }
    }

    void Opcodes::_6XNN(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);
        system.Registers.V[x] = opcode & 0x00FF;
    }

    void Opcodes::_7XNN(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);
        system.Registers.V[x] += opcode & 0x00FF;
    }

    void Opcodes::_8XY0(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);
        uint8_t y = Y(opcode);
        system.Registers.V[x] = system.Registers.V[y];
    }

    void Opcodes::_8XY1(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);
        uint8_t y = Y(opcode);
        system.Registers.V[x] |= system.Registers.V[y];
    }

    void Opcodes::_8XY2(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);
        uint8_t y = Y(opcode);
        system.Registers.V[x] &= system.Registers.V[y];
    }

    void Opcodes::_8XY3(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);
        uint8_t y = Y(opcode);
        system.Registers.V[x] ^= system.Registers.V[y];
    }

    void Opcodes::_8XY4(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);
        uint8_t y = Y(opcode);

        uint16_t sum = system.Registers.V[x] + system.Registers.V[y];
        system.Registers.V[x] = sum & 0xFF;
        if (sum > 0xFF)
        {
            system.Registers.V[0xF] = 1;
        }
        else
        {
            system.Registers.V[0xF] = 0;
        }
    }

    void Opcodes::_8XY5(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);
        uint8_t y = Y(opcode);

        uint8_t xValue = system.Registers.V[x];
        uint8_t yValue = system.Registers.V[y];
        system.Registers.V[x] = xValue - yValue;
        if (xValue >= yValue)
        {
            system.Registers.V[0xF] = 1;
        }
        else
        {
            system.Registers.V[0xF] = 0;
        }
    }

    void Opcodes::_8XY6(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);
        uint8_t y = Y(opcode);

        uint16_t xLSB = system.Registers.V[x] & 1;
        system.Registers.V[x] >>= 1;
        system.Registers.V[0xF] = xLSB;
    }

    void Opcodes::_8XY7(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);
        uint8_t y = Y(opcode);

        uint8_t xValue = system.Registers.V[x];
        uint8_t yValue = system.Registers.V[y];
        system.Registers.V[x] = yValue - xValue;
        if (yValue >= xValue)
        {
            system.Registers.V[0xF] = 1;
        }
        else
        {
            system.Registers.V[0xF] = 0;
        }
    }

    void Opcodes::_8XYE(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);

        uint8_t xMSB = system.Registers.V[x] >> 7;
        system.Registers.V[x] <<= 1;
        system.Registers.V[0xF] = xMSB;
    }

    void Opcodes::_9XY0(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);
        uint8_t y = Y(opcode);

        if (system.Registers.V[x] != system.Registers.V[y])
        {
            system.Registers.PC += 2;
        }
    }

    void Opcodes::_ANNN(Chip8 &system, uint16_t opcode)
    {
        system.Registers.I = opcode & 0x0FFF;
    }

    void Opcodes::_BNNN(Chip8 &system, uint16_t opcode)
    {
        system.Registers.PC = (opcode & 0x0FFF) + system.Registers.V[0];
    }

    void Opcodes::_CXNN(Chip8 &system, uint16_t opcode)
    {
        uint16_t x = X(opcode);
        system.Registers.V[x] = (rand() % 256) & (opcode & 0x00FF);
    }

    void Opcodes::_DXYN(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);
        uint8_t y = Y(opcode);

        uint16_t xCoord = system.Registers.V[x];
        uint16_t yCoord = system.Registers.V[y];
        uint16_t spriteHeight = opcode & 0xF;

        system.Registers.V[0xF] = 0;
        for (int row = 0; row < spriteHeight; ++row)
        {
            // Convert x and y to 1 dimensional coordinate
            uint16_t coord1D = xCoord + ((yCoord + row) * CHIP_8_SCREEN_WIDTH);
            uint16_t spriteRow = system.Memory[system.Registers.I + row];

            for (int pixelX = 0; pixelX < 8; ++pixelX)
            {
                // Pixel is active
                if ((spriteRow & (0x80 >> pixelX)) != 0)
                {
                    // Graphic's Pixel is also active
                    if (system.Graphic.gfx[coord1D + pixelX] == 1)
                    {
                        system.Registers.V[0xF] = 1;
                    }

                    system.Graphic.gfx[coord1D + pixelX] ^= 1;
                }
            }
        }
    }

    void Opcodes::_EX9E(Chip8 &system, uint16_t opcode)
    {
        // Skips instruction if key in VX is pressed
        uint8_t x = X(opcode);
        uint16_t key = system.Registers.V[x];
        if (system.Input.key[key] == Core::KEY_STATE::KEY_DOWN)
        {
            system.Registers.PC += 2;
        }
    }

    void Opcodes::_EXA1(Chip8 &system, uint16_t opcode)
    {
        // Skips instruction if key in VX is not pressed
        uint8_t x = X(opcode);
        uint16_t key = system.Registers.V[x];

        if (system.Input.key[key] != Core::KEY_STATE::KEY_DOWN)
        {
            system.Registers.PC += 2;
        }
    }

    void Opcodes::_FX07(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);
        system.Registers.V[x] = system.Timer.DelayTimer;
    }

    void Opcodes::_FX0A(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);
        bool isKeyPressed = false;
        for (int i = 0; i < CHIP_8_INPUT_MAX; ++i)
        {
            if (system.Input.key[i] == Core::KEY_STATE::KEY_DOWN)
            {
                system.Registers.V[x] = i;
                isKeyPressed = true;
            }
        }

        if (!isKeyPressed)
        {
            system.Registers.PC -= 2;
        }
    }

    void Opcodes::_FX15(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);
        system.Timer.DelayTimer = system.Registers.V[x];
    }

    void Opcodes::_FX18(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);
        system.Timer.SoundTimer = system.Registers.V[x];
    }

    void Opcodes::_FX1E(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);
        system.Registers.I += system.Registers.V[x];
    }

    void Opcodes::_FX29(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);
        uint16_t digit = system.Registers.V[x];
        system.Registers.I = CHIP_8_MEMORY_FONT_ADDRESS + (digit * 5);
    }

    void Opcodes::_FX33(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);
        uint8_t value = system.Registers.V[x];

        system.Memory[system.Registers.I + 2] = value % 10;
        value /= 10;

        system.Memory[system.Registers.I + 1] = value % 10;
        value /= 10;

        system.Memory[system.Registers.I] = value % 10;
    }

    void Opcodes::_FX55(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);
        for (int i = 0; i <= x; ++i)
        {
            system.Memory[system.Registers.I + i] = system.Registers.V[i];
        }
    }

    void Opcodes::_FX65(Chip8 &system, uint16_t opcode)
    {
        uint8_t x = X(opcode);
        for (int i = 0; i <= x; ++i)
        {
            system.Registers.V[i] = system.Memory[system.Registers.I + i];
        }
    }

    inline uint8_t Opcodes::X(uint16_t opcode)
    {
        return (opcode & 0x0F00) >> BIT_8;
    }

    inline uint8_t Opcodes::Y(uint16_t opcode)
    {
        return (opcode & 0x00F0) >> BIT_4;
    }
}