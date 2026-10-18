#include "joypad.h"
#include "Memory.h"
#include "interrupt.h"

Joypad::Joypad(Memory& mem_in, Interrupt& ir_in) : 
    mem(mem_in),
    ir(ir_in),
    button_state(0x00)
{}

u8 Joypad::get_p1_value() const
{
    // The CPU selects which line to poll by writing to bits 4 and 5 of R_P1 (active-low: 0 = selected)
    u8 p1_select = mem.get_raw_byte(R_P1);
    u8 result = 0x0F; // Default: unpressed (active-low lines pull high to 1)

    // Bit 4 is LOW -> Direction buttons selected
    if (!(p1_select & 0x10))
    {
        if (button_state & 0x01) result &= ~0x01; // Right
        if (button_state & 0x02) result &= ~0x02; // Left
        if (button_state & 0x04) result &= ~0x04; // Up
        if (button_state & 0x08) result &= ~0x08; // Down
    }

    // Bit 5 is LOW -> Action buttons selected
    if (!(p1_select & 0x20))
    {
        if (button_state & 0x10) result &= ~0x01; // A
        if (button_state & 0x20) result &= ~0x02; // B
        if (button_state & 0x40) result &= ~0x04; // Select
        if (button_state & 0x80) result &= ~0x08; // Start
    }

    // Bits 6 and 7 are unused and always read as 1
    return (p1_select & 0x30) | result | 0xC0;
}

void Joypad::button_pressed(JoypadButton button)
{
    if (button == JoypadButton::Nothing) return;

    auto it = joypad_button_values.find(button);
    if (it != joypad_button_values.end())
    {
        u8 mask = it->second;

        // Check if button was previously not pressed
        if (!(button_state & mask))
        {
            button_state |= mask;

            // Trigger Joypad Interrupt (Bit 4 of IF)
            ir.request_interrupt(InterruptType::Joypad);
        }
    }
}

void Joypad::button_released(JoypadButton button)
{
    if (button == JoypadButton::Nothing) return;

    auto it = joypad_button_values.find(button);
    if (it != joypad_button_values.end())
    {
        button_state &= ~(it->second);
    }
}