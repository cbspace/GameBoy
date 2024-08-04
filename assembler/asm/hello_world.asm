; Test Program "Hello World"
; Used to test the gbsem assembler
;
; Display the message on the screen using background tiles.

; Constants - Addresses
A_TDT1    =  $8000      ; Tile Data Table 1 start address (Sprite, BG, Window: 0->255)
A_BGTM1   =  $9800      ; Background Tile Map 1 start address
A_WRAM    =  $c000      ; Working RAM

; Constants - Register Addresses
R_LCDC    =  $ff40      ; LCD Control
R_LCDSTAT =  $ff41      ; LCD Status
R_SCY     =  $ff42      ; Scroll Y
R_SCX     =  $ff43      ; Scroll X
R_LY      =  $ff44      ; LCD Y coordinate
R_LYC     =  $ff45      ; LY Compare
R_DMA     =  $ff46      ; Direct Memory Access
R_BGP     =  $ff47      ; Background Palette

; Constant Values
;V_DMA_ADDR = $90        ; Address of DMA routine

; IRQ Vectors
section "VBLank", home($0040)
    reti
section "LCDC", home($0048)
    reti
section "Timer Overflow", home($0050)
    reti
section "Serial", home($0058)
    reti
section "Joypad", home($0060)
    reti

; ROM Begins at $0100
section "ROM Begin", home($0100)
    nop
    jp  start

; ROM Configuration Data
section "ROM Config", home($0104)
    ; Logo Data
    db $ce, $ed, $66, $66, $cc, $0d, $00, $0b
    db $03, $73, $00, $83, $00, $0c, $00, $0d
    db $00, $08, $11, $1f, $88, $89, $00, $0e
    db $dc, $cc, $6e, $e6, $dd, $dd, $d9, $99
    db $bb, $bb, $67, $63, $6e, $0e, $ec, $cc
    db $dd, $dc, $99, $9f, $bb, $b9, $33, $3e

    ; Game Title - "HELLO"
    db $48,$45,$4c,$4c,$4f,$00,$00,$00
    db $00,$00,$00,$00,$00,$00,$00
    
    ; Game Type = not colour, licence code = 0 (ASCII)
    db $00, $30, $30
    
    ; GB/SGB = GB
    db $00
    
    ; Cartridge Type = ROM ONLY, 32kB, RAM = 0
    db $00, $00, $00
    
    ; Location, licence code, Mask ROM Version
    db $01, $33, $00
    
    ; Complement Check and Checksum (not used)
    db $00, $00, $00

; Beginning of Code
section "Code Begin", home($0150)

start:
    di                   ; Disable Interrupts
    ld sp , $fffe        ; Initialise Stack Pointer
    call stop_lcd        ; Need to stop the LCD to write to display
    
    ; Populate the Tile Data Table
    ld bc , $50          ; 10 characters x 8 bytes each
    ld hl , char_blank
    ld de , A_TDT1
    call copy_data_dbl   ; Copy tile data and double bytes
    
    ; Populate the Tile Map
    ld  bc , $800        ; Tile Map is $400 bytes long per bank
    ld  hl , A_BGTM1     ; Use BGTM1 start address
    call zero_data       ; Set the Tile Map data to zero (blank tiles)
    ld  bc , $40         ; 64 bytes of data to copy
    ld  de , $9900       ; A_BGTM1 + $100
    ld  hl , bgtm_data   ; Tile Map data containing the text
    call copy_data       ; Copy text data
    
    call init_registers  ; Initialise display registers and start LCD
    ei
    
end:
    nop
    jr end
    
init_registers:
    ; Initialise Registers
    xor a
    ld  (R_SCX), a
    ld  (R_SCY), a
    ld  a , %11100100    ; Standard Palette Colours
    ld  (R_BGP), a
    ld  a , %10010011    ; LCD On, Tile Map 0, Window Off, TDT 1, BGTM 0, 8x8, sprite off, BG On
    ld  (R_LCDC), a
    ret
    
; set bc bytes to zero starting from hl
zero_data:
    xor a
    ld  (hli) , a
    dec bc
    ld  a , b
    or  c
    jr  nz , zero_data
    ret
    
; copy bc bytes from (hl) to (de)
copy_data:
    ld  a , (hli)
    ld  (de) , a
    inc de
    dec bc
    ld  a , b
    or  c
    jr  nz , copy_data
    ret
    
; copy bc bytes from (hl) to (de) with each byte copied twice
copy_data_dbl:
    ld  a , (hli)
    ld  (de) , a
    inc de
    ld  (de) , a
    inc de
    dec bc
    ld  a , b
    or  c
    jr  nz , copy_data_dbl
    ret
    
stop_lcd:
    ld  a , (R_LY)
    cp  145
    jr  nz , stop_lcd
    ld  a , (R_LY)
    res 7 , a
    ld  (R_LY) , a
    ret
    
dma_transfer:
    ; Initiate the DMA
    ;ld xy , hl
    ;ld (R_DMA) , hl
    ret
    

; Background Tile Map Data (32x32 bg, 20x18 screen)
; Middle 2 rows shown - h = 1, e = 2 etc
bgtm_data:
    db 0,0,0,0,0,0,0,1,2,3,3,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    db 0,0,0,0,0,0,0,5,4,6,3,7,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

; The below characters represent the outlines.
; Each byte is doubled to make the 1's 3's (darkest colour)
char_blank:
    db 0,0,0,0,0,0,0,0

char_h: 
    db %.1.....1
    db %.1.....1
    db %.1.....1
    db %.1111111
    db %.1.....1
    db %.1.....1
    db %.1.....1
    db %........
    
char_e: 
    db %.1111111
    db %.1......
    db %.1......
    db %.1111111
    db %.1......
    db %.1......
    db %.1111111
    db %........
    
char_l: 
    db %.1......
    db %.1......
    db %.1......
    db %.1......
    db %.1......
    db %.1......
    db %.1111111
    db %........
    
char_o: 
    db %..11111.
    db %.1.....1
    db %.1.....1
    db %.1.....1
    db %.1.....1
    db %.1.....1
    db %..11111.
    db %........

char_w: 
    db %.1..1..1
    db %.1..1..1
    db %.1.1.1.1
    db %.1.1.1.1
    db %.1.1.1.1
    db %.1.1.1.1
    db %..1...1.
    db %........

char_r: 
    db %..11111.
    db %.1.....1
    db %.1.....1
    db %.111111.
    db %.1...1..
    db %.1....1.
    db %.1.....1
    db %........

char_d: 
    db %.111111.
    db %.1.....1
    db %.1.....1
    db %.1.....1
    db %.1.....1
    db %.1.....1
    db %.111111.
    db %........
    
char_bang: 
    db %.11.....
    db %.11.....
    db %.11.....
    db %.11.....
    db %.11.....
    db %........
    db %.11.....
    db %........

; End of 32kB Cartridge
section "ROM Finish", home($8000)
