#pragma once

#include "displayconst.h"
#include "LibCB/Error.h"
#include "LibCB/Types.h"
#include <string>
#include <optional>
#include <fstream>
#include <iostream>

const u32 MEM_SIZE = 0x10000;           // Memory size for Gameboy
const u32 REG_ARRAY_SIZE = 8;           // Number of 8 bit registers
const u32 BYTE_ARRAY_SIZE = 16;         // Used in read_rom_title and write_array

const u32 ROM_START_ADDRESS = 0x100;    // ROM program starts at this address (DMG Rom Start is 0)
const u32 ROM_TITLE_ADDRESS = 0x134;    // Address of title

const u32 SP_INITIAL_VALUE = 0xfffe;    // Initial value of the stack pointer

const u8 RA = 0;                        // A register - These register IDs are mapped to the reg array
const u8 RF = 1;                        // F register
const u8 RB = 2;                        // B register
const u8 RC = 3;                        // C register
const u8 RD = 4;                        // D register
const u8 RE = 5;                        // E register
const u8 RH = 6;                        // H register
const u8 RL = 7;                        // L register

const u32 ZF = 0x80;                    // Zero flag mask
const u32 NF = 0x40;                    // Negative flag mask
const u32 HF = 0x20;                    // Half carry flag mask
const u32 CF = 0x10;                    // Full carry flag mask

const u32 A_OAM =               0xfe00;     // Start address of OAM memory (Sprite Atrribute Table)

using namespace std;

enum class Register16Bit {
    AF,
    BC,
    DE,
    HL
};

