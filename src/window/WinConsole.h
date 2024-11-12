#pragma once

#include <windows.h>

#define ASCII_BLOCK 219

namespace Core
{
    class WinConsole
    {
    public:
        WinConsole();
        ~WinConsole();

        void InitConsole(unsigned short width, unsigned short height);
        void SetCursorPosition(short x, short y);
        void Draw(unsigned char *pixels);

    private:
        bool isInitialized = false;
        HANDLE outputHandle;
        unsigned short width = 0;
        unsigned short height = 0;
    };
}