#!/usr/bin/env python3.10

# Gameboy Assembler Program
# Constants

# Conditions
LIST_CONDITIONS = ['nz','z','nc','c']

# JP, JR and CALL Opcodes for Conditions
LIST_JP_OPCODE = [0xc2,0xca,0xd2,0xda]
LIST_JR_OPCODE = [0x20,0x28,0x30,0x38]
LIST_CALL_OPCODE = [0xc4,0xcc,0xd4,0xdc]

# Opcodes for Load Instructions
LIST_LD_AN_OPCODE = [0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x0a,0x1a,0x7e]
LIST_LD_NA_OPCODE = [0x47,0x4f,0x57,0x5f,0x67,0x6f,0x02,0x12,0x77]
LIST_LD_RS_OPCODE = [0x01,0x11,0x21,0x31]

# Registers
LIST_PARAM = ['b','c','d','e','h','l','(hl)','a']
LIST_PARAM_LDA = ['b','c','d','e','h','l','(bc)','(de)','(hl)']
LIST_PARAM_LDN = ['b','c','d','e','h','l']
LIST_PARAM_REG_I = ['(bc)','(de)','(hl)']
LIST_PARAM_REG_S = ['bc','de','hl','sp']
LIST_PARAM_STACK = ['bc','de','hl','af']

# Reset Vectors
LIST_RST_VALUES = [0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38]

# Simple Loads use list match
LIST_LD = [
[['ld','(hl-)','a'],0x32],
[['ld','(hld)','a'],0x32],
[['ldd','(hl)','a'],0x32],
[['ld','(hl+)','a'],0x22],
[['ld','(hli)','a'],0x22],
[['ldi','(hl)','a'],0x22],
[['ld','a','(hl-)'],0x3a],
[['ld','a','(hld)'],0x3a],
[['ldd','a','(hl)'],0x3a],
[['ld','a','(hl+)'],0x2a],
[['ld','a','(hli)'],0x2a],
[['ldi','a','(hl)'],0x2a],
[['ld','($ff00+c)','a'],0xe2],
[['ld','a','($ff00+c)'],0xf2],
[['ld','(c)','a'],0xe2],
[['ld','a','(c)'],0xf2],
[['ld','sp','hl'],0xf9],
[['ld','b','b'],0x40],
[['ld','b','c'],0x41],
[['ld','b','d'],0x42],
[['ld','b','e'],0x43],
[['ld','b','h'],0x44],
[['ld','b','l'],0x45],
[['ld','b','(hl)'],0x46],
[['ld','c','b'],0x48],
[['ld','c','c'],0x49],
[['ld','c','d'],0x4a],
[['ld','c','e'],0x4b],
[['ld','c','h'],0x4c],
[['ld','c','l'],0x4d],
[['ld','c','(hl)'],0x4e],
[['ld','d','b'],0x50],
[['ld','d','c'],0x51],
[['ld','d','d'],0x52],
[['ld','d','e'],0x53],
[['ld','d','h'],0x54],
[['ld','d','l'],0x55],
[['ld','d','(hl)'],0x56],
[['ld','e','b'],0x58],
[['ld','e','c'],0x59],
[['ld','e','d'],0x5a],
[['ld','e','e'],0x5b],
[['ld','e','h'],0x5c],
[['ld','e','l'],0x5d],
[['ld','e','(hl)'],0x5e],
[['ld','h','b'],0x60],
[['ld','h','c'],0x61],
[['ld','h','d'],0x62],
[['ld','h','e'],0x63],
[['ld','h','h'],0x64],
[['ld','h','l'],0x65],
[['ld','h','(hl)'],0x66],
[['ld','l','b'],0x68],
[['ld','l','c'],0x69],
[['ld','l','d'],0x6a],
[['ld','l','e'],0x6b],
[['ld','l','h'],0x6c],
[['ld','l','l'],0x6d],
[['ld','l','(hl)'],0x6e],
[['ld','(hl)','b'],0x70],
[['ld','(hl)','c'],0x71],
[['ld','(hl)','d'],0x72],
[['ld','(hl)','e'],0x73],
[['ld','(hl)','h'],0x74],
[['ld','(hl)','l'],0x75],
]
