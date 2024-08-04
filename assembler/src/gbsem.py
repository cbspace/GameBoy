#!/usr/bin/env python3.10

# Gameboy Assembler Program

# Constants
CONST_VERSION = "0.40"

import sys, os

from gbsem_constants import *
from gbsem_common import *
from gbsem_asm_ins import *
from gbsem_gb_ins import *

# ------------------------- Program Start -----------------------------

# Display Welcome Message
print("Gameboy Assembler V" + CONST_VERSION)

# Check the command line arguments
if not (2 <= len(sys.argv) <= 3):
    print("    Command Line Usage:")
    print("    \"./gbsem.py SourceFilePath [RomFilePath]\"")
    print("    SourceFilePath: Gameboy source file, usually a .asm file extension")
    print("     [RomFilePath]: Optional parameter used to specify output ROM file name.")
    print("                    If omitted, the ROM file name is the input file name with")
    print("                    a .gb extension")
    print("    e.g \"gbsem.py source/test.asm roms/test_rom.gb\"")
elif os.path.exists(sys.argv[1]):
    # Store file names from command line
    filename_in = str(sys.argv[1])
    
    # Determine out file name based on command line
    if len(sys.argv) == 3:    # Out file name was provided
        filename_out = str(sys.argv[2])
    else:                     # Out file name not provided
        slash_pos = filename_in.rfind('/')
        if slash_pos == -1:
            slash_pos = 0
        if filename_in.find('.', slash_pos + 1) > 0:
            filename_out = filename_in[:filename_in.rfind('.')] + '.gb'
        else:
            filename_out = filename_in + '.gb'

    # Open input file readonly
    infile = open(filename_in, 'r')

    # Main loop
    for line in infile.readlines():
        # Read a line from the file and strip whitespaces and cr + lf
        data = line.strip()
        
        # Increase line counter
        inc_line_number()
        
        # Strip away comments
        data = data.split(';')[0]
        data = data.strip()
        
        # If line is empty then move on
        if len(data) == 0: continue
        
        # Make line lowercase to remove any case issues
        data = data.lower()
        
        # Replace tabs with spaces to simplify parsing
        data = data.replace('\t',' ')
        
        # Look for constants defined using 'EQU' or '='
        const_check = checkForConstant(data)
        if const_check == 1 or const_check == -1:
            continue
        
        # Look for label delimiter
        label_count = data.count(':')
        
        # If a lablel is found process it then process remainder of the line
        if label_count > 1:
            printError("Invalid label definition (':' can only occur once per line)")
        elif label_count == 1:
            label_split = data.split(':')
            defineLabel(label_split[0])
            fillJrIns()
            data = label_split[1].strip()
            # If line is empty then move on
            if len(data) == 0: continue
            
        # Look for 'dot labels' (eg .loop)
        # need to fix conflict with .db and .dw instructions
        if data[0] == '.' and data[0:3] != '.db' and data[0:3] != '.dw':
            label_split = data.split('.')
            if len(label_split) == 2:
                defineDotLabel(label_split[1])
            else:
                printError(".label definition invalid")
            continue
        
        #Separate instruction from parameters
        space_count = data.count(' ')

        # Find the instruction
        if space_count == 0:
            instruction = data
            params = []
        elif space_count > 0:
            first_space_split = data.split(' ',1)
            instruction = first_space_split[0]    
            param_string = first_space_split[1].strip()
            # Find parameters (separated by commas)
            params = param_string.split(',')
        
        # Get rid of any pesky spaces, tabs, etc
        # and replace [] brackets with ()
        for i in range(len(params)):
            params[i] = params[i].strip()
            params[i] = params[i].replace('[','(')
            params[i] = params[i].replace(']',')')

        # Process the instruction
        match instruction:
            case "db": # db n - Define byte - Writes single byte to hex file
                asm_db(params)
            case '.db':
                asm_db(params)
            case 'dw': # dw nnnn - Define word - Writes 2 bytes to hex file
                asm_dw(params)
            case '.dw':
                asm_dw(params)
            case 'include': # include file, currently not supported
                asm_include(params)
            case 'section': # section, used to indicate address
                asm_section(params)
            case 'nop': # No op
                writeIns([0x00])
            case 'adc': # add r + carry flag to A
                ins_adc_sbc(0x88,'adc',params)
            case 'add': # add r to A
                ins_add(params)
            case 'and': # and r with A
                ins_generic_r(0xa0,'and',params)
            case 'bit': # Test bit b in register r - bit b,r
                ins_bit_generic(0x40,'bit',params)
            case 'call': # call
                ins_call(params)
            case 'ccf': # Complement Carry Flag
                writeIns([0x3f])
            case 'cpl': # Complement A register
                writeIns([0x2f])
            case 'cp': # cp r - Compare r with A
                ins_generic_r(0xb8,'cp',params)
            case 'daa': # Decimal adjust register A
                writeIns([0x27])
            case 'dec': # Decrement register r
                ins_dec_inc(0x05,0x0b,'dec',params)
            case 'di': # Disable Interrupts
                writeIns([0xf3])
            case 'ei': # Enable Interrupts
                writeIns([0xfb])
            case 'halt': # Halt
                writeIns([0x76])
            case 'inc': # Increment register r
                ins_dec_inc(0x04,0x03,'inc',params)
            case 'jp': # Jump
                ins_jp(params)
            case 'jr': # Jump relative
                ins_jr(params)
            case 'ld': # Load
                ins_ld(params,'ld')
            case 'ldd': # Load and decrement
                ins_ld(params,'ldd')
            case 'ldh': # Load high
                ins_ldh(params)
            case 'ldi': # Load and increment
                ins_ld(params,'ldi')
            case 'ldhl': # Put sp + n effective address into hl (n=d8) - 0xf8
                ins_ldhl(params)
            case 'nop': # No op
                writeIns([0x00])
            case 'or': # Or r with A
                ins_generic_r(0xb0,'or',params)
            case 'pop': # Pop from stack
                ins_stack(0xc1,'pop',params)
            case 'push': # Push to stack
                ins_stack(0xc5,'push',params)
            case 'res': # Reset bit b in register r - res b,r
                ins_bit_generic(0x80,'res',params)
            case 'ret': # ret - Return from subroutine - ret or ret cc
                ins_ret(params)
            case 'reti': # ret - Return from interrupt - 0xD9
                writeIns([0xd9])
            case 'rla': # rla - Rotate A left through carry flag
                writeIns([0x17])
            case 'rlca': # rla - Rotate A left. Old bit 7 to carry flag
                writeIns([0x07])
            case 'rlc': # rlc - Rotate n left. Old bit 7 to carry flag
                ins_generic_cb_r(0x00,'rlc',params)
            case 'rl': # Rotate n left through Carry flag
                ins_generic_cb_r(0x10,'rl',params)
            case 'rra': # rra - Rotate A right through carry flag
                writeIns([0x1f])
            case 'rrca': # rrca - Rotate A right. Old bit 0 to carry flag
                writeIns([0x0f])
            case 'rrc': # rrc - Rotate n right. Old bit 0 to carry flag
                ins_generic_cb_r(0x08,'rrc',params)
            case 'rr': # Rotate n right through Carry flag
                ins_generic_cb_r(0x18,'rr',params)
            case 'rst': # Push current address to stack and jp to address + n
                ins_rst(params)
            case 'sbc': # subtract n + carry flag from A
                ins_adc_sbc(0x98,'sbc',params)
            case 'scf': # Set carry flag
                writeIns([0x37])
            case 'set': # Set bit b in register r - set b,r
                ins_bit_generic(0xc0,'set',params)
            case 'sla': # Shift n left into Carry. LSB of n set to 0.
                ins_generic_cb_r(0x20,'sla',params)
            case 'sra': # Shift n right into Carry - no change to MSB
                ins_generic_cb_r(0x28,'sra',params)
            case 'srl': # Shift n right into Carry - MSB set to 0
                ins_generic_cb_r(0x38,'srl',params)
            case 'stop': # Halt CPU and display until button press
                writeIns([0x10,0x00])
            case 'sub': # Subtract n from a
                ins_generic_r(0x90,'sub',params)
            case 'swap': # Swap upper and lower nibbles of n
                ins_generic_cb_r(0x30,'swap',params)
            case 'xor': # xor a with result in a
                ins_generic_r(0xa8,'xor',params)
            case _:
                #check for macro instructions
                if asm_macros(instruction,params) == 0:
                    printError("Invalid instruction '" + instruction + "'")

    # Fill in remaining jr instruction dot label addresses
    fillJrIns()
    
    # Fill in the jump instructions with associated label addresses
    fillJumps()

    # Display errors or finalise the rom
    finalise_rom(filename_out)

    # Close infile
    infile.close()
else:
    printError("Input file: \"" + sys.argv[1] + "\" not found", False)
