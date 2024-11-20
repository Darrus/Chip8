#pragma once

#include <cstdint>
#include <windows.h>

namespace Core
{
    class WinConsole
    {
    public:
        WinConsole();
        ~WinConsole();

        void InitConsole(uint16_t width, uint16_t height);
        void SetCursorPosition(short x, short y);
        void Draw(uint8_t *pixels);

        void Log(const char *text);
        void Log(const char *formatText, uint16_t num);
        // void Log(const char *formatText, double num);
        bool IsFocused();

    private:
        bool isInitialized = false;
        HANDLE outputHandle;
        uint16_t width = 0;
        uint16_t height = 0;
        uint16_t logCount = 0;
    };
}