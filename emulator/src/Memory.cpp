#include "Memory.h"

Memory::Memory()
{
    init();
}

void Memory::init()
{
    set_sp(SP_INITIAL_VALUE);    // sp initialised to fffe (gameboy default)
    set_pc(ROM_START_ADDRESS);   // pc initialised to 0x100 (start address)

    for (u8 i = 0; i < REG_ARRAY_SIZE; i++)
    {
        reg[i] = 0;
    }

    for (u32 i = 0; i < MEM_SIZE; i++)
    {
        ram[i] = 0;
    }
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
            reg[RF] = low_byte;
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
    // Make sure reg_id is valid
    if (reg_id < REG_ARRAY_SIZE)
    {
        flag_update(HF,(reg[reg_id] & 0xf0) == ((reg[reg_id] + 1) & 0xf0));
        flag_update(NF,0);
        reg[reg_id]++;
        flag_update(ZF,(reg[reg_id]==0));
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
    // Make sure reg_id is valid
    if (reg_id < REG_ARRAY_SIZE)
    {
        flag_update(HF,(reg[reg_id] & 0xf0) == ((reg[reg_id] - 1) & 0xf0));
        flag_update(NF,0);
        reg[reg_id]--;
        flag_update(ZF,(reg[reg_id]==0));
    }
}

/// Math Functions

// Add value to 8-bit register and update flags, bool parameter is add with carry
void Memory::reg_add(u8 reg_id, u8 add_value, bool carry)
{
    // Make sure reg_id is valid
    if (reg_id < REG_ARRAY_SIZE)
    {
        // Carry is added to accomodate ADDC operations
        u16 add_w_carry = add_value;
        if (carry)
        {
            add_w_carry += flag_get(CF);
        }

        u16 total = reg[reg_id] + add_w_carry;

        flag_update(HF,(reg[reg_id] & 0xf0) == (total & 0xf0));
        flag_update(CF,(total > 0xff));
        flag_update(NF,0);
        reg[reg_id] += add_value + carry;
        flag_update(ZF,(reg[reg_id]==0));
    }
}

// Subtract value from 8-bit register and flags updated, bool brrow used for sub with borrow
void Memory::reg_sub(u8 reg_id, u8 sub_value, bool borrow)
{
    // Make sure reg_id is valid
    if (reg_id < REG_ARRAY_SIZE)
    {
        i16 sub_w_borrow, total;

        // Borrow is subtracted to accomodate SBC operations
        sub_w_borrow = sub_value;
        if (borrow)
        {
            sub_w_borrow -= flag_get(CF);
        }

        total = sub_value - sub_w_borrow;
        flag_update(HF,(reg[reg_id] & 0xf0) == (total & 0xf0));
        flag_update(CF,!(total < 0));
        flag_update(NF,1);
        reg[reg_id] -= sub_value - borrow;
        flag_update(ZF,(reg[reg_id]==0));
    }
}

void Memory::reg_add16(Register16Bit reg_id, u16 reg_n_val)
{
    u32 total = reg_get16(reg_id) + reg_n_val;

    flag_update(HF,(reg_get16(reg_id) & 0xf000) == (total & 0xf000));
    flag_update(CF,(total > 0xffff));
    flag_update(NF,0);

    reg_set(reg_id,(u16)total);

}

// Decimal Adjust register A (DAA), flags updated
void Memory::reg_daa()
{
    // Check if ls digit is valid BCD value
    if ((reg[RA] & 0xf) > 9 || flag_get(HF))
    {
        reg[RA] += 6;
    }
    // Check if ms digit is valid BCD value
    if ((reg[RA] & 0xf0) > 9 || flag_get(CF))
    {
        reg[RA] += 0x60;

        flag_update(CF,1);
    }

    flag_update(HF,0);
    flag_update(ZF,(reg[RA]==0));
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

    flag_update(HF,(reg[RA] & 0xf0) == (total & 0xf0));
    flag_update(CF,!(total < 0));
    flag_update(ZF,(total==0));
    flag_update(NF,1);
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
    ram[address] = byte;

    if (address == R_DMA)
    {
        dma_transfer();
    }
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

// Increment byte and update flags
void Memory::inc_from_pointer(Register16Bit reg_id)
{
    u8 byte_in = get_from_pointer(reg_id);

    flag_update(HF,(byte_in & 0xf0) == ((byte_in + 1) & 0xf0));
    flag_update(NF,0);

    byte_in++;
    set_from_pointer(Register16Bit::HL,byte_in);

    flag_update(ZF,(byte_in==0));
}

// Decrement byte and update flags
void Memory::dec_from_pointer(Register16Bit reg_id)
{
    u8 byte_in = get_from_pointer(reg_id);

    flag_update(HF,(byte_in & 0xf0) == ((byte_in - 1) & 0xf0));
    flag_update(NF,0);

    byte_in--;
    set_from_pointer(Register16Bit::HL,byte_in);
    flag_update(ZF,(byte_in==0));
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
    u8 bit_val = get_from_pointer(reg_id) >> bit_number;

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
        ram[A_OAM + i] = ram[dma_start + i];
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
    u32 total = sp + amount;

    flag_update(HF,(sp & 0xf000) == (total & 0xf000));
    flag_update(CF,(total > 0xffff));
    flag_update(NF,0);
    flag_update(ZF,0);

    sp = (u16)total;
}

// Push 16-bit value to stack and decrement sp
void Memory::stack_push(u16 push_val)
{
    // Push high byte on first then lower byte
    u8 byte_val = push_val >> 8;
    ram[sp] = byte_val;
    byte_val = push_val & 0xff;
    ram[sp-1] = byte_val;
    dec_sp(2);
}

// Push PC value on to stack and decrement sp
void Memory::pc_push()
{
    // Push high byte on first then lower byte
    u8 byte_val = pc >> 8;
    ram[sp] = byte_val;
    byte_val = pc & 0xff;
    ram[sp-1] = byte_val;
    dec_sp(2);
}

// Pop 16-bit value from stack and increment sp
u16 Memory::stack_pop()
{
    // Pop high byte first then lower byte
    u16 ret_val = (ram[sp+2] << 8) + (ram[sp+1] & 0xff);
    inc_sp(2);

    return ret_val;
}

// Jump to address at PC + e (e = signed 8-bit immediate)
void Memory::jmp_n(i8 jmp_amount)
{
    i32 pc_val = (i32)pc + (i32)jmp_amount;
    pc = (u16)pc_val;
}
