#ifndef MEMORY_H
#define MEMORY_H

#define MEM_SIZE            0x10000     // Memory size for Gameboy
#define REG_ARRAY_SIZE      8           // Number of 8 bit registers
#define BYTE_ARRAY_SIZE     16          // Used in read_rom_title and write_array

#define ROM_START_ADDRESS   0x000 //0x100      // ROM program starts at this address
#define ROM_TITLE_ADDRESS   0x134       // Address of title

#define SP_INITIAL_VALUE    0xfffe      // Initial value of the stack pointer

#define RA  0                           // A register - These register IDs are mapped to the reg array
#define RF  1                           // F register
#define RB  2                           // B register
#define RC  3                           // C register
#define RD  4                           // D register
#define RE  5                           // E register
#define RH  6                           // H register
#define RL  7                           // L register

#define REG_16_START  8                 // Start of 16-bit register IDs

#define RAF  9                          // AF register - These register IDs are arbitrary and not directly mapped to registers
#define RBC  10                         // BC register
#define RDE  11                         // DE register
#define RHL  12                         // HL register

#define REG_16_END    13                // End of 16-bit register IDs

#define ZF     0x80                     // Zero flag mask
#define NF     0x40                     // Negative flag mask
#define HF     0x20                     // Half carry flag mask
#define CF     0x10                     // Full carry flag mask


#include <stdint.h>
#include <string>
using namespace std;

