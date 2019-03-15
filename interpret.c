#include "interpret.h"

int debug()
{
    disassemble();
    
    for(int i = 0; i < 0x10; i++)
        printf("register %x: %x\n", i, reg[i]);
    
    printf("register I: %x\n", i);
    printf("register F: %x\n", f);

    printf("(debug) ");
    char input = getchar();
    switch(input)
    {
        case 's':   break;
        default:    break;
    }

    return 0;
}

int interpret()
{
    // debug();
    disassemble();

    uint8_t v, x, y, z, max;
    v = ram[pc] >> 4;
    x = ram[pc] & 0x0f;
    y = ram[pc + 1] >> 4;
    z = ram[pc + 1] & 0x0f;
    max = reg[x] > reg[y] ? reg[x] : reg[y]; 
    
    // main switch
    // the opcodes are stored in big endian in chip-8
    switch(v) 
    {
        case 0x00:
            if(ram[pc + 1] == 0xe0)
            {
                // CLS
                clearScreen();
            }
            else if(ram[pc + 1] == 0xee)
            {
                // RET
                pc = stack[sp];
                sp--;
                return 0;
            }
            else
                printf("invalid operation\n");
            break;
        case 0x01:
            // JMP
            pc = (x << 8) + ram[pc + 1];
            return 1;
        case 0x02:
            // CALL
            sp++;
            stack[sp] = pc + 2;
            pc = (x << 8) + ram[pc + 1];
            return 2;
        case 0x03:
            // SE
            if(reg[x] == ram[pc + 1])
            {
                pc += 4;
                return 3;
            }
            break;
        case 0x04:
            // SNE
            if(reg[x] != ram[pc + 1])
            {
                pc += 4;
                return 4;
            }
            break;
        case 0x05:
            // SE
            if(reg[x] == reg[y])
            {
                pc += 4;
                return 5;
            }
            break;
        case 0x06:
            // LD
            reg[x] = ram[pc + 1];
            break;
        case 0x07:
            // ADD
            reg[x] += ram[pc + 1];
            break;
        case 0x08:
            switch(z)
            {   
                case 0x00:
                    // LD
                    reg[x] = reg[y];
                    break;
                case 0x01:
                    // OR
                    reg[x] |= reg[y];
                    break;
                case 0x02:
                    // AND
                    reg[x] &= reg[y];
                    break;
                case 0x03:
                    // XOR
                    reg[x] ^= reg[y];
                    break;
                case 0x04:
                    // ADD
                    f = (reg[x] + reg[y]) > max ? 0 : 1;
                    reg[x] += reg[y];
                    break;
                case 0x05:
                    // SUB
                    f = (reg[x] == max) ? 1 : 0;
                    reg[x] -= reg[y]; 
                    break;
                case 0x06:
                    // SHR
                    f = (reg[x] & 1) ? 1 : 0;
                    reg[x] >>= 1;
                    break;
                case 0x07:
                    // SUBN
                    f = (reg[y] == max) ? 1 : 0;
                    reg[x] = reg[y] - reg[x];
                    break;
                case 0x08:
                    // SHL
                    f = (reg[x] & 0x80) ? 1 : 0;
                    reg[x] <<= 1;
                    break;
                case 0x0e:
                    // SHL
                    f = (reg[x] & 0x80) ? 1 : 0;
                    reg[x] <<= 1;
                    break;
                default:
                    printf("invalid operation\n");
            }
            break;
        case 0x09:
            // SNE
            if(reg[x] == reg[y])
            {
                pc += 4;
                return 9;
            }
            break;
        case 0x0a:
            // LD
            i = (x << 8) + ram[pc + 1];
            break;
        case 0x0b:
            // JMP
            pc = (x << 8) + ram[pc + 1] + reg[0];
            break;
        case 0x0c:
            // RND
            reg[x] = ((uint8_t)rand()) & ram[pc + 1];
            break;
        case 0x0d:
            // DRW
            f = drawSprite(reg[x], reg[y], z);
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
                    // LD
                    reg[x] = dt;
                    break;
                case 0x0a:
                    // LD
                    reg[x] = k;
                    break;
                case 0x15:
                    // LD
                    dt = reg[x];
                    break;
                case 0x18:
                    // LD
                    st = reg[x];
                    break;
                case 0x1E:
                    // ADD
                    i += reg[x];
                    break;
                case 0x29:
                    // LD
                    i = reg[x] * 5;
                    break;
                case 0x33:
                    // LD
                    ram[i] = reg[x] / 100;
                    ram[i + 1] = (reg[x] % 100) / 10;
                    ram[i + 2] = reg[x] % 10; 
                    break;
                case 0x55:
                    // LD
                    for(int j = 0; j <= x; j++)
                        ram[i + j] = reg[x + j];
                    break;
                case 0x65:
                    // LD
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

    pc += 2;

    return 0;
}