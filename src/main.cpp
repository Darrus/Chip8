#include <iostream>
#include "Emulator.h"

int main(int argc, char **argv)
{
    Core::Emulator emulator;
    bool gameLoaded = false;

    if (argc > 1)
    {
        const char *path = *(argv + 1); // +1 to get second argument in memory
        gameLoaded = emulator.LoadGame(path);
    }
    else
    {
        // Load default path game
        gameLoaded = emulator.LoadGame("../roms/space_invaders.ch8");
    }

    if (gameLoaded)
    {
        emulator.Run();
    }

    return 0;
}