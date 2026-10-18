#include "Memory.h"
#include "interrupt.h"
#include "Timer.h"

Memory::Memory(Joypad& jpad_in, Timer& timer_in) :
    jpad(jpad_in),
    timer(timer_in)
{
    init();
}

void Memory::init()
{
    set_sp(SP_INITIAL_VALUE);    // sp initialised to fffe (gameboy default)
    set_pc(ROM_START_ADDRESS);   // pc initialised to 0x100 (start address)

    // Post-boot CPU registers (DMG)
    reg_set(Register16Bit::AF, 0x01B0);
    reg_set(Register16Bit::BC, 0x0013);
    reg_set(Register16Bit::DE, 0x00D8);
    reg_set(Register16Bit::HL, 0x014D);

    // Zero RAM/ROM
    for (u32 i = 0; i < MEM_SIZE; i++)
    {
        ram[i] = 0;
    }

    // Hardware registers
    write_byte(0xFF40, 0x91); // LCDC: LCD on, BG on
    write_byte(0xFF47, 0xE4); // BGP: Normal palette (11 10 01 00)
    write_byte(0xFF48, 0xD8); // OBP0
    write_byte(0xFF49, 0x00); // OBP1
    write_byte(0xFF50, 0x01); // Boot ROM unmapped
}

/// Flag Operations

// Set or clear a flag
void Memory::flag_update(u8 flag_id, u8 flg_val)
{
    if (flg_val!=0)
    {
        reg[RF] |= flag_id;
    }
    else
    {
        reg[RF] &= ~flag_id;
    }
}

// Return flag value
u8 Memory::flag_get(u8 flag_id)
{
    u8 flag_val = (reg[RF] & flag_id);
    return !(flag_val==0);
}

optional<Error> Memory::load_rom(string rom_path)
{
    streampos file_size;

    ifstream file(rom_path, ios::in|ios::binary|ios::ate);
    if (!file.is_open()) { return Error("Unable to open file",rom_path); }

    file_size = file.tellg();
    if (file_size > 32*1024) { return Error("ROM not supported, up to 32kb support only"); }

    file.seekg(0, ios::beg);
    file.read(ram, file_size);
    file.close();
    read_rom_title();

    return nullopt;
}

void Memory::read_rom_title()
{
    char rom_title_array[BYTE_ARRAY_SIZE];

    // Copy bytes (max length for title is 14 bytes)
    for (u8 i = 0; i < 14; i++)
    {
        rom_title_array[i] = ram[ROM_TITLE_ADDRESS + i];

        if (ram[ROM_TITLE_ADDRESS + i] == '\0') {
            break;
        } 
    }

    rom_title = rom_title_array;
}

string Memory::get_rom_title()
{
    return rom_title;
}

/// Register Functions

u8 Memory::reg_get(u8 reg_id)
{
    // Make sure reg_id is valid
    if (reg_id < REG_ARRAY_SIZE)
    {
        return reg[reg_id];
    } else {
        cout << "Invalid 16 bitregister id" << endl;
        return 0;
    }
}

u16 Memory::reg_get16(Register16Bit reg_id)
{
    switch(reg_id)
    {
        case Register16Bit::AF:
            return (reg[RA] << 8) + reg[RF];
        case Register16Bit::BC:
            return (reg[RB] << 8) + reg[RC];
        case Register16Bit::DE:
            return (reg[RD] << 8) + reg[RE];
        case Register16Bit::HL:
            return (reg[RH] << 8) + reg[RL];
        default:
            cout << "Invalid 16 bitregister id" << endl;
            return 0;
    }
}

// Set value in 8 bit register
void Memory::reg_set(u8 reg_id, u8 reg_value)
{
    // Make sure reg_id is valid
    if (reg_id < REG_ARRAY_SIZE)
    {
        reg[reg_id] = reg_value;
    }
}

