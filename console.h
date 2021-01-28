#include "types.h"

#define invalid() printf("Invalid instruction!\n")

typedef struct {
  u8 r[16]; // General purpose registers
  u8 f;     // Flag
  u8 k;     // Key pressed
  u8 st;    // Sound timer
  u8 dt;    // Delay timer
  u8 sp;    // Stack pointer
  u16 i;    // Register to store memory location for sprites
  u16 pc;   // Program counter
  u16 stack[16];
} cpu;

cpu cpu_new();

typedef struct {
  cpu cpu;
  u8 ram[0x1000]; // 4096 bytes of ram
  u8 fb[32 * 64]; // Frame buffer, 64 by 32 pixels, black and white
  u8 keypad[16];
} console;


console console_new();
usize console_load_rom(console *console, char *rom_path);
void console_step(console *console);

void fb_clear(u8 *fb);
u8 fb_draw_sprite(u8 *fb, u8 *sprite, u8 vx, u8 vy, u8 n);

void disassemble(u8 lo, u8 hi);
void disassemble_rom(u8 *rom, usize start, usize len);
