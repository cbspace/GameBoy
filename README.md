# EmuBoy Gameboy emulator in C++ #

A Gameboy emulator written in C++ using SDL2 rendering. The emulator will be platform portable and is being developed on Linux (G++). Initially ROM ONLY cartridges will be supported with future releases to include Memory Bank Controllers etc.

### Required packages:
- g++, gcc or build-essential
- SDL2 (libsdl2-dev on Linux)
- CMake

#### Build using Cmake (Linux):
1. Install dependencies:

   `sudo apt install git cmake g++ libsdl2-dev`

2. Clone the repo:

   `git clone https://github.com/cbspace/GameBoy.git`

3. `cd GameBoy`

4. `mkdir build`

5. Configure CMake

`cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ -S./ -B./build -G "Unix Makefiles"`

6. Build the source (when in GameBoy base directory)

   `./run.sh`

### Command line usage:
`./build/gameboy /path/to/rom.gb [-debug]`

Or if you wish to run with the DMG Boot ROM

`./build/gameboy /path/to/rom.gb [-dmg] /path/to/dmg_rom.bin [-debug]`

`[-dmg]` Option used to run the DMG Boot Rom. This ROM is now working
`[-debug]` Option will run with the Debugger window enabled