// Set value in 16 bit register
void Memory::reg_set(Register16Bit reg_id, u16 reg_value)
{
    u8 high_byte = reg_value >> 8;
    u8 low_byte = reg_value & 0xff;
    
    switch(reg_id)
    {
        case Register16Bit::AF:
            reg[RA] = high_byte;
            reg[RF] = low_byte & 0xf0; // bottom 4 bits of F are always 0
            break;
        case Register16Bit::BC:
            reg[RB] = high_byte;
            reg[RC] = low_byte;
            break;
        case Register16Bit::DE:
            reg[RD] = high_byte;
            reg[RE] = low_byte;
            break;
        case Register16Bit::HL:
            reg[RH] = high_byte;
            reg[RL] = low_byte;
            break;
    }
}

// Copy data between 8 bit registers
void Memory::reg_copy(u8 to_reg_id, u8 from_reg_id)
{
    // Check if registers are valid
    if ((from_reg_id < REG_ARRAY_SIZE) && (to_reg_id < REG_ARRAY_SIZE))
    {
        reg_set(to_reg_id, reg_get(from_reg_id));
    }
}

// Increment 8-bit register, flags updated
void Memory::reg_inc(u8 reg_id)
{
    if (reg_id < REG_ARRAY_SIZE)
    {
        flag_update(HF, (reg[reg_id] & 0x0F) == 0x0F);
        flag_update(NF, 0);
        reg[reg_id]++;
        flag_update(ZF, reg[reg_id] == 0);
    }
}

// Increment 16-bit register, flags not updated
void Memory::reg_inc16(Register16Bit reg_id)
{
    u16 reg_val = reg_get16(reg_id);
    reg_val++;
    reg_set(reg_id,reg_val);
}

// Decrement 16-bit register, flags not updated
void Memory::reg_dec16(Register16Bit reg_id)
{
    u16 reg_val = reg_get16(reg_id);
    reg_val--;
    reg_set(reg_id,reg_val);
}

// Decrement register, flags updated
void Memory::reg_dec(u8 reg_id)
{
    if (reg_id < REG_ARRAY_SIZE)
    {
        flag_update(HF, (reg[reg_id] & 0x0F) == 0x00);
        flag_update(NF, 1);
        reg[reg_id]--;
        flag_update(ZF, reg[reg_id] == 0);
    }
}

/// Math Functions

// Add value to 8-bit register and update flags, bool parameter is add with carry
void Memory::reg_add(u8 reg_id, u8 add_value, bool carry)
{
    if (reg_id < REG_ARRAY_SIZE)
    {
        u8 c = carry ? flag_get(CF) : 0;
        u16 total = reg[reg_id] + add_value + c;

        flag_update(HF, ((reg[reg_id] & 0x0F) + (add_value & 0x0F) + c) > 0x0F);
        flag_update(CF, total > 0xFF);
        flag_update(NF, 0);
        reg[reg_id] = static_cast<u8>(total);
        flag_update(ZF, reg[reg_id] == 0);
    }
}

// Subtract value from 8-bit register and flags updated, bool brrow used for sub with borrow
void Memory::reg_sub(u8 reg_id, u8 sub_value, bool borrow)
{
    if (reg_id < REG_ARRAY_SIZE)
    {
        u8 b = borrow ? flag_get(CF) : 0;
        i16 total = reg[reg_id] - sub_value - b;

        flag_update(HF, (reg[reg_id] & 0x0F) < (sub_value & 0x0F) + b);
        flag_update(CF, total < 0);
        flag_update(NF, 1);
        reg[reg_id] = static_cast<u8>(total);
        flag_update(ZF, reg[reg_id] == 0);
    }
}

void Memory::reg_add16(Register16Bit reg_id, u16 reg_n_val)
{
    u32 current = reg_get16(reg_id);
    u32 operand = reg_n_val;
    u32 total = current + operand;

    // Half Carry: carry from bit 11 to bit 12
    bool half_carry = ((current & 0x0FFF) + (operand & 0x0FFF)) > 0x0FFF;
    flag_update(HF, half_carry ? 1 : 0);

    // Carry: carry from bit 15 to bit 16
    flag_update(CF, total > 0xFFFF ? 1 : 0);

    // Subtract flag is always cleared
    flag_update(NF, 0);

    // Zero Flag (ZF) is intentionally NOT modified!
    reg_set(reg_id, static_cast<u16>(total));
}

