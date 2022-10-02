#pragma once

#include <SDL2/SDL.h>
#include <map>
#include "Memory.h"
#include "interrupt.h"
#include "lib/Types.h"

const u32 R_P1 = 0xff00;      // Register for reading joy pad info

enum class JoypadButton {
    Start,
    Select,
    B,
    A,
    Down,
    Up,
    Left,
    Right,
    Nothing
};

class Joypad
{
    public:
        Joypad(Memory& mem_in, Interrupt& ir_in);
        u8 key_down();

        map<JoypadButton, int> joypad_button_values{
            {JoypadButton::Start, 0x80},
            {JoypadButton::Select, 0x40},
            {JoypadButton::B, 0x20},
            {JoypadButton::A, 0x10},
            {JoypadButton::Down, 0x08},
            {JoypadButton::Up, 0x04},
            {JoypadButton::Left, 0x02},
            {JoypadButton::Right, 0x01},
            {JoypadButton::Nothing, 0x00}
        };

    private:
        Memory& mem;
        Interrupt& ir;

};