# EmuBoy Gameboy emulator in C++ #

A Gameboy emulator written in C++ using the Qt6 framework. The emulator will be platform portable and is being developed on Linux (G++). Initially ROM ONLY cartridges will be supported with future releases to include Memory Bank Controllers etc.

### Required packages:
- g++, gcc or build-essential
- Qt6
- CMake

#### Build using Cmake (Linux):
1. Install dependencies:

   `sudo apt install git cmake g++ qt6-base-dev`

2. Clone the repo:

   `git clone https://github.com/cbspace/GameBoy.git`

3. `cd GameBoy`

4. `mkdir build`

5. Configure CMake

`./build.sh -configure`

6. Build and run the source (when in GameBoy base directory)

   `./build.sh -build` or to build and run `./build.sh -run [params]`

### Command line usage:
`./build/gameboy /path/to/rom.gb [-debug]`

Or if you wish to run with the DMG Boot ROM

`./build/gameboy /path/to/rom.gb [-dmg] /path/to/dmg_rom.bin [-debug]`

`[-dmg]` Option used to run the DMG Boot Rom. This ROM is now working
`[-debug]` Option will run with the Debugger window enabled
