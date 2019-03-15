#include <stdint.h>

uint8_t reg[0x10];      // 16 general purpose registers
uint16_t stack[0x10];   // 16 level stack

uint8_t f;              // flag
uint8_t k;              // key pressed
uint8_t st;             // sound timer
uint8_t dt;             // delay timer
uint16_t i;             // register to store memory location for sprites
uint16_t sp;            // stack pointer
uint16_t pc;            // program counter

void initCPU(void);