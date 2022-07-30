cmake --build ./build --config Debug --target clean -j 26
cmake --build ./build --config Debug --target all -j 26
./build/gameboy $1 $2