// Decimal Adjust register A (DAA), flags updated
void Memory::reg_daa()
{
    u8 a = reg[RA];
    bool n = flag_get(NF);
    bool c = flag_get(CF);
    bool h = flag_get(HF);

    u8 correction = 0;
    if (!n) // After addition (ADD, ADC, INC)
    {
        if (h || (a & 0x0F) > 0x09) {
            correction |= 0x06;
        }

        if (c || a > 0x99) {
            correction |= 0x60;
            flag_update(CF, 1);
        }

        a += correction;
    }
    else // After subtraction (SUB, SBC, DEC, CP)
    {
        if (h) {
            correction |= 0x06;
        }

        if (c) {
            correction |= 0x60;
            // CF remains 1
        }
        a -= correction;
    }
    reg[RA] = a;
    flag_update(ZF, a == 0);
    flag_update(HF, 0);
    // NF and CF are handled above; bits 0-3 of F remain 0
}

// Rotate contents of A register left and store bit 7 in CF, flags updated
// When carry is true, bit 0 = previous bit 7
// When carry is false, bit 0 = previous CF
// Flags 0 0 0 C
void Memory::reg_rla(bool carry)
{
    u8 bit7 = (reg[RA] & 0x80) >> 7;
    u8 a_value = reg[RA] << 1;

    if (carry)
    {
        // Append previous bit 7 to bit 0
        reg[RA] = a_value + bit7;
    }
    else
    {
        // Append previous CF to bit 0
        reg[RA] = a_value + flag_get(CF);
    }

    flag_update(HF,0);
    flag_update(NF,0);
    flag_update(ZF,0);
    flag_update(CF,(bool)bit7);
}

// Rotate contents of register n left and store bit 7 in CF, flags updated
// When carry is true, bit 0 = previous bit 7
// When carry is false, bit 0 = previous CF
// Flags Z 0 0 C
void Memory::reg_rl(u8 reg_id, bool carry)
{
    u8 bit7 = (reg[reg_id] & 0x80) >> 7;
    u8 a_value = reg[reg_id] << 1;

    if (carry)
    {
        // Append previous bit 7 to bit 0
        reg[reg_id] = a_value + bit7;
    }
    else
    {
        // Append previous CF to bit 0
        reg[reg_id] = a_value + flag_get(CF);
    }

    flag_update(HF,0);
    flag_update(NF,0);
    flag_update(ZF,(reg[reg_id]==0));
    flag_update(CF,(bool)bit7);
}

// Rotate contents of A register right and store bit 0 in CF, flags updated
// When carry is true, bit 7 = previous bit 0
// When carry is false, bit 7 = previous CF
// Flags 0 0 0 C
void Memory::reg_rra(bool carry)
{
    u8 bit0 = reg[RA] & 0x01;
    u8 a_value = reg[RA] >> 1;

    if (carry)
    {
        // Append previous bit 0 to bit 7
        reg[RA] = a_value + (bit0 << 7);
    }
    else
    {
        // Append previous CF to bit 7
        reg[RA] = a_value + (flag_get(CF) << 7);
    }

    flag_update(HF,0);
    flag_update(NF,0);
    flag_update(ZF,0);
    flag_update(CF,(bool)bit0);
}

// Rotate contents of register n right and store bit 0 in CF, flags updated
// When carry is true, bit 7 = previous bit 0
// When carry is false, bit 7 = previous CF
// Flags Z 0 0 C
void Memory::reg_rr(u8 reg_id, bool carry)
{
    u8 bit0 = reg[reg_id] & 0x01;
    u8 a_value = reg[reg_id] >> 1;

    if (carry)
    {
        // Append previous bit 0 to bit 7
        reg[reg_id] = a_value + (bit0 << 7);
    }
    else
    {
        // Append previous CF to bit 7
        reg[reg_id] = a_value + (flag_get(CF) << 7);
    }

    flag_update(HF,0);
    flag_update(NF,0);
    flag_update(ZF,(reg[reg_id]==0));
    flag_update(CF,(bool)bit0);
}

