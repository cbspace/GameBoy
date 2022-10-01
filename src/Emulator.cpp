#include "Emulator.h"

using namespace std;

Emulator::Emulator() : 
    disp(memory, interrupt, clock),
    interrupt(memory, clock),
    //jp(memory, interrupt),
    emudebug(memory),
    cpu(memory, clock, interrupt, emudebug),
    quit_flag(false)
{}

void Emulator::start(string rom_path, bool rom_is_dmg, bool debug_mode_enabled)
{
    if ( auto e = memory.load_rom(rom_path) ) { 
        cout << e.value().get_error_string() << endl;;
        return; 
    }
    
    if (rom_is_dmg) {
        memory.set_pc(0x00);
        emudebug.insert_logo();
        cout << "DMG ROM Loaded" << endl;
    } else {
        //disp.set_window_title(memory.get_rom_title());
        cout << "Rom Title: " << memory.get_rom_title() << endl;
        memory.write_byte(R_LCDC, 0x91);
    }
}

void Emulator::main_loop()
{
    cpu.cycle();
    disp.display_cycle();
    interrupt.check_interrupts();
}

void Emulator::print_cl_usage_message() {
    cout << "Command line usage:" << endl;
    cout << "(1)  gameboy /path/to/rom.gb [-debug]" << endl;
    cout << "(2)  gameboy /path/to/rom.gb [-dmg] /path/to/dmg_rom.bin [-debug]" << endl;
}
