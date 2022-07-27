# EmuBoy Gameboy emulator in C++ #

A Gameboy emulator written in C++ using SDL2 rendering. The emulator will be platform portable and is being developed on Linux (G++). Initially ROM ONLY cartridges will be supported with future releases to include Memory Bank Controllers etc.

### Required packages:
..* g++, gcc or build-essential
..* SDL2 (libsdl2-dev on Linux)
..* CMake

#### Build using Cmake (Linux):
1. Install dependencies:

...sudo apt install git cmake g++ libsdl2-dev

2. Clone the repo:

... git clone https://github.com/cbspace/GameBoy.git

2. Build the source (when in GameBoy base directory)

...cmake build

### Command line usage:
gameboy path_to_rom [-dmg]

[-dmg] Option used to run the DMG Boot Rom. This ROM is now working