// Shift contents of register n left and store bit 7 in CF, bit0 = 0, flags updated
// Flags Z 0 0 C
void Memory::reg_sla(u8 reg_id)
{
    u8 bit7 = (reg[reg_id] & 0x80) >> 7;
    reg[reg_id] = reg[reg_id] << 1;

    flag_update(HF,0);
    flag_update(NF,0);
    flag_update(ZF,(reg[reg_id]==0));
    flag_update(CF,(bool)bit7);
}

// Shift contents of register A right and store bit 0 in CF, bit7 unchanged, flags updated
// Flags Z 0 0 C
void Memory::reg_sra(u8 reg_id)
{
    u8 bit0 = reg[reg_id] & 0x01;
    u8 bit7 = reg[reg_id] & 0x80;
    reg[reg_id] = (reg[reg_id] >> 1) + bit7;

    flag_update(HF,0);
    flag_update(NF,0);
    flag_update(ZF,(reg[reg_id]==0));
    flag_update(CF,(bool)bit0);
}

// Shift contents of register A right and store bit 0 in CF, bit7 = 0, flags updated
// Flags Z 0 0 C
void Memory::reg_srl(u8 reg_id)
{
    u8 bit0 = reg[reg_id] & 0x01;
    reg[reg_id] = reg[reg_id] >> 1;

    flag_update(HF,0);
    flag_update(NF,0);
    flag_update(ZF,(reg[reg_id]==0));
    flag_update(CF,(bool)bit0);
}

/// Logical Functions

// Register A is ANDed with and_value, result stored in register A - flags updated
void Memory::reg_and(u8 and_value)
{
    reg[RA] &= and_value;

    flag_update(HF,1);
    flag_update(CF,0);
    flag_update(ZF,(reg[RA]==0));
    flag_update(NF,0);
}

// Register A is ORed with and_value, result stored in register A - flags updated
void Memory::reg_or(u8 or_value)
{
    reg[RA] |= or_value;

    flag_update(HF,0);
    flag_update(CF,0);
    flag_update(ZF,(reg[RA]==0));
    flag_update(NF,0);
}

// Register A is XORed with and_value, result stored in register A - flags updated
void Memory::reg_xor(u8 xor_value)
{
    reg[RA] ^= xor_value;

    flag_update(HF,0);
    flag_update(CF,0);
    flag_update(ZF,(reg[RA]==0));
    flag_update(NF,0);
}

// Register A is compared with cmp_value, result stored in register A - flags updated
void Memory::reg_compare(u8 cmp_value)
{
    i16 total = reg[RA] - cmp_value;
    flag_update(HF, (reg[RA] & 0x0F) < (cmp_value & 0x0F));
    flag_update(CF, total < 0);
    flag_update(ZF, (total & 0xFF) == 0);
    flag_update(NF, 1);
}

// Swap upper and lower nibbles of register, flags updated
void Memory::reg_swap(u8 reg_id)
{
    reg[reg_id] = (reg[reg_id] >> 4) | ((reg[reg_id] & 0xf) << 4);

    flag_update(HF,0);
    flag_update(CF,0);
    flag_update(ZF,(reg[reg_id]==0));
    flag_update(NF,0);
}

// Test bit b in register and set flags accordingly
// Flags Z 0 1 -
void Memory::bit_test(u8 reg_id, u8 bit_number)
{
    u8 bit_val = (reg[reg_id] >> bit_number) & 1;

    flag_update(ZF,(bit_val==0));
    flag_update(NF,0);
    flag_update(HF,1);
}

// Set bit in register, flags not affected
void Memory::bit_set(u8 reg_id, u8 bit_number)
{
    u8 bit_mask = 1 << bit_number;
    u8 new_val = reg[reg_id] | bit_mask;
    reg[reg_id] = new_val;
}

