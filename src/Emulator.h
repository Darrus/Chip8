#pragma once

#include "window/WinConsole.h"
#include "system/Chip8.h"

namespace Core
{
    class Emulator
    {
    public:
        Emulator();

        bool LoadGame(const char *path);
        void EmulateCycle();
        void Run();

    private:
        WinConsole console;
        Chip8System::Chip8 system;

        uint16_t KeyTranslation[CHIP_8_INPUT_MAX] = {
            '1', '2', '3', '4',
            'Q', 'W', 'E', 'R',
            'A', 'S', 'D', 'F',
            'Z', 'X', 'C', 'V'};

        void ExecuteOpcode(uint16_t opcode);
        void PollInputs();
        void UpdateKeyState(int key);
        void UpdateTimer();
        void Render();
    };
}