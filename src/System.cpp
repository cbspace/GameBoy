#include "System.h"

using namespace std;

System::System() : 
    disp(memory, interrupt, clock),
    interrupt(memory),
    debug(memory),
    cpu(memory, clock, interrupt),
    quit_flag(false)
{}

optional<Error> System::start(string rom_path, bool rom_is_dmg, bool debug_mode_enabled)
{
    memory.init();

    if ( auto e = memory.load_rom(rom_path) )
        return e; 
    
    if (rom_is_dmg) {
        memory.set_pc(0x00);
        debug.insert_logo();
        cout << "DMG ROM Loaded" << endl;
    } else {
        //disp.set_window_title(memory.get_rom_title());
        cout << "Rom Title: " << memory.get_rom_title() << endl;
        memory.write_byte(R_LCDC, 0x91);
    }

    return nullopt;
}

void System::main_loop()
{
    cpu.cycle();
    disp.display_cycle();
    interrupt.check_interrupts();
}

void System::print_cl_usage_message() {
    cout << "Command line usage:" << endl;
    cout << "(1)  gameboy /path/to/rom.gb [-debug]" << endl;
    cout << "(2)  gameboy /path/to/rom.gb [-dmg] /path/to/dmg_rom.bin [-debug]" << endl;
}

u32* System::get_display_buffer() {
    return disp.get_buffer();
}

bool System::new_frame_is_drawn() {
    return disp.new_frame_is_drawn();
}