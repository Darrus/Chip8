#pragma once

#include "WinConsole.h"
#include "Chip8.h"

namespace Core
{
    class Emulator
    {
    public:
        Emulator();

        void LoadGame(char *path);
        void EmulateCycle();
        void Run();

    private:
        WinConsole console;
        Chip8System::Chip8 system;

        void ExecuteOpcode(unsigned short opcode);
        void UpdateTimer();
        void Render();
    };
}