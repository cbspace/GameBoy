#!/usr/bin/env python3.10

# Gameboy Assembler Program
# Gameboy Instructions

from gbsem_constants import *
from gbsem_common import *

# Instruction with r parameter (r=LIST_PARAM)
# and r - and r with A
# cp r - Compare r with A
def ins_generic_r(base_opcode,ins_name,params):
    if len(params) == 1:
        r = params[0]
        if r in LIST_PARAM:
            writeIns([base_opcode + LIST_PARAM.index(r)])
        else: #number
            write_n(base_opcode + 0x46,r)
    else:
        printError("Invalid use of instruction - " + ins_name + " r - expecting 1 parameter")
    return

# Instruction with r parameter (r=LIST_PARAM)
# Opcode is 0xcb followed by base_opcode + reg index
# Used by: RLC n, RL n, RRC n, RR n, SLA n, SRA n, SRL n, SWAP r
def ins_generic_cb_r(base_opcode,ins_name,params):
    if len(params) == 1:
        r = params[0]
        if r in LIST_PARAM:
            writeIns([0xcb, base_opcode + LIST_PARAM.index(r)])
        else: #error
            printError("Invalid use of instruction - " + ins_name + " r - invalid parameter")
    else:
        printError("Invalid use of instruction - " + ins_name + " r - expecting 1 parameter")
    return

# Instructions inc r and dec r with r parameter (r=LIST_PARAM)
# dec r - decrement register r
# inc r - increment register r
def ins_generic_decn_incn(base_opcode,ins_name,params):
    if len(params) == 1:
        r = params[0]
        if r in LIST_PARAM:
            writeIns([base_opcode + 0x08 * LIST_PARAM.index(r)])
        else:
            printError("Invalid use of '" + ins_name + " r' - incorrect parameter")
    else:
        printError("Invalid use of - " + ins_name + " r - too many parameters")
    return

# Load instructions
# ------ Simple Loads use list match ------
# ld (hl-),a      - SAME AS LDD (HL),A - 0x32
# ld (hld),a      - SAME AS LDD (HL),A - 0x32
# ldd (hl),a      - Put a into address (hl) and decrement hl - 0x32
# ld (hl+),a      - SAME AS LDI (HL),A - 0x22
# ld (hli),a      - SAME AS LDI (HL),A - 0x22
# ldi (hl),a      - Put a into address (hl) and increment hl - 0x22
# ld a,(hl-)      - SAME AS LDD A,(HL) - 0x3a
# ld a,(hld)      - SAME AS LDD A,(HL) - 0x3a
# ldd a,(hl)      - Put value at address (hl) into a and decrement hl - 0x3a
# ld a,(hl+)      - SAME AS LDI (HL),A - 0x2a
# ld a,(hli)      - SAME AS LDI (HL),A - 0x2a
# ldi a,(hl)      - Put value at address (hl) into a and increment hl - 0x2a
# ld ($ff00+c),a  - Put A into address $FF00 + reg c - 0xe2
# ld a,($ff00+c)  - Put value at $FF00 + reg c into A - 0xf2
# ld sp,hl        - Put hl into stack pointer - 0xf9
# ld r1,r2        - r1 and r2 in LIST_PARAM
# ------ Loads using registers and immediates------
# ld a,n          - n= abcdehl(bc)(de)(hl)(d16)d8
# ld n,a          - n= abcdehl(bc)(de)(hl)(d16)
# ld r1,r2        - (hl),n w n=8bit Immediate
# ld n,nn         - Put nn into n (n=LIST_PARAM_REG_S),(nn=d16)
# ld nn,n         - nn = bcdehl , n = 8bit Immediate
# ld hl,sp+n      - SAME AS LDHL SP,N - 0xf8
# ld (nn),sp      - Put stack pointer at address (nn) nn=d16 - 0x08
def ins_ld(params,ins_name):
    if len(params) == 2:
        match_result = ins_ld_match(params,ins_name)
        if match_result != -1: # found a match
            writeIns([match_result])
        elif params[0] == 'a':
            n = params[1]
            if n == 'a':
                writeIns([0x7f]) # ld a,a
            else:
                if n in LIST_PARAM_LDA:
                    writeIns([LIST_LD_AN_OPCODE[LIST_PARAM_LDA.index(n)]])
                else: #(nn)
                    if n[0] == '(' and n[-1] == ')':
                        n = n.strip('()')
                        n_int = processSymbol(n,16)
                        if n_int != -1:
                            write_nn(0xfa,n_int)
                    else: # 8bit immediate
                        write_n(0x3e,n)
        elif params[1] == 'a':
            n = params[0]
            if n in LIST_PARAM_LDA:
                writeIns([LIST_LD_NA_OPCODE[LIST_PARAM_LDA.index(n)]])
            else: #(nn)
                if n[0] == '(' and n[-1] == ')':
                    n = n.strip('()')
                    nn_int = processSymbol(n,16,False,False)
                    if nn_int == -1:
                        nn_int = processAddress(n,'ld')
                    write_nn(0xea,nn_int)
                else: # no valid parameter
                    printError("Invalid use of instruction - ld n,a")
        elif params[0] == '(hl)': # ld (hl),n w n=8bit Immediate
            write_n(0x36,params[1])
        elif params[0] in LIST_PARAM_REG_S: # ld n,nn
            if ins_ldhl_spn(params) != 1:
                nn_int = processSymbol(params[1],16,False,False)
                if nn_int == -1:
                    nn_int = processAddress(params[1],'ld',params[0])
                write_nn(LIST_LD_RS_OPCODE[LIST_PARAM_REG_S.index(params[0])],nn_int)
        elif params[0] in LIST_PARAM_LDN: # ld nn,n
            write_n(0x06 + 0x08 * LIST_PARAM_LDN.index(params[0]),params[1])
        elif params[0][0] == '(' and params[0][-1] == ')' and params[1] == 'sp':
            nn = params[0].strip('()')
            nn_int = processSymbol(nn,16)
            if nn != -1:
                write_nn(0x08,nn_int)
        else:
            printError("Invalid use of instruction '" + ins_name + "' - invalid parameters")
    else:
        printError("Invalid use of instruction '" + ins_name + "' - only allowed 2 parameters")
    return