// Reset bit in register, flags not affected
void Memory::bit_res(u8 reg_id, u8 bit_number)
{
    u8 bit_mask = 1 << bit_number;
    u8 new_val = reg[reg_id] & ~bit_mask;
    reg[reg_id] = new_val;
}

/// RAM/ROM Functions

// Read byte from RAM/ROM
u8 Memory::get_byte(u16 address)
{
    // Joypad
    if (address == R_P1) 
    {
        return jpad.get_p1_value();
    }

    // Timer
    if ((address >= A_TIMA) && (address <= A_TAC))
    {
        return timer.read_byte(address);
    }

    if (address == A_IFLAGS)
    {
        return ram[A_IFLAGS] | 0xE0; // Upper 3 bits always read 1 on DMG
    }

    return ram[address];
}

// Read byte from RAM/ROM without bounds check
u8 Memory::get_raw_byte(u16 address)
{
    return ram[address];
}

// Read a bit from an byte in RAM/ROM
u8 Memory::get_bit(u16 address, u8 bit_number)
{
    u8 bit_mask = 1 << bit_number;
    return (ram[address] & bit_mask) >> bit_number;
}

// Read byte from ROM and increment PC
u8 Memory::fetch_byte()
{
    return ram[pc++];
}

// Read byte from RAM/ROM pointed to by 16-bit register
u8 Memory::get_from_pointer(Register16Bit reg_id)
{
    return get_byte(reg_get16(reg_id));
}

// Set byte at RAM address pointed to by 16-bit register to byte value
void Memory::set_from_pointer(Register16Bit reg_id, u8 byte_value)
{
    write_byte(reg_get16(reg_id), byte_value);
}

// Write byte to RAM/ROM
void Memory::write_byte(u16 address, u8 byte)
{
    // Trap ROM writes (MBC commands or ignore if ROM without MBC)
    if (address < 0x8000)
    {
        // Handle MBC banking here if cartridge uses MBC1;
        // Do NOT overwrite ram[address] directly!
        return;
    }

    // Echo RAM mirrors 0xC000 - 0xDDFF
    if (address >= 0xE000 && address < 0xFE00)
    {
        ram[address] = byte;
        ram[address - 0x2000] = byte;
        return;
    }

    // Timer
    if ((address >= A_TIMA) && (address <= A_TAC))
    {
        return timer.write_byte(address, byte);
    }

    ram[address] = byte;

    if (address == R_DMA)
    {
        dma_transfer();
    }
}

// Write a raw byte
void Memory::write_raw(u16 address, u8 byte)
{
    ram[address] = byte;
}

// Update individual bit in RAM/ROM
void Memory::write_bit(u16 address, u8 bit_number, bool bit_val)
{
    u8 new_value;

    u8 current_value = ram[address];
    u8 bit_mask = 1 << bit_number;

    if (bit_val)
    {
        new_value = current_value | bit_mask;
    }
    else
    {
        new_value = current_value & ~bit_mask;
    }

    ram[address] = new_value;
}

// Increment byte at pointer and update flags (Z 0 H -)
void Memory::inc_from_pointer(Register16Bit reg_id)
{
    u8 byte_in = get_from_pointer(reg_id);

    // HF: Carry from bit 3 to bit 4 (lower nibble overflow)
    flag_update(HF, (byte_in & 0x0F) == 0x0F);
    flag_update(NF, 0);

    byte_in++;
    set_from_pointer(reg_id, byte_in);

    flag_update(ZF, byte_in == 0);
    // CF is unaffected
}

// Decrement byte at pointer and update flags (Z 1 H -)
void Memory::dec_from_pointer(Register16Bit reg_id)
{
    u8 byte_in = get_from_pointer(reg_id);

    // HF: Borrow from bit 4 (lower nibble underflow)
    flag_update(HF, (byte_in & 0x0F) == 0x00);
    flag_update(NF, 1);

    byte_in--;
    set_from_pointer(reg_id, byte_in);

    flag_update(ZF, byte_in == 0);
    // CF is unaffected
}

