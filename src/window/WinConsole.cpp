#include <iostream>
#include "WinConsole.h"
#include "../system/SystemInfo.h"

#define ASCII_BLOCK 219

using std::cout;
using std::endl;
using std::sprintf;

namespace Core
{
    WinConsole::WinConsole()
    {
        // window = GetConsoleWindow();
    }

    WinConsole::~WinConsole()
    {
    }

    void WinConsole::InitConsole(unsigned short width, unsigned short height)
    {
        this->width = width;
        this->height = height;
        this->outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        this->isInitialized = true;
    }

    void WinConsole::SetCursorPosition(short x, short y)
    {
        COORD pos = {x, y};
        SetConsoleCursorPosition(this->outputHandle, pos);
    }

    void WinConsole::Draw(unsigned char *pixels)
    {
        if (!this->isInitialized)
            throw "Console has not been initialized!";

        SetCursorPosition(0, 0);
        for (unsigned short y = 0; y < this->height; ++y)
        {
            for (unsigned short x = 0; x < this->width; ++x)
            {
                if (pixels[(y * CHIP_8_SCREEN_WIDTH) + x])
                    cout << (char)ASCII_BLOCK;
                else
                    cout << (char)32;
            }
            cout << endl;
        }
    }

    void WinConsole::Log(const char *text)
    {
        // SetCursorPosition(70, logCount++);
        // cout << text;
    }

    void WinConsole::Log(const char *formatText, unsigned short num)
    {
        char str[4000];
        sprintf(str, formatText, num);
        Log(str);
    }

    bool WinConsole::IsFocused()
    {
        return GetConsoleWindow() == GetForegroundWindow();
    }
}