# check if instruction matches pattern for ld hl,sp+n
# inputs: parameter list
# return 1 on success or 0 on no match
def ins_ldhl_spn(params):
    if params[0] == 'hl' and len(params[1]) > 3:
        if params[1][0:3] == 'sp+':
            n = params[1][3:]
            write_n(0xf8,n)
            return 1
    return 0

# Test for simple ld commands and match to opcode
# Returns -1 for no match and returns opcode for a match (8bit integer)
def ins_ld_match(params,ins_name):
    match_value = -1
    for i in LIST_LD:
        if ins_name == i[0][0] and params[0] == i[0][1] and params[1] == i[0][2]:
            match_value = i[1]
    return match_value

# ldh (n),a   - Put a into memory address $FF00 + n (8bit immediate) - 0xe0
# ldh a,(n)   - Put address $FF00 + n (8bit immediate) into a - 0xf0
def ins_ldh(params):
    p0 = params[0]
    p1 = params[1]
    if len(params) == 2:
        if p1 == 'a':
            if ins_ldh_param(p0,0xe0) == -1:
                printError("Invalid use of 'ldh' - invalid parameters")
        elif p0 == 'a':
            if ins_ldh_param(p1,0xf0) == -1:
                printError("Invalid use of 'ldh' - invalid parameters")
        else:
            printError("Invalid use of 'ldh' - invalid parameters")
    else:
        printError("Invalid use of 'ldh' - expecting 2 parameters")
    return
    
# process the '($ff00+n)' or (n) string
# return 1 on success and -1 on error
def ins_ldh_param(in_str,opcode):
    if in_str[0] == '(' and in_str[-1] == ')':
        s = in_str.strip('()')
        if s.count('+') == 1 and len(s) > 6:
            if s[0:6] == '$ff00+':
                n = s[6:]
                write_n(opcode,n)
                return 1
        else:
            n_value = processSymbol(s,8)
            if n_value != -1:
                writeIns([opcode,n_value])
                return 1
    return -1

