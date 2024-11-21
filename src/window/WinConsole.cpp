#include <iostream>
#include "WinConsole.hpp"

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

    void WinConsole::InitConsole(uint16_t width, uint16_t height)
    {
        this->width = width;
        this->height = height;
        this->outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        this->isInitialized = true;
    }

    void WinConsole::SetCursorPosition(short x, short y)
    {
        COORD pos = {x, y};
        SetConsoleCursorPosition(outputHandle, pos);
    }

    void WinConsole::Draw(uint8_t *pixels)
    {
        if (!isInitialized)
            throw "Console has not been initialized!";

        SetCursorPosition(0, 0);
        for (uint16_t y = 0; y < height; ++y)
        {
            for (uint16_t x = 0; x < width; ++x)
            {
                if (pixels[(y * width) + x])
                    cout << (char)ASCII_BLOCK;
                else
                    cout << (char)32;
            }
            cout << endl;
        }
    }

    void WinConsole::Log(const char *text)
    {
        logCount = (logCount + 1) % 50;
        SetCursorPosition(70, logCount);
        cout << text;
        cout << "                  ";
    }

    void WinConsole::Log(const char *formatText, uint16_t num)
    {
        char str[4000];
        sprintf(str, formatText, num);
        Log(str);
    }

    // void WinConsole::Log(const char *formatText, double num)
    // {
    //     char str[4000];
    //     sprintf(str, formatText, num);
    //     Log(str);
    // }

    bool WinConsole::IsFocused()
    {
        return GetConsoleWindow() == GetForegroundWindow();
    }
}
