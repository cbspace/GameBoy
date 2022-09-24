#include "joypad.h"
#include "Memory.h"
#include "interrupt.h"

#include <SDL2/SDL.h>

Joypad::Joypad(Memory& mem_in, Interrupt& ir_in) : 
    mem(mem_in),
    ir(ir_in) 
{}

u8 Joypad::key_down()
{
    SDL_Event e;
    u8 quit_signal = 0;

    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            quit_signal = 1;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                {
                    quit_signal = 1;
                }
                case SDLK_RETURN:
                {
                    mem.write_byte(R_P1,joypad_button_values[JoypadButton::Start]);
                    ir.if_update(I_JOYPAD, true);
                }
                default:
                {
                    mem.write_byte(R_P1,joypad_button_values[JoypadButton::Nothing]);
                }
            }
        }
    }
    return quit_signal;
}