#pragma once

#include "LibCB/Types.h"
#include <map>

class Memory;
class Interrupt;
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

        void button_pressed(JoypadButton button);
        void button_released(JoypadButton button);

        // Called by Memory when reading 0xFF00
        u8 get_p1_value() const;

        std::map<JoypadButton, int> joypad_button_values{
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

        // Bit set = key currently held down (1 = pressed, 0 = released)
        u8 button_state;
};