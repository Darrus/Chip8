#include "system/SystemInfo.h"
#include <iostream>
#include "window/WinConsole.h"
#include <windows.h>

#define KEYSTATE_DOWN 0x01       // 0000 0001
#define KEYSTATE_RELEASED 0x8000 // 1000 0000 0000 0000

int main(int argc, char **argv)
{
    Core::WinConsole console;
    console.InitConsole(CHIP_8_SCREEN_WIDTH, CHIP_8_SCREEN_HEIGHT);

    while (true)
    {
        console.Draw(nullptr);
        if ((GetAsyncKeyState(VK_ESCAPE) & KEYSTATE_DOWN) != 0)
        {
            break;
        }
    }

    return 0;
}