class Memory
{
    public:
        Memory();
        void init();
        // Registers
        u8 reg_get(u8 reg_id);
        u16 reg_get16(Register16Bit reg_id);        
        void reg_set(u8 reg_id, u8 reg_value);
        void reg_set(Register16Bit reg_id, u16 reg_value);
        void reg_copy(u8 from_reg_id, u8 to_reg_id);            // Copy data between 8-bit registers
        void reg_inc(u8 reg_id);                                // Increment 8-bit register, flags updated
        void reg_dec(u8 reg_id);                                // Decrement 8-bit register, flags updated
        void reg_inc16(Register16Bit reg_id);                   // Increment 16-bit register, flags not updated
        void reg_dec16(Register16Bit reg_id);                   // Decrement 16-bit register, flags not updated
        // Math
        void reg_add(u8 reg_id, u8 add_value, bool = 0);        // Add value to 8-bit register and update flags, bool parameter is add with carry
        void reg_sub(u8 reg_id, u8 sub_value, bool borrow = 0); // Subtract value from 8-bit register and update flags, bool param is borrow with carry
        void reg_add16(Register16Bit reg_id, u16 reg_n_val);    // Add 2 16-bit registers and update flags
        void reg_daa();                                         // Decimal Adjust register A (DAA), flags updated
        void reg_rla(bool carry);                               // Rotate contents of A register left and store bit 7 in CF, flags updated
        void reg_rl(u8 reg_id, bool carry);                     // Rotate contents of register n left and store bit 7 in CF, flags updated
        void reg_rra(bool carry);                               // Rotate contents of A register right and store bit 0 in CF, flags updated
        void reg_rr(u8 reg_id, bool carry);                     // Rotate contents of register n right and store bit 0 in CF, flags updated
        void reg_sla(u8 reg_id);                                // Shift contents of register n left and store bit 7 in CF, bit0 = 0, flags updated
        void reg_sra(u8 reg_id);                                // Shift contents of register A right and store bit 0 in CF, bit7 unchanged, flags updated
        void reg_srl(u8 reg_id);                                // Shift contents of register A right and store bit 0 in CF, bit7 = 0, flags updated
        // Logical
        void reg_and(u8 and_value);                             // Register A is ANDed with and_value, result stored in register A - flags updated
        void reg_or(u8 or_value);                               // Register A is ORed with and_value, result stored in register A - flags updated
        void reg_xor(u8 xor_value);                             // Register A is XORed with and_value, result stored in register A - flags updated
        void reg_compare(u8 cmp_value);                         // Register A is compared with cmp_value, result stored in register A - flags updated
        void reg_swap(u8 reg_id);                               // Swap upper and lower nibbles of register, flags updated
        void bit_test(u8 reg_id, u8 bit_number);                // Test bit b in register and set flags accordingly
        void bit_set(u8 reg_id, u8 bit_number);                 // Set bit in register, flags not affected
        void bit_res(u8 reg_id, u8 bit_number);                 // Reset bit in register, flags not affected
        // RAM/ROM      
        u8 get_byte(u16 address);                               // Read byte from RAM/ROM
        u8 get_bit(u16 address, u8 bit_number);                 // Read a bit from an byte in RAM/ROM
        u8 fetch_byte();                                        // Read byte from ROM and increment PC
        u8 get_from_pointer(Register16Bit reg_id);              // Read byte from RAM/ROM pointed to by 16-bit register
        void set_from_pointer(Register16Bit reg_id, u8 byte_value);       // Set byte at RAM address pointed to by 16-bit register to byte value
        void write_byte(u16 address, u8 byte);                            // Write byte to RAM/ROM
        void write_bit(u16 address, u8 bit_number, bool bit_val);         // Update individual bit in RAM/ROM
        void inc_from_pointer(Register16Bit reg_id);                      // Increment byte at (n) and update flags
        void dec_from_pointer(Register16Bit reg_id);                      // Decrement byte at (n) and update flags
        void swap_from_pointer(Register16Bit reg_id);                     // Swap upper and lower nibbles of byte at (n), flags updated
        void rl_from_pointer(Register16Bit reg_id, bool with_carry);
        void rr_from_pointer(Register16Bit reg_id, bool with_carry);
        void sla_from_pointer(Register16Bit reg_id);                      // Shift contents of byte at (n) left and store bit 7 in CF, bit0 = 0, flags updated
        void sra_from_pointer(Register16Bit reg_id);                      // Shift contents byte at (n) right and store bit 0 in CF, bit7 unchanged, flags updated
        void srl_from_pointer(Register16Bit reg_id);                      // Shift contents byte at (n) right and store bit 0 in CF, bit7 = 0, flags updated
        void bit_test_from_pointer(Register16Bit reg_id, u8 bit_number);  // Test bit b in byte at (n) and set flags accordingly
        void bit_set_from_pointer(Register16Bit reg_id, u8 bit_number);   // Set bit in in byte at (n), flags not affected
        void bit_res_from_pointer(Register16Bit reg_id, u8 bit_number);   // Reset bit in in byte at (n), flags not affected
        void dma_transfer();                                              // Perform DMA transfer from ROM/RAM to OAM
        // Stack pointer and Program Counter
        void set_pc(u16 pc_value);                             // Set pc value
        u16 get_pc();                                          // Get pc value
        void set_sp(u16 sp_value);                             // Set sp value
        u16 get_sp();                                          // Get sp value
        void inc_pc(u8 amount);                                // Increment pc by amount
        void dec_pc(u8 amount);                                // Decrement pc by amount
        void inc_sp(u8 amount);                                // Increment sp by amount
        void dec_sp(u8 amount);                                // Secrement sp by amount
        void sp_add(u8 amount);                                // Add value to sp and update flags
        void stack_push(u16 push_val);                         // Push 16bit value to stack and decrement sp
        void pc_push();                                        // Push PC value on to stack and decrement sp
        u16 stack_pop();                                       // Pop 16-bit value from stack and increment sp
        void jmp_n(i8 jmp_amount);                             // Jump to address at PC + e (e = signed 8-bit immediate)
        // Flags
        void flag_update(u8 flag_id, u8 flg_val);              // Set or clear flag
        u8 flag_get(u8 flag_id);                               // Return flag value
        // ROM Cartridge and game title
        optional<Error> load_rom(string rom_path);             // Function to load a ROM file
        void read_rom_title();                                 // Read rom title and load into string
        string get_rom_title();                                // Get the current ROM title

    private:
        char ram[MEM_SIZE];                                    // 64kB RAM
        u8 reg[REG_ARRAY_SIZE];                                // Registers: A/F, B/C, D/E, H/L
        u16 sp, pc;                                            // Stack pointer and program counter
        string rom_title;                                      // Title of the current game ROM file

};

/* Flags register (RF)

    7   6   5   4   3   2   1   0
    Z   N   H   C   0   0   0   0

    Z = Zero flag - set when result of mathematical operation is zero
        or when results using CP compare instruction match
    N = Subtraction flag - set when subtraction was performed in the last
        math operation
    H = Half carry flag - set when there is an overflow from low nibble to
        high nible in the last math operation
    C = Carry flag - set if a carry occured from last math operation or if
        register A is the smaller value when executing CP instruction
*/