# ldhl sp,n   - Put sp + n effective address into hl (n=d8) - 0xf8
def ins_ldhl(params):
    if len(params) == 2 and params[0] =='sp':
        write_n(0xf8,params[1])
    else:
        printError("Invalid use of instruction 'ldhl sp,n'")
    return

# adc - add n + carry flag to A
# sbc - subtract n + carry flag from A
def ins_adc_sbc(base_opcode,ins_name,params):
    if len(params) == 2 and params[0] =='a':
        n = params[1]
        if n in LIST_PARAM:
            writeIns([base_opcode + LIST_PARAM.index(n)])
        else: #number
            write_n(base_opcode + 0x46,n)
    else:
        printError("Invalid use of instruction - '" + ins_name + "' a,n - expecting 1 parameter")
    return

# add n to A
def ins_add(params):
    if len(params) == 2:
        if params[0] =='a':
            ins_generic_r(0x80,'add',[params[1]])
        elif params[0] == 'hl':
            n = params[1]
            if n in LIST_PARAM_REG_S:
                writeIns([0x09 + 0x10 * LIST_PARAM_REG_S.index(n)])
            else: #error
                printError("Register '" + params[1] + "' is invalid")
        elif params[0] == 'sp':
            write_n(0xe8,params[1])
        else:
            printError("Invalid use of instruction - ADD a,n")
    else:
        printError("Invalid use of instruction 'add a,n' - only allowed 2 parameters")
    return

# generic function for:
# BIT b,r , RES b,r , SET b,r , 
def ins_bit_generic(base_opcode,ins_name,params):
    if len(params) == 2 and len(params[0]) == 1 and params[0].isdigit():
        b = int(params[0])
        r = params[1]
        if b <= 7:
            if r in LIST_PARAM:
                writeIns([0xcb,base_opcode + 0x08 * b + LIST_PARAM.index(r)])
        else:
            printError("Invalid bit number, must be 0 - 7")
    else:
        printError("Invalid use of instruction - '" + ins_name +"' b,r")
    return

# call
def ins_call(params):
    if len(params) == 1:  # call nn - Call subroutine at address nn
        nn = processAddress(params[0],'call')
        byte1 = 0xcd
    elif len(params) == 2: # call cc,nn conditional call to address nn (cc=nz,z,nc,c)
        cc = params[0]
        if cc in LIST_CONDITIONS:
            nn = processAddress(params[1],'call',cc)
            byte1 = LIST_CALL_OPCODE[LIST_CONDITIONS.index(cc)]
        else:
            printError("Call condition is not valid (cc=nz,z,nc,c)")
    write_nn(byte1, nn)
    return

# Decrement register r or
# Increment register r
def ins_dec_inc(base_opcode_r,base_opcode_rr,ins_name,params):
    if len(params) == 1:
        r = params[0]
        if r in LIST_PARAM_REG_S:
            writeIns([base_opcode_rr + 0x10 * LIST_PARAM_REG_S.index(r)])
        else:
            ins_generic_decn_incn(base_opcode_r,ins_name,params)
    else:
        printError("Invalid use of '"+ ins_name + " r' - too many parameters")
    return


# Jump
# JP nn(unsigned d16)
# JP cc(cc=nz,z,nc,c),nn(unsigned d16)
# JP (hl)
def ins_jp(params):
    if len(params) == 1:
        if params[0] == '(hl)':  # jp (HL) - Jump to address in HL register
            writeIns([0xe9])
        else:    # jp nn - Jump to address nn
            nn = processAddress(params[0],'jp')
            write_nn(0xc3, nn)
    elif len(params) == 2: # jp cc,nn conditional jump to address nn (cc=nz,z,nc,c)
        cc = params[0]
        if cc in LIST_CONDITIONS:
            nn = processAddress(params[1],'jp',cc)
            byte1 = LIST_JP_OPCODE[LIST_CONDITIONS.index(cc)]
            write_nn(byte1, nn)
        else:
            printError("Jump condition is not valid (cc=nz,z,nc,c)")
    else:
        printError("Invalid use of 'jp' - expected 1 or 2 parameters")
    return
    
