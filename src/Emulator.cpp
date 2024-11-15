#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>

#include "Emulator.h"
#include "system/SystemInfo.h"

#define VKEY_PRESSED 0x1

#define BIT_4 4
#define BIT_8 8

using namespace std::chrono;

namespace Core
{
    Emulator::Emulator()
    {
        console.InitConsole(CHIP_8_SCREEN_WIDTH, CHIP_8_SCREEN_HEIGHT);
        system.Registers.PC = CHIP_8_MEMORY_PROGRAM_START_ADDRESS;
        LoadGame("../roms/space_invaders.ch8");
    }

    void Emulator::LoadGame(const char *path)
    {
        console.Log("Loading game...");
        // Open the file as a stream of binary and move the file pointer to the end
        std::ifstream file(path, std::ios::binary | std::ios::ate);

        if (file.is_open())
        {
            // Get size of file and allocate a buffer to hold the contents
            std::streampos size = file.tellg();
            char *buffer = new char[size];

            // Go back to the beginning of the file and fill the buffer
            file.seekg(0, std::ios::beg);
            file.read(buffer, size);
            file.close();

            // Load the ROM contents into the Chip8's memory, starting at 0x200
            for (long i = 0; i < size; ++i)
            {
                system.Memory[CHIP_8_MEMORY_PROGRAM_START_ADDRESS + i] = buffer[i];
            }

            // Free the buffer
            delete[] buffer;
        }
        console.Log("Game loaded.");
    }

    void Emulator::EmulateCycle()
    {
        // console.Log("Emulating cycle...");
        system.opcode = system.Memory[system.Registers.PC] << BIT_8;
        system.opcode |= system.Memory[system.Registers.PC + 1];
        system.Registers.PC += 2;

        // console.Log("Opcode: %X", system.opcode);
        ExecuteOpcode(system.opcode);
        UpdateTimer();
        // console.Log("Cycle emulated.");
    }

