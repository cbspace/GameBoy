#ifndef JOYPAD_H
#define JOYPAD_H

#include <SDL2/SDL.h>
#include "../memory/memory.h"

#define R_P1    0xff00      // Register for reading joy pad info

// Button Values
#define JP_START    0x80
#define JP_SELECT   0x40
#define JP_B        0x20
#define JP_A        0x10
#define JP_DOWN     0x08
#define JP_UP       0x04
#define JP_LEFT     0x02
#define JP_RIGHT    0x01

class Joypad
{
    public:
        Joypad();
        void attach_memory(Memory* mem_in);     // Set pointer used to access memory object
        uint8_t key_down();                     // Process key press events, return 1 on quit

    private:
        Memory* mem;

};

#endif // JOYPAD_H