// Swap upper and lower nibbles of register, flags updated
void Memory::swap_from_pointer(Register16Bit reg_id)
{
    u8 byte_in = get_from_pointer(reg_id);
    u8 new_val = (byte_in >> 4) | ((byte_in & 0xf) << 4);
    set_from_pointer(Register16Bit::HL, new_val);

    flag_update(HF,0);
    flag_update(CF,0);
    flag_update(ZF,(new_val==0));
    flag_update(NF,0);
}

// Rotate contents of byte at (n) left and store bit 7 in CF, flags updated
// When with_carry is true, bit 0 = previous bit 7 (RLC)
// When with_carry is false, bit 0 = previous CF (RL)
// Flags Z 0 0 C
void Memory::rl_from_pointer(Register16Bit reg_id, bool with_carry)
{
    u8 byte_in = get_from_pointer(reg_id);
    u8 bit7 = (byte_in & 0x80) >> 7;
    u8 new_value = byte_in << 1;

    if (with_carry)
    {
        new_value += bit7;
        set_from_pointer(reg_id, new_value);
    }
    else
    {
        new_value += flag_get(CF);
        set_from_pointer(reg_id, new_value);
    }

    flag_update(HF,0);
    flag_update(NF,0);
    flag_update(ZF,(new_value==0));
    flag_update(CF,(bool)bit7);
}

// Rotate contents of byte at (n) right and store bit 0 in CF, flags updated
// When with_carry is true, bit 7 = previous bit 0 (RRC)
// When with_carry is false, bit 7 = previous CF (RR)
// Flags Z 0 0 C
void Memory::rr_from_pointer(Register16Bit reg_id, bool with_carry)
{
    u8 byte_in = get_from_pointer(reg_id);
    u8 bit0 = byte_in & 0x01;
    u8 new_value = byte_in >> 1;

    if (with_carry)
    {
        new_value += (bit0 << 7);
        set_from_pointer(reg_id, new_value);
    }
    else
    {
        new_value += (flag_get(CF) << 7);
        set_from_pointer(reg_id, new_value);
    }

    flag_update(HF,0);
    flag_update(NF,0);
    flag_update(ZF,(new_value==0));
    flag_update(CF,(bool)bit0);
}

// Shift contents of byte at (n) left and store bit 7 in CF, bit0 = 0, flags updated
// Flags Z 0 0 C
void Memory::sla_from_pointer(Register16Bit reg_id)
{
    u8 byte_in = get_from_pointer(reg_id);
    u8 bit7 = (byte_in & 0x80) >> 7;
    u8 byte_val = byte_in << 1;

    set_from_pointer(reg_id, byte_val);

    flag_update(HF,0);
    flag_update(NF,0);
    flag_update(ZF,(byte_val==0));
    flag_update(CF,(bool)bit7);
}

// Shift contents of byte at (n) right and store bit 0 in CF, bit7 unchanged, flags updated
// Flags Z 0 0 C
void Memory::sra_from_pointer(Register16Bit reg_id)
{
    u8 byte_in = get_from_pointer(reg_id);
    u8 bit0 = byte_in & 0x01;
    u8 bit7 = byte_in & 0x80;
    u8 byte_val = (byte_in >> 1) + bit7;
    set_from_pointer(reg_id, byte_val);

    flag_update(HF,0);
    flag_update(NF,0);
    flag_update(ZF,(byte_val==0));
    flag_update(CF,(bool)bit0);
}

// Shift contents of byte at (n) right and store bit 0 in CF, bit7 = 0, flags updated
// Flags Z 0 0 C
void Memory::srl_from_pointer(Register16Bit reg_id)
{
    u8 byte_in = get_from_pointer(reg_id);
    u8 bit0 = byte_in & 0x01;
    u8 byte_val = byte_in >> 1;
    set_from_pointer(reg_id, byte_val);

    flag_update(HF,0);
    flag_update(NF,0);
    flag_update(ZF,(byte_val==0));
    flag_update(CF,(bool)bit0);
}

