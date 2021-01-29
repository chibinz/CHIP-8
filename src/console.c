#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "console.h"

static const u8 characters[0x200] = {
    0xf0, 0x90, 0x90, 0x90, 0xf0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xf0, 0x10, 0xf0, 0x80, 0xf0, // 2
    0xf0, 0x10, 0xf0, 0x10, 0xf0, // 3
    0x90, 0x90, 0xf0, 0x10, 0x10, // 4
    0xf0, 0x80, 0xf0, 0x10, 0xf0, // 5
    0xf0, 0x80, 0xf0, 0x90, 0xf0, // 6
    0xf0, 0x10, 0x20, 0x40, 0x40, // 7
    0xf0, 0x90, 0xf0, 0x90, 0xf0, // 8
    0xf0, 0x90, 0xf0, 0x10, 0xf0, // 9
    0xf0, 0x90, 0xf0, 0x90, 0x90, // A
    0xe0, 0x90, 0xe0, 0x90, 0xe0, // B
    0xf0, 0x80, 0x80, 0x80, 0xf0, // C
    0xe0, 0x90, 0x90, 0x90, 0xe0, // D
    0xf0, 0x80, 0xf0, 0x80, 0xf0, // E
    0xf0, 0x80, 0xf0, 0x80, 0x80, // F
};

cpu cpu_new() {
  return (cpu){
      .r = {0},
      .sp = 0,
      .i = 0,
      .pc = 0x200,
      .stack = {0},
  };
}

console console_new() {
  console chip = {.cpu = cpu_new(),
                  .dt = 0,
                  .st = 0,
                  .tick = 0,
                  .ram = {0},
                  .fb = {0},
                  .keypad = {0}};

  memcpy(chip.ram, characters, sizeof(characters));

  return chip;
}

void console_tick(console *console) {
  console_cpu_tick(console);
  console_timer_tick(console);
}

// CHIP-8 CPU runs at about 500 Hz
// Timer frequency is 60 Hz
// 500 / 60 = 8
void console_timer_tick(console *console) {
  console->tick += 1;

  if (console->tick == 8) {
    console->tick = 0;
    console->dt = console->dt == 0 ? 0 : console->dt - 1;
    console->st = console->st == 0 ? 0 : console->st - 1;
  }
}

void console_cpu_tick(console *console) {
  u8 *ram = console->ram;
  u8 *keypad = console->keypad;
  u16 *stack = console->cpu.stack;
  cpu *cpu = &console->cpu;

  u8 lo, hi, v, x, y, z;
  lo = ram[cpu->pc];
  hi = ram[cpu->pc + 1];
  v = lo >> 4;
  x = lo & 0x0f;
  y = hi >> 4;
  z = hi & 0x0f;

  u16 nnn = ((u16)x << 8) | hi;

  // disassemble(lo, hi);

  // PC gets incremented first
  cpu->pc += 2;

  // Main switch
  // The opcodes are stored in big endian in chip-8
  switch (v) {
  case 0x00:
    if (hi == 0xe0) { // CLS
      fb_clear(console->fb);
    } else if (hi == 0xee) { // RET
      cpu->sp -= 1;
      cpu->pc = stack[cpu->sp];
    } else {
      invalid();
    }
    break;
  case 0x01: // JMP
    cpu->pc = nnn;
    break;
  case 0x02: // CALL
    stack[cpu->sp] = cpu->pc;
    cpu->pc = nnn;
    cpu->sp += 1;
    break;
  case 0x03: // SE
    if (cpu->r[x] == hi) {
      cpu->pc += 2;
    }
    break;
  case 0x04: // SNE
    if (cpu->r[x] != hi) {
      cpu->pc += 2;
    }
    break;
  case 0x05: // SE
    if (cpu->r[x] == cpu->r[y]) {
      cpu->pc += 2;
    }
    break;
  case 0x06: // LD
    cpu->r[x] = hi;
    break;
  case 0x07: // ADD
    cpu->r[x] += hi;
    break;
  case 0x08:
    switch (z) {
    case 0x00: // LD
      cpu->r[x] = cpu->r[y];
      break;
    case 0x01: // OR
      cpu->r[x] |= cpu->r[y];
      break;
    case 0x02: // AND
      cpu->r[x] &= cpu->r[y];
      break;
    case 0x03: // XOR
      cpu->r[x] ^= cpu->r[y];
      break;
    case 0x04: // ADD
      u8 sum = cpu->r[x] + cpu->r[y];
      cpu->r[0xf] = (u8)(sum < cpu->r[x]);
      cpu->r[x] = sum;
      break;
    case 0x05: // SUB
      cpu->r[0xf] = (u8)(cpu->r[x] > cpu->r[y]);
      cpu->r[x] = cpu->r[x] - cpu->r[y];
      break;
    case 0x06: // SHR
      cpu->r[0xf] = (cpu->r[x] & 1) ? 1 : 0;
      cpu->r[x] >>= 1;
      break;
    case 0x07: // SUBN
      cpu->r[0xf] = (u8)(cpu->r[y] > cpu->r[x]);
      cpu->r[x] = cpu->r[y] - cpu->r[x];
      break;
    case 0x0e: // SHL
      cpu->r[0xf] = (cpu->r[x] & 0x80) ? 1 : 0;
      cpu->r[x] <<= 1;
      break;
    default:
      invalid();
    }
    break;
  case 0x09: // SNE
    if (cpu->r[x] != cpu->r[y]) {
      cpu->pc += 2;
    }
    break;
  case 0x0a: // LD
    cpu->i = nnn;
    break;
  case 0x0b: // JMP
    cpu->pc = nnn + cpu->r[0];
    break;
  case 0x0c: // RND
    cpu->r[x] = ((u8)rand()) & hi;
    break;
  case 0x0d: // DRW
    cpu->r[0xf] =
        fb_draw_sprite(console->fb, &ram[cpu->i], cpu->r[x], cpu->r[y], z);
    break;
  case 0x0e: // Keypad instrs
    if (hi == 0x9e) {
      if (keypad[cpu->r[x]]) {
        cpu->pc += 2;
      }
    } else if (hi == 0xa1) {
      if (keypad[cpu->r[x]] == 0) {
        cpu->pc += 2;
      }
    } else {
      invalid();
    }
    break;
  case 0x0f:
    switch (hi) {
    case 0x07: // LD
      cpu->r[x] = console->dt;
      break;
    case 0x0a: // LD
      unimplemented();
      break;
    case 0x15: // LD
      console->dt = cpu->r[x];
      break;
    case 0x18: // LD
      console->st = cpu->r[x];
      break;
    case 0x1E: // ADD
      cpu->i += cpu->r[x];
      break;
    case 0x29: // LD
      cpu->i = cpu->r[x] * 5;
      break;
    case 0x33: // LD
      ram[cpu->i] = cpu->r[x] / 100;
      ram[cpu->i + 1] = (cpu->r[x] % 100) / 10;
      ram[cpu->i + 2] = cpu->r[x] % 10;
      break;
    case 0x55: // LD [I], Vx
      memcpy(&ram[cpu->i], cpu->r, x + 1);
      break;
    case 0x65: // LD
      memcpy(cpu->r, &ram[cpu->i], x + 1);
      break;
    default:
      invalid();
    }
    break;
  default:
    invalid();
  }
}