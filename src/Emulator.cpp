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
            switch (opcode * 0x00EE)
            {
            case 0x00E0:
                // Clear screen
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
            unsigned short index = (opcode & 0x0F00) >> BIT_8;
            if (system.Registers.V[index] == opcode & 0x00FF)
            {
                system.Registers.PC += 2;
            }
            break;
        case 0x4000:
            unsigned short index = (opcode & 0x0F00) >> BIT_8;
            if (system.Registers.V[index] != opcode & 0x00FF)
            {
                system.Registers.PC += 2;
            }
            break;
        case 0x5000:
            unsigned short indexX = (opcode & 0x0F00) >> BIT_8;
            unsigned short indexY = (opcode & 0x00F0) >> BIT_4;
            if (system.Registers.V[indexX] == system.Registers.V[indexY])
            {
                system.Registers.PC += 2;
            }
            break;
        case 0x6000:
            unsigned short index = (opcode & 0x0F00) >> BIT_8;
            system.Registers.V[index] = opcode & 0x00FF;
            break;
        case 0x7000:
            unsigned short index = (opcode & 0x0F00) >> BIT_8;
            system.Registers.V[index] += opcode & 0x00FF;
            break;
        case 0x8000:
            unsigned short indexX = (opcode & 0x0F00) >> BIT_8;
            unsigned short indexY = (opcode & 0x00F0) >> BIT_8;

            switch (opcode & 0x000F)
            {
            case 0x0000:
                system.Registers.V[indexX] = system.Registers.V[indexY];
                break;
            case 0x0001:
                system.Registers.V[indexX] |= system.Registers.V[indexY];
                break;
            case 0x0002:
                system.Registers.V[indexX] &= system.Registers.V[indexY];
                break;
            case 0x0003:
                system.Registers.V[indexX] ^= system.Registers.V[indexY];
                break;
            case 0x0004:
                system.Registers.V[indexX] += system.Registers.V[indexY];
                break;
            case 0x0005:
                system.Registers.V[indexX] -= system.Registers.V[indexY];
                break;
            case 0x0006:
                // system.Registers.V[indexX] = system.Registers.V[indexY];
                break;
            case 0x0007:
                system.Registers.V[indexX] = system.Registers.V[indexY] - system.Registers.V[indexX];
                break;
            case 0x000E:
                // system.Registers.V[indexX] = system.Registers.V[indexY];
                break;
            }
            break;
        case 0x9000:
            break;
        case 0xA000:
            system.Registers.I = opcode & 0x0FFF;
            break;
        case 0xB000:
            break;
        case 0xC000:
            break;
        case 0xD000:
            break;
        case 0xE000:
            switch (opcode & 0x000F)
            {
            case 0x0001:
                break;
            case 0x000E:
                break;
            }
            break;
        case 0xF000:
            switch (opcode & 0x000F)
            {
            case 0x0003:
                break;
            case 0x0005:
                switch (opcode * 0x00F0)
                {
                case 0x0010:
                    break;
                case 0x0050:
                    break;
                case 0x0060:
                    break;
                }
                break;
            case 0x0007:
                break;
            case 0x0008:
                break;
            case 0x0009:
                break;
            case 0x000A:
                break;
            case 0x000E:
                break;
            }
            break;
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
        console.Draw(nullptr);
    }
}