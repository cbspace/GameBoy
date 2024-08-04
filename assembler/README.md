# gbsem Gameboy ROM Assembler

 A simple Gameboy DMG ROM Assembler written in Python3.

### Features

 * Wide variey of Assembly syntax accepted
 * Descriptive error messages when errors are found
 
### Future Features

 * Support for macros
 * Listing file generation

### Requirements

 1. Python 3.10 or greater  
 To install (Debian based distros): `sudo apt-get install python3.10`

 2. Files must be executable i.e.  
 `chmod 777 src/*`

### Command line usage (Linux):

`./src/gbsem.py path_to_input [path_to_output]`  
eg. `./src/gbsem.py ~/gb/mygame.asm ~/gb/rom/mygamerom.gb`

Input file: Gameboy assembly code (usually .asm).  
Output file: Gameboy ROM (.gb)

If output filename is not specified it will be the input filename with a .gb extension in the same directory as the input.
