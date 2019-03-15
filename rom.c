#include "rom.h"

void readROM(char *file)
{
    // pass rom to file pointer, abort if can not open file
    FILE *rom;
    rom = fopen(file, "rb");
    if(rom == NULL)
    {
        printf("Can not open rom\n");
        exit(-1);
    }
    
    // get romsize
    fseek(rom, 0L, SEEK_END);
    rsize = ftell(rom);
    rewind(rom);

    // read rom into ram
    fread(ram + 0x200, 1, rsize, rom);

    printf("Rom name: %s\n", file);
    printf("File size(number of bytes): %u\n", rsize);
 
    fclose(rom);
}