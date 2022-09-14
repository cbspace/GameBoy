#pragma once

#include <SDL2/SDL.h>
#include "memory.h"
#include "interrupt.h"

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
#define JP_EMPTY    0x00

class Joypad
{
    public:
        Joypad(Memory& mem_in, Interrupt& ir_in);
        uint8_t key_down();

    private:
        Memory& mem;
        Interrupt& ir;

};