// Test bit b in byte at (n) and set flags accordingly
// Flags Z 0 1 -
void Memory::bit_test_from_pointer(Register16Bit reg_id, u8 bit_number)
{
    u8 bit_val = (get_from_pointer(reg_id) >> bit_number) & 1;

    flag_update(ZF,(bit_val==0));
    flag_update(NF,0);
    flag_update(HF,1);
}

// Set bit in in byte at (n), flags not affected
void Memory::bit_set_from_pointer(Register16Bit reg_id, u8 bit_number)
{
    u8 byte_val = get_from_pointer(reg_id);
    u8 bit_mask = 1 << bit_number;
    u8 new_val = byte_val | bit_mask;

    set_from_pointer(reg_id, new_val);
}

// Reset bit in in byte at (n), flags not affected
void Memory::bit_res_from_pointer(Register16Bit reg_id, u8 bit_number)
{
    u8 byte_val = get_from_pointer(reg_id);
    u8 bit_mask = 1 << bit_number;
    u8 new_val = byte_val & ~bit_mask;
    set_from_pointer(reg_id, new_val);
}

// Perform DMA transfer from ROM/RAM to OAM
void Memory::dma_transfer()
{
    u16 dma_start = ram[R_DMA] << 8;

    for (u8 i = 0; i < 0xA0; i++)
    {
        ram[A_OAM + i] = get_byte(dma_start + i);
    }
}

/// Stack pointer and Program Counter

// Set pc value
void Memory::set_pc(u16 pc_value)
{
    pc = pc_value;
}

// Get pc value
u16 Memory::get_pc()
{
    return pc;
}

// Set sp value
void Memory::set_sp(u16 sp_value)
{
    sp = sp_value;
}

// Get sp value
u16 Memory::get_sp()
{
    return sp;
}

// Increment pc by amount
void Memory::inc_pc(u8 amount)
{
    pc += amount;
}

// Decrement pc by amount
void Memory::dec_pc(u8 amount)
{
    pc -= amount;
}

// Increment sp by amount
void Memory::inc_sp(u8 amount)
{
    sp += amount;
}

// Decrement sp by amount
void Memory::dec_sp(u8 amount)
{
    sp -= amount;
}

// Add value to sp and update flags
void Memory::sp_add(u8 amount)
{
    u16 old_sp = sp;
    i8 signed_amount = static_cast<i8>(amount);

    // 1. Full 16-bit register update using signed offset
    sp = static_cast<u16>(static_cast<i32>(old_sp) + signed_amount);

    // 2. Hardware flag behavior:
    // ZF and NF are always cleared to 0
    flag_update(ZF, 0);
    flag_update(NF, 0);

    // HF & CF are evaluated strictly as an UNSIGNED 8-bit addition 
    // between (sp & 0xFF) and the raw byte amount:
    u8 sp_low = static_cast<u8>(old_sp & 0xFF);

    // Half Carry: carry out of bit 3 (lower nibble overflow)
    bool half_carry = ((sp_low & 0x0F) + (amount & 0x0F)) > 0x0F;
    flag_update(HF, half_carry ? 1 : 0);

    // Carry: carry out of bit 7 (byte overflow)
    bool carry = ((sp_low & 0xFF) + (amount & 0xFF)) > 0xFF;
    flag_update(CF, carry ? 1 : 0);
}

// Push 16-bit value to stack and decrement sp
void Memory::stack_push(u16 push_val)
{
    dec_sp(1);
    ram[sp] = static_cast<u8>(push_val >> 8);
    dec_sp(1);
    ram[sp] = static_cast<u8>(push_val & 0xFF);
}

// Push PC value on to stack and decrement sp
void Memory::pc_push()
{
    stack_push(pc);
}

// Pop 16-bit value from stack and increment sp
u16 Memory::stack_pop()
{
    u8 low = ram[sp];
    inc_sp(1);
    u8 high = ram[sp];
    inc_sp(1);
    return (static_cast<u16>(high) << 8) | low;
}

// Jump to address at PC + e (e = signed 8-bit immediate)
void Memory::jmp_n(i8 jmp_amount)
{
    i32 pc_val = (i32)pc + (i32)jmp_amount;
    pc = (u16)pc_val;
}