/* Memory Map
                                            _
0000 - 3FFF     16kB ROM Bank #0            |   32kB Cartridge
4000 - 7FFF     16kB Switchable ROM Bank   _|
8000 - 9FFF     8kB Video RAM
A000 - BFFF     8kB Switchable RAM Bank
C000 - DFFF     8kB Internal RAM
E000 - FDFF     Echo of 8kB Internal RAM
FE00 - FE9F     Sprite Attrib Memory (0AM)
FEA0 - FEFF     Empty but unusable for I/O
FF00 - FF3F     I/O Ports
FF4C - FF7F     Empty but unusable for I/O
FF80 - FFEF     Internal RAM
FFFF            Interrupt Register

Reserved Memory Locations

Restart addressess: RST $xx calls address
0000 Restart $00 Address
0008 Restart $08 Address
0010 Restart $10 Address
0018 Restart $18 Address
0020 Restart $20 Address
0028 Restart $28 Address
0030 Restart $30 Address
0038 Restart $38 Address

Interrupt Addresses
0040 Vertical Blank Interrupt Start Address
0048 LCDC Status Interrupt Start Address
0050 Timer Overflow Interrupt Start Address
0058 Serial Transfer Completion Interrupt Start Address
0060 High-to-low of P10-P13 Interrupt Start Address

Internal Information Area in Cartridge

0100 - 0103 Begin code execution point. Usually contains a NOP
            and a JP but not always.

0104 - 0133 Scrolling Logo graphic
            ce ed 66 66  cc 0d 00 0b 03 73 00 83
00 0c 00 0d 00 08 11 1f  88 89 00 0e dc cc 6e e6
dd dd d9 99 bb bb 67 63  6e 0e ec cc dd dc 99 9f
bb b9 33 3e

0134 - 0142 Title of the game in UPPERCASE ASCII
0143        $80 = Color GB, $00 or other = not Color GB
0144        Ascii hex digit licencee code (high)
0144        Ascii hex digit licencee code (high)
0146        GS/GB Indicator (00 = GameBoy, 03 = Super GameBoy

0147        Cartridge Type
0   ROM ONLY                12  ROM+MBC3+RAM
1   ROM+MBC1                13  ROM+MBC3+RAM+BATT
2   ROM+MBC1+RAM            19  ROM+MBC5+RAM
3   ROM+MBC1+RAM+BATT       1A  ROM+MBC5+RAM
5   ROM+MBC2                1B  ROM+MBC5+RAM+BATT
6   ROM+MBC2+BATTERY        1C  ROM+MBC5+RUMBLE
8   ROM+RAM                 1D  ROM+MBC5+RUMBLE+SRAM
9   ROM+RAM+BATTERY         1E  ROM+MBC5+RUMBLE+SRAM+BATT
B   ROM+MMMO1               1F  Pocket Camera
C   ROM+MMMO1+SRAM          FS  Bandai TAMA5
D   ROM+MMMO1+SRAM+BATT     FE  Hudson HuC-3
F   ROM+MBC3+TIMER+BATT     FF  Hudson HuC-1
10  ROM+MBC3+TIMER+RAM+BATT
11  ROM+MBC3

0148        ROM size
0   256Kbit   =  32KB  =   2 banks
1   512Kbit   =  62KB  =   4 banks
2     1Mbit   = 128KB  =   8 banks
3     2Mbit   = 256KB  =  16 banks
4     4Mbit   = 512KB  =  32 banks
5     8Mbit   =   1MB  =  64 banks
6     16Mbit  =   2MB  = 128 banks
$52    9Mbit  = 1.1MB  =  72 banks
$53   10Mbit  = 1.2MB  =  80 banks
$54   12Mbit  = 1.5MB  =  96 banks

0149        RAM Size
0   None
1    16Kbit   =   2KB  =   1 bank
2    64Kbit   =   8KB  =   1 bank
3   256Kbit   =  32KB  =   4 banks
4     1Mbit   = 128KB  =  16 banks

014A        Destination Code (0 Japan, 1 Non-Japanese)
014B        Licensee code (old)
014C        Mask ROM Version Number (usually $00)
014D        Complement check
014E - 014F Checksum

*/
