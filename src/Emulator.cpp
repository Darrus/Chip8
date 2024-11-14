#include <stdlib.h>
#include "Emulator.h"
#include "system/SystemInfo.h"

#define KEYSTATE_DOWN 0x01       // 0000 0001
#define KEYSTATE_RELEASED 0x8000 // 1000 0000 0000 0000

#define BIT_4 4
#define BIT_8 8

namespace Core
{
    Emulator::Emulator()
    {
        console.InitConsole(CHIP_8_SCREEN_WIDTH, CHIP_8_SCREEN_HEIGHT);
    }

    void Emulator::EmulateCycle()
    {
        system.opcode = system.Memory[system.Registers.PC] << BIT_8 | system.Memory[system.Registers.PC + 1];
        system.Registers.PC += 2;

        ExecuteOpcode(system.opcode);
        UpdateTimer();
    }

    void Emulator::Run()
    {
        while (true)
        {
            EmulateCycle();
            Render();

            if ((GetAsyncKeyState(VK_ESCAPE) & KEYSTATE_DOWN) != 0)
            {
                break;
            }
        }
    }

    void Emulator::ExecuteOpcode(unsigned short opcode)
    {
        switch (opcode & 0xF000)
        {
        case 0x0000:
            switch (opcode & 0x0EEE)
            {
            case 0x00E0:
                // Clear screen
                for (unsigned short i = 0; i < CHIP_8_SCREEN_WIDTH * CHIP_8_SCREEN_HEIGHT; ++i)
                {
                    system.Graphic.gfx[i] = 0;
                }
                break;
            case 0x00EE:
                --system.Registers.SP; // Pop the stack
                system.Registers.PC = system.Registers.stack[system.Registers.SP];
                break;
            default:
                // Calls machine code routine
            }
            break;
        case 0x1000:
            system.Registers.PC = opcode * 0x0FFF;
            break;
        case 0x2000:
            system.Registers.stack[system.Registers.SP] = system.Registers.PC;
            ++system.Registers.SP; // Push to stack
            system.Registers.PC = opcode & 0x0FFF;
            break;
        case 0x3000:
        {
            unsigned short x = (opcode & 0x0F00) >> BIT_8;
            if (system.Registers.V[x] == (opcode & 0x00FF))
            {
                system.Registers.PC += 2;
            }
            break;
        }
        case 0x4000:
        {
            unsigned short x = (opcode & 0x0F00) >> BIT_8;
            if (system.Registers.V[x] != (opcode & 0x00FF))
            {
                system.Registers.PC += 2;
            }
            break;
        }
        case 0x5000:
        {
            unsigned short x = (opcode & 0x0F00) >> BIT_8;
            unsigned short y = (opcode & 0x00F0) >> BIT_4;
            if (system.Registers.V[x] == system.Registers.V[y])
            {
                system.Registers.PC += 2;
            }
            break;
        }
        case 0x6000:
        {
            unsigned short x = (opcode & 0x0F00) >> BIT_8;
            system.Registers.V[x] = opcode & 0x00FF;
            break;
        }
        case 0x7000:
        {
            unsigned short x = (opcode & 0x0F00) >> BIT_8;
            system.Registers.V[x] += opcode & 0x00FF;
            break;
        }
        case 0x8000:
        {
            unsigned short x = (opcode & 0x0F00) >> BIT_8;
            unsigned short y = (opcode & 0x00F0) >> BIT_8;

            switch (opcode & 0x000F)
            {
            case 0x0000:
                system.Registers.V[x] = system.Registers.V[y];
                break;
            case 0x0001:
                system.Registers.V[x] |= system.Registers.V[y];
                break;
            case 0x0002:
                system.Registers.V[x] &= system.Registers.V[y];
                break;
            case 0x0003:
                system.Registers.V[x] ^= system.Registers.V[y];
                break;
            case 0x0004:
                system.Registers.V[x] += system.Registers.V[y];
                break;
            case 0x0005:
                system.Registers.V[x] -= system.Registers.V[y];
                break;
            case 0x0006:
                system.Registers.V[0xF] = system.Registers.V[x] & 1;
                system.Registers.V[0xF] >>= 1;
                break;
            case 0x0007:
                if (system.Registers.V[y] > system.Registers.V[x])
                {
                    system.Registers.V[0xF] = 1;
                }
                else
                {
                    system.Registers.V[0xF] = 0;
                }
                system.Registers.V[x] = system.Registers.V[y] - system.Registers.V[x];
                break;
            case 0x000E:
                system.Registers.V[0xF] = (opcode & 0x80) >> 7;
                system.Registers.V[x] <<= 1;
                break;
            }
        }
        break;
        case 0x9000:
        {
            unsigned short x = (opcode & 0x0F00) >> BIT_8;
            unsigned short y = (opcode & 0x00F0) >> BIT_4;

            if (system.Registers.V[x] != system.Registers.V[y])
            {
                system.Registers.PC += 2;
            }
        }
        break;
        case 0xA000:
            system.Registers.I = opcode & 0x0FFF;
            break;
        case 0xB000:
            system.Registers.PC = (opcode & 0x0FFF) + system.Registers.V[0];
            break;
        case 0xC000:
        {
            unsigned short x = opcode & 0x0F00 >> BIT_8;
            system.Registers.V[x] = (rand() % 256) & (opcode & 0x00FF);
        }
        break;
        case 0xD000:
        {
            unsigned short x = opcode & 0x0F00 >> BIT_8;
            unsigned short y = opcode & 0x00F0 >> BIT_4;

            unsigned short xCoord = system.Registers.V[x];
            unsigned short yCoord = system.Registers.V[y];
            unsigned short spriteHeight = opcode & 0xF;

            system.Registers.V[0xF] = 0;
            for (unsigned short row = 0; row < spriteHeight; ++row)
            {
                // Convert x and y to 1 dimensional coordinate
                unsigned short coord1D = x + ((y + row) * CHIP_8_SCREEN_WIDTH);
                unsigned short spriteRow = system.Memory[system.Registers.I + row];

                for (unsigned short pixelX = 0; pixelX < coord1D; ++pixelX)
                {
                    // Pixel is active
                    if (spriteRow & (0x80 >> pixelX) != 0)
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
            break;
        }
        case 0xE000:
            switch (opcode & 0x000F)
            {
            case 0x0001:
                // Skips instruction if key in VX is not pressed
                break;
            case 0x000E:
                // Skips instruction if key in FX is pressed
                break;
            }
            break;
        case 0xF000:
            switch (opcode & 0x000F)
            {
            case 0x0003:
            {
                unsigned short x = opcode & 0x0F00 >> BIT_8;
                unsigned short value = system.Registers.V[x];

                system.Memory[system.Registers.I + 2] = value % 10;
                value /= 10;

                system.Memory[system.Registers.I + 1] = value % 10;
                value /= 10;

                system.Memory[system.Registers.I] = value % 10;
                break;
            }
            case 0x0005:
                switch (opcode * 0x00F0)
                {
                case 0x0010:
                {
                    unsigned short x = opcode & 0x0F00 >> BIT_8;
                    system.Timer.DelayTimer = system.Registers.V[x];
                    break;
                }
                case 0x0050:
                {
                    unsigned short x = opcode & 0x0F00 >> BIT_8;
                    for (unsigned short i = 0; i <= x; ++i)
                    {
                        system.Memory[system.Registers.I + i] = system.Registers.V[i];
                    }
                }
                break;
                case 0x0060:
                {
                    unsigned short x = opcode & 0x0F00 >> BIT_8;
                    for (unsigned short i = 0; i <= x; ++i)
                    {
                        system.Registers.V[i] = system.Memory[system.Registers.I + i];
                    }
                    break;
                }
                }
                break;
            case 0x0007:
            {
                unsigned short x = opcode & 0x0F00 >> BIT_8;
                system.Registers.V[x] = system.Timer.DelayTimer;
            }
            break;
            case 0x0008:
            {
                unsigned short x = opcode & 0x0F00 >> BIT_8;
                system.Timer.SoundTimer = system.Registers.V[x];
                break;
            }
            case 0x0009:
                // Sets I to the location of sprite
                break;
            case 0x000A:
            {
                unsigned short x = opcode & 0x0F00 >> BIT_8;
                bool isKeyPressed = false;
                for (unsigned short i = 0; i < CHIP_8_INPUT_MAX; ++i)
                {
                    if (system.Input.key[i] == KEY_STATE::KEY_PRESSED)
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
            case 0x000E:
            {
                unsigned short x = opcode & 0x0F00 >> BIT_8;
                system.Registers.I += system.Registers.V[x];
                break;
            }
            }
            break;
        }
    }

    void Emulator::PollInputs()
    {
        for (unsigned short i = 0; i < CHIP_8_INPUT_MAX; ++i)
        {
            UpdateKeyState(i);
        }
    }

    void Emulator::UpdateKeyState(int key)
    {
        unsigned short state = GetAsyncKeyState(KeyTranslation[key]);
        if ((state & 0x80) != 0)
        {
            if (system.Input.key[key] < KEY_STATE::KEY_PRESSED)
            {
                system.Input.key[key] = KEY_STATE::KEY_PRESSED;
            }
            else
            {
                system.Input.key[key] = KEY_STATE::KEY_DOWN;
            }
        }
        else
        {
            if (system.Input.key[key] >= KEY_STATE::KEY_PRESSED)
            {
                system.Input.key[key] = KEY_STATE::KEY_RELEASED;
            }
            else
            {
                system.Input.key[key] = KEY_STATE::KEY_NONE;
            }
        }
    }

    void Emulator::UpdateTimer()
    {
        if (system.Timer.DelayTimer > 0)
        {
            --system.Timer.DelayTimer;
        }

        if (system.Timer.SoundTimer > 0)
        {
            if (system.Timer.SoundTimer == 1)
            {
            }
            --system.Timer.SoundTimer;
        }
    }

    void Emulator::Render()
    {
        console.Draw(system.Graphic.gfx);
    }
}