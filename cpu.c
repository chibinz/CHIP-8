#include "cpu.h"

void initCPU(void)
{
    for(int i = 0; i < 0x10; i++)
    {
        reg[i]   = 0;
        stack[i] = 0;
    }

    f   =     0;
    k   =     0;
    st  =     0;
    dt  =     0;
    i   =     0;
    sp  =    -1;
    pc  = 0x200;

}