#include "Graphic.h"
#include "Input.h"
#include "Memory.h"
#include "Register.h"
#include "Timer.h"
#include "SystemInfo.h"

namespace Chip8System
{
    struct Chip8
    {
        unsigned char Memory[CHIP_8_MEMORY_MAX];
        unsigned short opcode;

        Graphic Graphic;
        Input Input;
        Register Registers;
        Timer Timer;
    };
}