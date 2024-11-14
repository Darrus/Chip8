#pragma once

#include "WinConsole.h"
#include "Chip8.h"
#include "KeyState.h"

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

        unsigned short KeyTranslation[CHIP_8_INPUT_MAX] = {
            1, 2, 3, 4,
            'Q', 'W', 'E', 'R',
            'A', 'S', ' D', 'F',
            'Z', 'X', 'C', 'V'};

        void ExecuteOpcode(unsigned short opcode);
        void PollInputs();
        void UpdateKeyState(int key);
        void UpdateTimer();
        void Render();
    };
}