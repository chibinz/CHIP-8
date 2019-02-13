#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "TerminalGraphics.h"

#define FORM_WIDTH 64
#define FORM_LENGTH 32

int interpret();
int debug();
int draw(uint8_t, uint8_t, uint8_t);

int opcount[0x10];

uint8_t reg[0x10];      //general purpose registers
uint8_t ram[0x1000];    //4K of ram
uint16_t stack[0x10];   //16 level stack

uint8_t f;              //flag
uint8_t k;              //key pressed
uint8_t st;             //sound timer
uint8_t dt;             //delay timer
uint16_t i;             //register to store memory location for sprites
uint16_t sp;            //stack pointer
uint16_t pc;            //program counter
uint16_t rsize;         //rom size

int main(int argc, char **argv)
{
    memset(opcount, 0, sizeof(int));
    f = 0;

    //check number of arguments
    if(argc != 2)
    {
        printf("use: interpreter <rom>\n");
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
    
    //read rom into ram
    fread(ram + 0x200, 1, rsize, rom);

    uint8_t sprites[0x200] = 
    {
        0xf0, 0x90, 0x90, 0x90, 0xf0,   //0
        0x20, 0x60, 0x20, 0x20, 0x70,   //1 
        0xf0, 0x10, 0xf0, 0x80, 0xf0,   //2
        0xf0, 0x10, 0xf0, 0x10, 0xf0,   //3
        0x90, 0x90, 0xf0, 0x10, 0x10,   //4
        0xf0, 0x80, 0xf0, 0x10, 0xf0,   //5
        0xf0, 0x80, 0xf0, 0x90, 0xf0,   //6
        0xf0, 0x10, 0x20, 0x40, 0x40,   //7
        0xf0, 0x90, 0xf0, 0x90, 0xf0,   //8
        0xf0, 0x90, 0xf0, 0x10, 0xf0,   //9
        0xf0, 0x90, 0xf0, 0x90, 0x90,   //A
        0xe0, 0x90, 0xe0, 0x90, 0xe0,   //B
        0xf0, 0x80, 0x80, 0xf0, 0xf0,   //C
        0xe0, 0x90, 0x90, 0x90, 0xe0,   //D
        0xf0, 0x80, 0xf0, 0x80, 0xf0,   //E
        0xf0, 0x80, 0xf0, 0x80, 0x80,   //F
    };

    memcpy(ram, sprites, 0x200);
    
    //call interpreter to interpret the rom;
    pc = 0x200;
    while(pc < rsize + 0x200)
    {
        interpret();
        pc += 2;
    }

    for(int i = 0; i < 0x10; i++)
        printf("%x %3d\n", i, opcount[i]);

    fclose(rom);
    return 0;
}

int interpret(void)
{ 
    //debug();
    uint8_t v, x, y, z, max;
    v = ram[pc] >> 4;
    x = ram[pc] & 0x0f;
    y = ram[pc + 1] >> 4;
    z = ram[pc + 1] & 0x0f;
    max = reg[x] > reg[y] ? reg[x] : reg[y]; 
    opcount[v]++;
    
    //print program counter location and code
    printf("%03x %02x %02x ", pc, ram[pc], ram[pc + 1]);

    //main switch
    //the opcodes are stored in big endian in chip-8
    switch(v) 
    {
        case 0x00:
            if(ram[pc + 1] == 0xe0)
            {
                printf("CLS\n");
                for(int j = 0; j < FORM_LENGTH; j++)
                    for(int k = 0; k < FORM_WIDTH; k++)
                        graph[j][k] = 0;
            }
            else if(ram[pc + 1] == 0xee)
                printf("RET\n");
            else
                printf("invalid operation\n");
            break;
        case 0x01:
            printf("JMP  %03x\n", (x << 8) + ram[pc + 1]);
            break; 
        case 0x02:
            printf("CALL %03x\n", (x << 8) + ram[pc + 1]);
            break;
        case 0x03:
            printf("SE   V%x %02x\n", x, ram[pc + 1]);
            break;
        case 0x04:
            printf("SNE  V%x %02x\n", x, ram[pc + 1]);
            break;
        case 0x05:
            printf("SE   V%x V%x\n", x, y);
            break;
        case 0x06:
            printf("LD   V%x %02x\n", x, ram[pc + 1]);
            reg[x] = ram[pc + 1];
            break;
        case 0x07:
            printf("ADD  V%x %02x\n", x, ram[pc + 1]);
            reg[x] += ram[pc + 1];
            break;
        case 0x08:
            switch(z)
            {   
                case 0x00:
                    printf("LD   V%x V%x\n", x, y);
                    reg[x] = reg[y];
                    break;
                case 0x01:
                    printf("OR   V%x V%x\n", x, y);
                    reg[x] |= reg[y];
                    break;
                case 0x02:
                    printf("AND  V%x V%x\n", x, y);
                    reg[x] &= reg[y];
                    break;
                case 0x03:
                    printf("XOR  V%x V%x\n", x, y);               
                    reg[x] ^= reg[y];
                    break;
                case 0x04:
                    printf("ADD  V%x V%x\n", x, y);
                    f = (reg[x] + reg[y]) > max ? 0 : 1;
                    reg[x] += reg[y];
                    break;
                case 0x05:
                    printf("SUB  V%x V%x\n", x, y);
                    f = (reg[x] == max) ? 1 : 0;
                    reg[x] -= reg[y]; 
                    break;
                case 0x06:
                    printf("SHR  V%x\n", x);
                    f = (reg[x] & 1) ? 1 : 0;
                    reg[x] >>= 1;
                    break;
                case 0x07:
                    printf("SUBN V%x V%x\n", x, y);
                    f = (reg[y] == max) ? 1 : 0;
                    reg[x] = reg[y] - reg[x];
                    break;
                case 0x08:
                    printf("SHL  V%x V%x\n", x, y);
                    f = (reg[x] & 0x80) ? 1 : 0;
                    reg[x] <<= 1;
                    break;
                case 0x0e:
                    printf("SHL  V%x V%x\n", x, y);
                    f = (reg[x] & 0x80) ? 1 : 0;
                    reg[x] <<= 1;
                    break;
                default:
                    printf("invalid operation\n");
            }
            break;
        case 0x09:
            printf("SNE  V%x V%x\n", x, y);
            break;
        case 0x0a:
            printf("LD   I %03x\n", (x << 8) + ram[pc + 1]);
            i = (x << 8) + ram[pc + 1];
            break;
        case 0x0b:
            printf("JMP  %03x + V0\n", (x << 8) + ram[pc + 1]);
            break;
        case 0x0c:
            printf("RND  V%x %02x\n", x, ram[pc + 1]);
            reg[x] = rand() & ram[pc + 1];
            break;
        case 0x0d:
            printf("DRW  V%x V%x %x\n", x, y, z);
            draw(x, y, z);
            break;
        case 0x0e:
            if(ram[pc + 1] == 0x9e)
                printf("SKP  V%x\n", x);
            else if(ram[pc + 1] == 0xa1)
                printf("SKNP V%x\n", x);
            else    
                printf("invalid operation\n");
            break;
        case 0x0f:
            switch(ram[pc + 1])
            {
                case 0x07:
                    printf("LD   V%x DT\n", x);
                    reg[x] = dt;
                    break;
                case 0x0a:
                    printf("LD   V%x K\n", x);
                    reg[x] = k;
                    break;
                case 0x15:
                    printf("LD   DT V%x\n", x);
                    dt = reg[x];
                    break;
                case 0x18:
                    printf("LD   ST V%x\n", x);               
                    st = reg[x];
                    break;
                case 0x1E:
                    printf("ADD  I V%x\n", x);
                    i += reg[x];
                    break;
                case 0x29:
                    printf("LD   F V%x\n", x); 
                    i = reg[x] * 5;
                    break;
                case 0x33:
                    printf("LD   B V%x\n", x);
                    ram[i] = reg[x] / 100;
                    ram[i + 1] = (reg[x] % 100) / 10;
                    ram[i + 2] = reg[x] % 10; 
                    break;
                case 0x55:
                    printf("LD   [I] V%x\n", x);
                    for(int j = 0; j <= x; j++)
                        ram[i + j] = reg[x + j];
                    break;
                case 0x65:
                    printf("LD   V%x [I]\n", x);
                    for(int j = 0; j <= x; j++)
                        reg[x + j] = ram[i + j];
                    break;
                default:
                    printf("invalid operation\n");
            }
            break;
        default:
            printf("invalid operation\n");
    }

    return 0;
}

int debug()
{
    for(int i = 0; i < 0x10; i++)
        printf("register %x: %x\n", i, reg[i]);
    
    printf("register I: %x\n", i);
    printf("register F: %x\n", f);

    return 0;
}

int draw(uint8_t x, uint8_t y, uint8_t n)
{
    printf("\n\n\n");
    for(int j = 0; j < n; j++)
    {
        uint8_t mask = 0b10000000;
        for(int k = 0; k < 8; k++)
        {
            graph[x + k][y + j] ^= ram[i + j] & mask;
            mask >>= 1;
        }
    }

    for(int j = 0; j < FORM_LENGTH; j++, putchar('\n'))
        for(int k = 0; k < FORM_WIDTH; k++)
            if(graph[k][j])
                putchar('#');
            else
                putchar(' ');

    printf("\n\n\n");
    return 0;
}