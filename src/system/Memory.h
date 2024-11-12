#pragma once

#include "SystemInfo.h"

/** Memory Map
 * 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
 * 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
 * 0x200-0xFFF - Program ROM and work RAM
 **/
namespace Chip8System
{
    class Memory
    {
        unsigned char Memory[CHIP_8_MEMORY_MAX];
    };
}