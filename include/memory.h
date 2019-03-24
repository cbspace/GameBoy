#ifndef MEMORY_H
#define MEMORY_H


class memory
{

    int ram[0x10000];      // The RAM

    /* Memory Map
                                               _
    0000 - 3FFF     16kB ROM Bank #0            |   32kB Cartridge
    4000 - 7FFF     16kB Switchable ROM Bank   _|
    8000 - 9FFF     8kB Video RAM
    A000 - BFFF     8kB Switchable RAM Bank
    C000 - DFFF     8kB Internal RAM
    E000 - FDFF     Echo of 8kB Internal RAM
    FE00 - FE9F     Sprite Attrib Memory (0AM)
    FEA0 - FEFF     Empty but unusable for I/O
    FF00 - FF3F     I/O Ports
    FF4C - FF7F     Empty but unusable for I/O
    FF80 - FFEF     Internal RAM
    FFFF            Interrupt Register

    */

    public:
        memory();
};

#endif // MEMORY_H