    void Emulator::Run()
    {
        const double FPS = 30;
        const double TIME_TO_UPDATE = std::pow(10.0, 9.0) / FPS;
        double dt = 0.0;
        high_resolution_clock::time_point start = high_resolution_clock::now();

        while (true)
        {
            high_resolution_clock::time_point now = high_resolution_clock::now();
            dt += duration_cast<nanoseconds>(now - start).count();
            start = high_resolution_clock::now();

            if (dt > TIME_TO_UPDATE)
            {
                dt = 0.0;
                PollInputs();
                EmulateCycle();
                Render();
                // std::system("pause");
            }

            if (console.IsFocused() && (GetAsyncKeyState(VK_ESCAPE) & VKEY_PRESSED) != 0)
                break;
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
                system.Registers.PC = opcode & 0x0FFF;
                break;
            }
            break;
        case 0x1000:
        {
            system.Registers.PC = opcode & 0x0FFF;
            break;
        }
        case 0x2000:
        {
            system.Registers.stack[system.Registers.SP] = system.Registers.PC;
            ++system.Registers.SP; // Push to stack
            system.Registers.PC = opcode & 0x0FFF;
            break;
        }
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
            unsigned short y = (opcode & 0x00F0) >> BIT_4;

            switch (opcode & 0x000F)
            {
            case 0x0000:
            {
                system.Registers.V[x] = system.Registers.V[y];
                break;
            }
            case 0x0001:
            {
                system.Registers.V[x] |= system.Registers.V[y];
                break;
            }
            case 0x0002:
            {
                system.Registers.V[x] &= system.Registers.V[y];
                break;
            }
            case 0x0003:
            {
                system.Registers.V[x] ^= system.Registers.V[y];
                break;
            }
            case 0x0004:
            {
                unsigned short sum = system.Registers.V[x] + system.Registers.V[y];
                system.Registers.V[x] = sum & 0xFF;
                if (sum > 0xFF)
                {
                    system.Registers.V[0xF] = 1;
                }
                else
                {
                    system.Registers.V[0xF] = 0;
                }
                break;
            }
            case 0x0005:
            {
                unsigned char xValue = system.Registers.V[x];
                unsigned char yValue = system.Registers.V[y];
                system.Registers.V[x] = xValue - yValue;
                if (xValue >= yValue)
                {
                    system.Registers.V[0xF] = 1;
                }
                else
                {
                    system.Registers.V[0xF] = 0;
                }
                break;
            }
            case 0x0006:
            {
                unsigned short xLSB = system.Registers.V[x] & 1;
                system.Registers.V[x] >>= 1;
                system.Registers.V[0xF] = xLSB;
                break;
            }
            case 0x0007:
            {
                unsigned char xValue = system.Registers.V[x];
                unsigned char yValue = system.Registers.V[y];
                system.Registers.V[x] = yValue - xValue;
                if (yValue >= xValue)
                {
                    system.Registers.V[0xF] = 1;
                }
                else
                {
                    system.Registers.V[0xF] = 0;
                }
                break;
            }
            case 0x000E:
            {
                unsigned char xMSB = system.Registers.V[x] >> 7;
                system.Registers.V[x] <<= 1;
                system.Registers.V[0xF] = xMSB;
                break;
            }
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
            unsigned short x = (opcode & 0x0F00) >> BIT_8;
            system.Registers.V[x] = (rand() % 256) & (opcode & 0x00FF);
        }
        break;
        case 0xD000:
        {
            unsigned char x = (opcode & 0x0F00) >> BIT_8;
            unsigned char y = (opcode & 0x00F0) >> BIT_4;

            unsigned short xCoord = system.Registers.V[x];
            unsigned short yCoord = system.Registers.V[y];
            unsigned short spriteHeight = opcode & 0xF;

            system.Registers.V[0xF] = 0;
            for (unsigned short row = 0; row < spriteHeight; ++row)
            {
                // Convert x and y to 1 dimensional coordinate
                unsigned short coord1D = xCoord + ((yCoord + row) * CHIP_8_SCREEN_WIDTH);
                unsigned short spriteRow = system.Memory[system.Registers.I + row];

                for (unsigned short pixelX = 0; pixelX < 8; ++pixelX)
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
            break;
        }
        case 0xE000:
            switch (opcode & 0x000F)
            {
            case 0x0001:
            {
                // Skips instruction if key in VX is not pressed
                unsigned char x = (opcode & 0x0F00) >> BIT_8;
                unsigned short key = system.Registers.V[x];
                console.Log("Key: %d", key);
                console.Log("Key State: %X", system.Input.key[key]);
                if (system.Input.key[key] != KEY_STATE::KEY_PRESSED)
                {
                    system.Registers.PC += 2;
                }
                break;
            }
            case 0x000E:
            {
                // Skips instruction if key in VX is not pressed
                unsigned char x = (opcode & 0x0F00) >> BIT_8;
                unsigned short key = system.Registers.V[x];
                console.Log("Key: %d", key);
                console.Log("Key State: %X", system.Input.key[key]);
                if (system.Input.key[key] == KEY_STATE::KEY_PRESSED)
                {
                    system.Registers.PC += 2;
                }
                break;
            }
            }
            break;
        case 0xF000:
        {
            switch (opcode & 0x00F0)
            {
            case 0x0000:
            {
                switch (opcode & 0x000F)
                {
                case 0x0007:
                {
                    unsigned short x = (opcode & 0x0F00) >> BIT_8;
                    system.Registers.V[x] = system.Timer.DelayTimer;
                    break;
                }
                case 0x000A:
                {
                    console.Log("Waiting for key press...");
                    unsigned short x = (opcode & 0x0F00) >> BIT_8;
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
                    break;
                }
                }
                break;
            }
            case 0x0010:
            {
                switch (opcode & 0x000F)
                {
                case 0x0005:
                {
                    unsigned short x = (opcode & 0x0F00) >> BIT_8;
                    system.Timer.DelayTimer = system.Registers.V[x];
                    break;
                }
                case 0x0008:
                {
                    unsigned short x = (opcode & 0x0F00) >> BIT_8;
                    system.Timer.SoundTimer = system.Registers.V[x];
                    break;
                }
                case 0x000E:
                {
                    unsigned short x = (opcode & 0x0F00) >> BIT_8;
                    system.Registers.I += system.Registers.V[x];
                    break;
                }
                }
                break;
            }
            case 0x0020:
            {
                unsigned short x = (opcode & 0x0F00) >> BIT_8;
                unsigned short digit = system.Registers.V[x];
                system.Registers.I = CHIP_8_MEMORY_FONT_ADDRESS + (digit * 5);
                break;
            }
            case 0x0030:
            {
                unsigned short x = (opcode & 0x0F00) >> BIT_8;
                unsigned short value = system.Registers.V[x];

                system.Memory[system.Registers.I + 2] = value % 10;
                value /= 10;

                system.Memory[system.Registers.I + 1] = value % 10;
                value /= 10;

                system.Memory[system.Registers.I] = value % 10;
                break;
            }
            case 0x0050:
            {
                unsigned short x = (opcode & 0x0F00) >> BIT_8;
                for (unsigned short i = 0; i <= x; ++i)
                {
                    system.Memory[system.Registers.I + i] = system.Registers.V[i];
                }
                break;
            }
            case 0x0060:
            {
                unsigned short x = (opcode & 0x0F00) >> BIT_8;
                for (unsigned short i = 0; i <= x; ++i)
                {
                    system.Registers.V[i] = system.Memory[system.Registers.I + i];
                }
                break;
            }
            }
            break;
        }
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
        if ((state & VKEY_PRESSED) != 0)
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
        // console.Log("Rendering...");
        console.Draw(system.Graphic.gfx);
        // console.Log("Rendered");
    }
}