#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>

#include "Emulator.h"
#include "KeyState.h"
#include "system/SystemInfo.h"
#include "system/Opcodes.h"
#include "Clock.h"

#define VKEY_PRESSED 0x1
#define FPS 15

#define BIT_8 8

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
        const double FRAME_UPDATE_TIME = 1000 / FPS;
        const double CYCLE_UPDATE_TIME = 1000 / CHIP_8_CYCLE_SPEED;

        Clock frameClock;
        frameClock.Start();

        Clock cycleClock;
        cycleClock.Start();

        while (true)
        {
            if (console.IsFocused() && (GetAsyncKeyState(VK_ESCAPE) & VKEY_PRESSED) != 0)
                break;

            if (cycleClock.Delta() >= CYCLE_UPDATE_TIME)
            {
                cycleClock.Start();
                EmulateCycle();
            }

            if (frameClock.Delta() >= FRAME_UPDATE_TIME)
            {
                frameClock.Start();
                PollInputs();
                Render();
            }
        }
    }

    void Emulator::ExecuteOpcode(uint16_t opcode)
    {
        switch (opcode & 0xF000)
        {
        case 0x0000:
            switch (opcode & 0x0EEE)
            {
            case 0x00E0:
                Chip8System::Opcodes::_00E0(system, opcode);
                break;
            case 0x00EE:
                Chip8System::Opcodes::_00EE(system, opcode);
                break;
            default:
                Chip8System::Opcodes::_0NNN(system, opcode);
                break;
            }
            break;
        case 0x1000:
            Chip8System::Opcodes::_1NNN(system, opcode);
            break;
        case 0x2000:
            Chip8System::Opcodes::_2NNN(system, opcode);
            break;
        case 0x3000:
            Chip8System::Opcodes::_3XNN(system, opcode);
            break;
        case 0x4000:
            Chip8System::Opcodes::_4XNN(system, opcode);
            break;
        case 0x5000:
            Chip8System::Opcodes::_5XY0(system, opcode);
            break;
        case 0x6000:
            Chip8System::Opcodes::_6XNN(system, opcode);
            break;
        case 0x7000:
            Chip8System::Opcodes::_7XNN(system, opcode);
            break;
        case 0x8000:
        {
            switch (opcode & 0x000F)
            {
            case 0x0000:
                Chip8System::Opcodes::_8XY0(system, opcode);
                break;
            case 0x0001:
                Chip8System::Opcodes::_8XY1(system, opcode);
                break;
            case 0x0002:
                Chip8System::Opcodes::_8XY2(system, opcode);
                break;
            case 0x0003:
                Chip8System::Opcodes::_8XY3(system, opcode);
                break;
            case 0x0004:
                Chip8System::Opcodes::_8XY4(system, opcode);
                break;
            case 0x0005:
                Chip8System::Opcodes::_8XY5(system, opcode);
                break;
            case 0x0006:
                Chip8System::Opcodes::_8XY6(system, opcode);
                break;
            case 0x0007:
                Chip8System::Opcodes::_8XY7(system, opcode);
                break;
            case 0x000E:
                Chip8System::Opcodes::_8XYE(system, opcode);
                break;
            }
        }
        break;
        case 0x9000:
            Chip8System::Opcodes::_9XY0(system, opcode);
            break;
        case 0xA000:
            Chip8System::Opcodes::_ANNN(system, opcode);
            break;
        case 0xB000:
            Chip8System::Opcodes::_BNNN(system, opcode);
            break;
        case 0xC000:
            Chip8System::Opcodes::_CXNN(system, opcode);
            break;
        case 0xD000:
            Chip8System::Opcodes::_DXYN(system, opcode);
            break;
        case 0xE000:
        {
            switch (opcode & 0x000F)
            {
            case 0x000E:
                Chip8System::Opcodes::_EX9E(system, opcode);
                break;
            case 0x0001:
                Chip8System::Opcodes::_EXA1(system, opcode);
                break;
            }
            break;
        }
        case 0xF000:
        {
            switch (opcode & 0x00F0)
            {
            case 0x0000:
            {
                switch (opcode & 0x000F)
                {
                case 0x0007:
                    Chip8System::Opcodes::_FX07(system, opcode);
                    break;
                case 0x000A:
                    Chip8System::Opcodes::_FX0A(system, opcode);
                    break;
                }
                break;
            }
            case 0x0010:
            {
                switch (opcode & 0x000F)
                {
                case 0x0005:
                    Chip8System::Opcodes::_FX15(system, opcode);
                    break;
                case 0x0008:
                    Chip8System::Opcodes::_FX18(system, opcode);
                    break;
                case 0x000E:
                    Chip8System::Opcodes::_FX1E(system, opcode);
                    break;
                }
                break;
            }
            case 0x0020:
                Chip8System::Opcodes::_FX29(system, opcode);
                break;
            case 0x0030:
                Chip8System::Opcodes::_FX33(system, opcode);
                break;
            case 0x0050:
                Chip8System::Opcodes::_FX55(system, opcode);
                break;
            case 0x0060:
                Chip8System::Opcodes::_FX65(system, opcode);
                break;
            }
            break;
        }
        }
    }

    void Emulator::PollInputs()
    {
        for (int i = 0; i < CHIP_8_INPUT_MAX; ++i)
        {
            UpdateKeyState(i);
        }
    }

    void Emulator::UpdateKeyState(int key)
    {
        uint16_t state = GetAsyncKeyState(KeyTranslation[key]);
        if ((state & VKEY_PRESSED) != 0)
        {
            console.Log("Key Pressed: %d", key);
            system.Input.key[key] = KEY_STATE::KEY_DOWN;
        }
        else
        {
            if (system.Input.key[key] == KEY_STATE::KEY_DOWN)
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