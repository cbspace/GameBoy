#include "System.h"

using namespace std;

System::System() : 
    disp(memory, interrupt, clock),
    memory(jpad),
    interrupt(memory),
    debug(memory),
    cpu(memory, clock, interrupt),
    jpad(memory, interrupt),
    quit_flag(false)
{}

optional<Error> System::start(string rom_path, bool rom_is_dmg, bool debug_mode_enabled)
{
    memory.init();

    if (auto e = memory.load_rom(rom_path))
        return e;

    if (rom_is_dmg)
    {
        memory.set_pc(0x0000);
        debug.insert_logo(); // Ensure this copies the 48-byte logo into 0x0104 - 0x0133
        cout << "DMG ROM Loaded" << endl;
    } else {
        cout << "Rom Title: " << memory.get_rom_title() << endl;
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