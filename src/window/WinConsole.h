#pragma once

#include <windows.h>

namespace Core
{
    class WinConsole
    {
    public:
        // HWND window;

        WinConsole();
        ~WinConsole();

        void InitConsole(unsigned short width, unsigned short height);
        void SetCursorPosition(short x, short y);
        void Draw(unsigned char *pixels);

        void Log(const char *text);
        void Log(const char *formatText, unsigned short num);
        bool IsFocused();

    private:
        bool isInitialized = false;
        HANDLE outputHandle;
        unsigned short width = 0;
        unsigned short height = 0;
        unsigned short logCount = 0;
    };
}