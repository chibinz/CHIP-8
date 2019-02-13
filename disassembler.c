#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int disassemble(uint8_t *buffer, uint32_t *pc);

int opcount[0x10];

int main(int argc, char **argv)
{
    //register, timer, program counter
    uint8_t reg[0x0f];
    uint16_t i;
    uint32_t pc; 
    uint32_t rsize;
    memset(opcount, 0, sizeof(int));

    //check number of arguments
    if(argc != 2)
    {
        printf("use: disassembler <rom>\n");
        return 1;
    }
    printf("%s\n", argv[1]);

    //pass rom to file pointer, abort if can not open file
    FILE *rom;
    rom = fopen(argv[1], "rb");
    if(rom == NULL)
    {
        printf("can not open rom\n");
        return 1;
    }
    
    //get romsize
    fseek(rom, 0L, SEEK_END);
    rsize = ftell(rom);
    rewind(rom);
    printf("file size(number of bytes): %u\n", rsize);
    
    //read rom into buffer
    uint8_t *buffer = (uint8_t *)malloc(sizeof(uint8_t) * (rsize + 0x200));
    fread(buffer + 0x200, 1, rsize, rom);
    if(buffer == NULL)
    {
        printf("can not allocate buffer\n");
        return 1;
    }

    uint8_t sprites[0x200] = 
    {
        0xf0, 0x90, 0x90, 0x90, 0xf0,
        0x20, 0x60, 0x20, 0x20, 0x70,
        0xf0, 0x10, 0xf0, 0x80, 0xf0,
        0xf0, 0x10, 0xf0, 0x10, 0xf0,
        0x90, 0x90, 0xf0, 0x10, 0x10,
        0xf0, 0x80, 0xf0, 0x10, 0xf0,
        0xf0, 0x80, 0xf0, 0x90, 0xf0,
        0xf0, 0x10, 0x20, 0x40, 0x40,
        0xf0, 0x90, 0xf0, 0x90, 0xf0,
        0xf0, 0x90, 0xf0, 0x10, 0xf0,
        0xf0, 0x90, 0xf0, 0x90, 0x90,
        0xe0, 0x90, 0xe0, 0x90, 0xe0,
        0xf0, 0x80, 0x80, 0xf0, 0xf0,
        0xe0, 0x90, 0x90, 0x90, 0xe0,
        0xf0, 0x80, 0xf0, 0x80, 0xf0,
        0xf0, 0x80, 0xf0, 0x80, 0x80,
    };

    memcpy(buffer, sprites, 0x200);
    
    //call disassembler to disassemble the rom;
    pc = 0x200;
    while(pc < rsize + 0x200)
    {
        disassemble(buffer, &pc);
        pc += 2;
    }

    for(int i = 0; i < 0x10; i++)
        printf("%x %3d\n", i, opcount[i]);

    fclose(rom);
    free(buffer);
    return 0;
}

