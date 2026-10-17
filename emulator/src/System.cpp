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

    if (auto e = memory.load_rom(rom_path))
        return e;

    if (rom_is_dmg)
    {
        memory.set_pc(0x0000);
        debug.insert_logo(); // Ensure this copies the 48-byte logo into 0x0104 - 0x0133
        cout << "DMG ROM Loaded" << endl;
    }
    else
    {
        // Skip boot ROM: Set standard post-boot register state
        memory.set_pc(0x0100);
        memory.set_sp(0xFFFE);
        
        // Post-boot CPU registers (DMG)
        memory.reg_set(Register16Bit::AF, 0x01B0);
        memory.reg_set(Register16Bit::BC, 0x0013);
        memory.reg_set(Register16Bit::DE, 0x00D8);
        memory.reg_set(Register16Bit::HL, 0x014D);

        // Hardware registers
        memory.write_byte(0xFF40, 0x91); // LCDC: LCD on, BG on
        memory.write_byte(0xFF47, 0xE4); // BGP: Normal palette (11 10 01 00)
        memory.write_byte(0xFF48, 0xD8); // OBP0
        memory.write_byte(0xFF49, 0x00); // OBP1
        memory.write_byte(0xFF50, 0x01); // Boot ROM unmapped
        
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