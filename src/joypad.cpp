#include "joypad.h"
#include "memory.h"
#include "interrupt.h"

#include <SDL2/SDL.h>

Joypad::Joypad(Memory& mem_in, Interrupt& ir_in) : 
    mem(mem_in),
    ir(ir_in) 
{}

uint8_t Joypad::key_down()
{
    SDL_Event e;
    uint8_t quit_signal = 0;

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