#!/usr/bin/env python3.10

# Gameboy Assembler Program
# Assembler Instructions

from gbsem_common import *
from gbsem_gb_ins import *

# Section, used to indicate address
# Syntax: 'Section "name",HOME[$address]'
def asm_section(params):
    section_addr_str = params[1]
    if section_addr_str[0:4] == 'home':
        s_addr = processSymbol(section_addr_str[4:].strip('()'),16)
        if s_addr >= get_address():
            for i in range(get_address(), s_addr):
                writeIns([0xff])
        else:
            printError("Section address must occur after current address")
    return

# db n - Define byte - Writes single byte to hex file
def asm_db(params):
    # Loop through params as multiple words can be used on a single line
    for b in params:
        int_str = b.replace("l","$") # replace label name with address for disassemblies
        byte1 = processSymbol(int_str,8,True)
        writeIns([byte1])
    return

# dw nn - Define word - Writes 2 bytes to hex file
def asm_dw(params):
    # Loop through params as multiple words can be used on a single line
    for w in params:
        int_str = w.replace("l","$") # replace label name with address for disassemblies
        byte1 = processSymbol(int_str,16,True) & 0xFF
        byte2 = processSymbol(int_str,16,True) >> 8
        writeIns([byte1, byte2])
    return

# include file, currently not supported
def asm_include(params):
    file_name = params[0].strip('""')
    printWarning("Include not supported yet - \"" + file_name + "\" not added",True)
    return

# check for macro instructions
# inputs: instruction and parameters
# outputs: 1 if valid macro found
#          0 otherwise
#lda: MACRO
#    ld a, \2
#    ld \1, a
#ld0: MACRO
#    xor a
#    ld \1, a
#djnz: MACRO
#    dec b
#    jr nz, \1
def asm_macros(instruction,params):
    if instruction == 'lda':
        ins_ld(['a',params[1]],'ld')
        ins_ld([params[0],'a'],'ld')
        return 1
    elif instruction == 'ld0':
        ins_generic_r(0xa8,'xor',['a'])
        ins_ld([params[0],'a'],'ld')
        return 1
    elif instruction == 'djnz':
        ins_dec_inc(0x05,0x0b,'dec',['b'])
        #jr nz, params[0]
        return 1
    else:
        return 0