int disassemble(uint8_t *buffer, uint32_t *pc)
{ 
    uint8_t n4bit[4];
    n4bit[0] = buffer[*pc] >> 4;
    n4bit[1] = buffer[*pc] & 0x0f;
    n4bit[2] = buffer[*pc + 1] >> 4;
    n4bit[3] = buffer[*pc + 1] & 0x0f; 
    opcount[n4bit[0]]++;
    
    //print program counter location and code
    printf("%03x %02x %02x ", *pc, buffer[*pc], buffer[*pc + 1]);
 
    //opcodes 00e_
    void op0x00()
    {
        if(buffer[*pc + 1] == 0xe0)
            printf("CLS\n");
        else if(buffer[*pc + 1] == 0xee)
            printf("RET\n");
        else
            printf("invalid operation\n");
    }

    //opcodes 8VxVy_
    void op0x08()
    {
        switch(n4bit[3])
        {
            case 0x00:
                printf("LD   V%x V%x\n", n4bit[1], n4bit[2]);
                break;
            case 0x01:
                printf("OR   V%x V%x\n", n4bit[1], n4bit[2]);
                break;
            case 0x02:
                printf("AND  V%x V%x\n", n4bit[1], n4bit[2]);
                break;
            case 0x03:
                printf("XOR  V%x V%x\n", n4bit[1], n4bit[2]);               
                break;
            case 0x04:
                printf("ADD  V%x V%x\n", n4bit[1], n4bit[2]);
                break;
            case 0x05:
                printf("SUB  V%x V%x\n", n4bit[1], n4bit[2]); 
                break;
            case 0x06:
                printf("SHR  V%x\n", n4bit[1]);
                break;
            case 0x07:
                printf("SUBN V%x V%x\n", n4bit[1], n4bit[2]);
                break;
            case 0x08:
                printf("SHL  V%x V%x\n", n4bit[1], n4bit[2]);
                break;
            case 0x0e:
                printf("SHL  V%x V%x\n", n4bit[1], n4bit[2]);
                break;
            default:
                printf("invalid operation\n");
        }
    }

    //opcodes 0xeVx__
    void op0x0e()
    {
        if(buffer[*pc + 1] == 0x9e)
            printf("SKP  V%x\n", n4bit[1]);
        else if(buffer[*pc + 1] = 0xa1)
            printf("SKNP V%x\n", n4bit[1]);
        else    
            printf("invalid operation\n");
    }

    //opcodes 0xfVx__
    void op0x0f()
    {
        switch(buffer[*pc + 1])
        {
            case 0x07:
                printf("LD   V%x DT\n", n4bit[1]);
                break;
            case 0x0a:
                printf("LD   V%x K\n", n4bit[1]);
                break;
            case 0x15:
                printf("LD   DT V%x\n", n4bit[1]);
                break;
            case 0x18:
                printf("LD   ST V%x\n", n4bit[1]);               
                break;
            case 0x1E:
                printf("ADD  I V%x\n", n4bit[1]);
                break;
            case 0x29:
                printf("LD   F V%x\n", n4bit[1]); 
                break;
            case 0x33:
                printf("LD   B V%x\n", n4bit[1]);
                break;
            case 0x55:
                printf("LD   [I] V%x\n", n4bit[1]);
                break;
            case 0x65:
                printf("SHL  V%x [I]\n", n4bit[1]);
                break;
            default:
                printf("invalid operation\n");
        }
    }

    //main switch
    switch (n4bit[0]) //the opcodes are stored in big endian in chip-8
    {
        case 0x00:
            op0x00();
            break;
        case 0x01:
            printf("JMP  %03x\n", (n4bit[1] << 8) + buffer[*pc + 1]);
            break; 
        case 0x02:
            printf("CALL %03x\n", (n4bit[1] << 8) + buffer[*pc + 1]);
            break;
        case 0x03:
            printf("SE   V%x %02x\n", n4bit[1], buffer[*pc + 1]);
            break;
        case 0x04:
            printf("SNE  V%x %02x\n", n4bit[1], buffer[*pc + 1]);
            break;
        case 0x05:
            printf("SE   V%x V%x\n", n4bit[1], n4bit[2]);
            break;
        case 0x06:
            printf("LD   V%x %02x\n", n4bit[1], buffer[*pc + 1]);
            break;
        case 0x07:
            printf("ADD  V%x %02x\n", n4bit[1], buffer[*pc + 1]);
            break;
        case 0x08:
            op0x08();
            break;
        case 0x09:
            printf("SNE  V%x V%x\n", n4bit[1], n4bit[2]);
            break;
        case 0x0a:
            printf("LD   I %03x\n", (n4bit[1] << 8) + buffer[*pc + 1]);
            break;
        case 0x0b:
            printf("JMP  %03x + V0\n", (n4bit[1] << 8) + buffer[*pc + 1]);
            break;
        case 0x0c:
            printf("RND  V%x %02x\n", n4bit[1], buffer[*pc + 1]);
            break;
        case 0x0d:
            printf("DRW  V%x V%x %x\n", n4bit[1], n4bit[2], n4bit[3]);
            break;
        case 0x0e:
            op0x0e();
            break;
        case 0x0f:
            op0x0f();
    }

    return 0;
}