class Memory
{
    public:
        Memory();                                                   // Constructor
        void init();                                                // Initialise registers
        /// Registers
        uint8_t reg_get(uint8_t reg_id);                            // Get contents of 8 bit register
        uint16_t reg_get16(uint8_t reg_id);                         // Get contents of 16 bit register
        void reg_set(uint8_t reg_id, uint8_t reg_value);            // Set value in 8 bit register
        void reg_set(uint8_t reg_id, uint16_t reg_value);           // Set value in 16 bit register
        void reg_copy(uint8_t from_reg_id, uint8_t to_reg_id);      // Copy data between 8-bit registers
        void reg_inc(uint8_t reg_id);                               // Increment 8-bit register, flags updated
        void reg_dec(uint8_t reg_id);                               // Decrement 8-bit register, flags updated
        void reg_inc16(uint8_t reg_id);                             // Increment 16-bit register, flags not updated
        void reg_dec16(uint8_t reg_id);                             // Decrement 16-bit register, flags not updated
        /// Math
        void reg_add(uint8_t reg_id, uint8_t add_value, bool = 0);  // Add value to 8-bit register and update flags, bool parameter is add with carry
        void reg_sub(uint8_t reg_id, uint8_t sub_value, bool borrow = 0);   // Subtract value from 8-bit register and update flags, bool param is borrow with carry
        void reg_add16(uint8_t reg_id, uint16_t reg_n_val);         // Add 2 16-bit registers and update flags
        void reg_daa();                                             // Decimal Adjust register A (DAA), flags updated
        void reg_rla(bool carry);                                   // Rotate contents of A register left and store bit 7 in CF, flags updated
        void reg_rl(uint8_t reg_id, bool carry);                    // Rotate contents of register n left and store bit 7 in CF, flags updated
        void reg_rra(bool carry);                                   // Rotate contents of A register right and store bit 0 in CF, flags updated
        void reg_rr(uint8_t reg_id, bool carry);                    // Rotate contents of register n right and store bit 0 in CF, flags updated
        void reg_sla(uint8_t reg_id);                               // Shift contents of register n left and store bit 7 in CF, bit0 = 0, flags updated
        void reg_sra(uint8_t reg_id);                               // Shift contents of register A right and store bit 0 in CF, bit7 unchanged, flags updated
        void reg_srl(uint8_t reg_id);                               // Shift contents of register A right and store bit 0 in CF, bit7 = 0, flags updated
        /// Logical
        void reg_and(uint8_t and_value);                            // Register A is ANDed with and_value, result stored in register A - flags updated
        void reg_or(uint8_t or_value);                              // Register A is ORed with and_value, result stored in register A - flags updated
        void reg_xor(uint8_t xor_value);                            // Register A is XORed with and_value, result stored in register A - flags updated
        void reg_compare(uint8_t cmp_value);                        // Register A is compared with cmp_value, result stored in register A - flags updated
        void reg_swap(uint8_t reg_id);                              // Swap upper and lower nibbles of register, flags updated
        void bit_test(uint8_t reg_id, uint8_t bit_number);          // Test bit b in register and set flags accordingly
        void bit_set(uint8_t reg_id, uint8_t bit_number);           // Set bit in register, flags not affected
        void bit_res(uint8_t reg_id, uint8_t bit_number);           // Reset bit in register, flags not affected
        /// RAM/ROM
        uint8_t get_byte(uint16_t address);                         // Read byte from RAM/ROM
        uint8_t fetch_byte();                                       // Read byte from ROM and increment PC
        uint8_t get_from_pointer(uint8_t reg_id);                   // Read byte from RAM/ROM pointed to by 16-bit register
        void set_from_pointer(uint8_t reg_id, uint8_t byte_value);  // Set byte at RAM address pointed to by 16-bit register to byte value
        void write_byte(uint16_t address, uint8_t byte);            // Write byte to RAM/ROM
        void inc_from_pointer(uint8_t reg_id);                      // Increment byte at (n) and update flags
        void dec_from_pointer(uint8_t reg_id);                      // Decrement byte at (n) and update flags
        void swap_from_pointer(uint8_t reg_id);                     // Swap upper and lower nibbles of byte at (n), flags updated
        void rl_from_pointer(uint8_t reg_id, bool carry);           // Rotate contents of byte at (n) left and store bit 7 in CF, flags updated
        void rr_from_pointer(uint8_t reg_id, bool carry);           // Rotate contents of byte at (n) right and store bit 0 in CF, flags updated
        void sla_from_pointer(uint8_t reg_id);                      // Shift contents of byte at (n) left and store bit 7 in CF, bit0 = 0, flags updated
        void sra_from_pointer(uint8_t reg_id);                      // Shift contents byte at (n) right and store bit 0 in CF, bit7 unchanged, flags updated
        void srl_from_pointer(uint8_t reg_id);                      // Shift contents byte at (n) right and store bit 0 in CF, bit7 = 0, flags updated
        void bit_test_from_pointer(uint8_t reg_id, uint8_t bit_number); // Test bit b in byte at (n) and set flags accordingly
        void bit_set_from_pointer(uint8_t reg_id, uint8_t bit_number);  // Set bit in in byte at (n), flags not affected
        void bit_res_from_pointer(uint8_t reg_id, uint8_t bit_number);  // Reset bit in in byte at (n), flags not affected
        /// Stack pointer and Program Counter
        void set_pc(uint16_t pc_value);                             // Set pc value
        uint16_t get_pc();                                          // Get pc value
        void set_sp(uint16_t sp_value);                             // Set sp value
        uint16_t get_sp();                                          // Get sp value
        void inc_pc(uint8_t amount);                                // Increment pc by amount
        void dec_pc(uint8_t amount);                                // Decrement pc by amount
        void inc_sp(uint8_t amount);                                // Increment sp by amount
        void dec_sp(uint8_t amount);                                // Secrement sp by amount
        void sp_add(uint8_t amount);                                // Add value to sp and update flags
        void stack_push(uint16_t push_val);                         // Push 16bit value to stack and decrement sp
        void pc_push();                                             // Push PC value on to stack and decrement sp
        uint16_t stack_pop();                                       // Pop 16-bit value from stack and increment sp
        void jmp_n(int8_t jmp_amount);                              // Jump to address at PC + e (e = signed 8-bit immediate)
        /// Flags
        void flag_update(uint8_t flag_id, uint8_t flg_val);         // Set or clear flag
        uint8_t flag_get(uint8_t flag_id);                          // Return flag value
        /// ROM Cartridge and game title
        int8_t load_rom(char* rom_path);                            // Function to load a ROM file
        void read_rom_title();                                      // Read rom title and load into string
        string get_rom_title();                                     // Get the current ROM title

    private:
        char ram[MEM_SIZE];                                         // 64kB RAM
        uint8_t reg[REG_ARRAY_SIZE];                                // Registers: A/F, B/C, D/E, H/L
        uint16_t sp, pc;                                            // Stack pointer and program counter
        string rom_title;                                           // Title of the current game ROM file

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

    0104 - 0133 Scrolling Nintendo graphic
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

};

#endif // MEMORY_H