# Jump Relative
# JR n (signed d8)
# JR cc(cc=nz,z,nc,c),n(signed d8)
def ins_jr(params):
    if len(params) == 1: # jr n (signed d8)
        if params[0][0] == '.': # dot label
            n_value = processAddressRelative(params[0].split('.')[1])
            if n_value == -1000: # Error
                printError("Invalid .label")
            elif n_value == 0: # Label is found so leave instruction blank
                writeIns([0x00, 0x00])
        else: # label or number ###
            n = processAddress(params[0],'jr')
            if n == -1: # Error
                printError("Invalid address or label")
            elif n == -2: # Label is found so leave instruction blank
                writeIns([0x00, 0x00])
            else:          # Address is found so write the bytes
                writeIns([0x18, n])
    elif len(params) == 2: # jr cc(cc=nz,z,nc,c),n(signed d8)
        cc = params[0]
        if cc in LIST_CONDITIONS:
            if params[1][0] == '.': # dot label
                n_value = processAddressRelative(params[1].split('.')[1],cc)
                if n_value == -1000: # Error
                    printError("Invalid .label")
                elif n_value == 0: # Label is found so leave instruction blank
                    writeIns([0x00, 0x00])
            else: # label or number ###
                n = processAddress(params[1],'jr',cc)
                byte1 = LIST_JR_OPCODE[LIST_CONDITIONS.index(cc)]
                if n == -1: # Error
                    printError("Invalid address or label")
                elif n == -2: # Label is found so leave instruction blank
                    writeIns([0x00, 0x00])
                else:          # Address is found so write the bytes
                    writeIns([byte1, n])
        else:
            printError("Jump condition is not valid (cc=nz,z,nc,c)")
    else:
        printError("Invalid use of 'jr' - expected 1 or 2 parameters")
    return

# Write ROM data for instruction with nn parameter
# Input: byte1 - the first byte (opcode)
#           nn - The processed integer
def write_nn(byte1, nn):
    if nn == -1: # Error
        printError("Invalid address or label")
    elif nn == -2: # Label is found so leave instruction blank
        writeIns([0x00, 0x00, 0x00])
    else:          # Address is found so write the bytes
        byte3 = nn >> 8
        byte2 = nn & 0xFF
        writeIns([byte1, byte2, byte3])
    return

# Write ROM data for instruction with n parameter
# Input: byte1 - the first byte (opcode)
#            n - The processed integer
def write_n(byte1, n):
    n_value = processSymbol(n,8)
    if n_value != -1:
        writeIns([byte1,n_value])
    return

# ret or ret cc - Return from subroutine or retun conditional (cc=LIST_CONDITIONS)
def ins_ret(params):
    if len(params) == 0: # ret unconditional
        writeIns([0xc9])
    elif len(params) == 1: # ret conditional (cc=nz,z,nc,c)
        cc = params[0]
        if cc in LIST_CONDITIONS:
            byte1 = 0xc0 + 0x08 * LIST_CONDITIONS.index(cc)
            writeIns([byte1])
        else:
            printError("Ret condition is not valid (cc=nz,z,nc,c)")
    else:
        printError("Invalid use of 'ret' - 0 or 1 parameters only")
    return

# pop and push to stack
def ins_stack(base_opcode,ins_name,params):
    if len(params) == 1:
        r = params[0]
        if r in LIST_PARAM_STACK:
            byte1 = base_opcode + 0x10 * LIST_PARAM_STACK.index(r)
            writeIns([byte1])
    else:
        printError("Invalid use of '"+ ins_name + " r' - expected 1 parameter")
    return
    
# Push current address to stack and jp to address + n
# n = $00,$08,$10,$18,$20,$28,$30,$38
def ins_rst(params):
    if len(params) == 1:
        n_int = processSymbol(params[0],8)
        if n_int != -1:
            if n_int in LIST_RST_VALUES:
                byte1 = 0xc7 + 0x08 * LIST_RST_VALUES.index(n_int)
                writeIns([byte1])
            else:
                printError("Invalid use of 'rst n' - invalid reset register value")
    else:
        printError("Invalid use of 'rst n' - expected 1 parameter")
    return
