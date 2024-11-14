#include <iostream>
#include "WinConsole.h"
#include "../system/SystemInfo.h"

using std::cout;
using std::endl;

namespace Core
{
    WinConsole::WinConsole()
    {
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

        for (unsigned short y = 0; y < this->height; ++y)
        {
            for (unsigned short x = 0; x < this->width; ++x)
            {
                if (pixels[(y * CHIP_8_SCREEN_WIDTH) + x])
                    cout << (char)ASCII_BLOCK;
            }
            cout << endl;
        }
        WinConsole::SetCursorPosition(0, 0);
    }
}
