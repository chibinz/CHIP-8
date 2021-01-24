#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "ram.h"
#include "rom.h"
#include "interpret.h"
#include "disassemble.h"

int main(int argc, char **argv)
{
    //check number of arguments
    if(argc != 2)
    {
        printf("Use: interpreter <rom>\n");
        exit(-1);
    }

    initCPU();
    initRAM();
    readROM(argv[1]);

    return 0;
}
