#include <exception>
#include <iostream>
#include "Emulator.h"

int main(int argc, char **argv)
{
    try
    {
        Core::Emulator emulator;
        emulator.Run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        system("pause");
    }

    return 0;
}