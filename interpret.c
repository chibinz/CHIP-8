#include "console.h"

void console_step(console *console) {
  u8 *ram = console->ram;
  u8 *keypad = console->keypad;
  u8 *stack = &console->cpu.stack;
  cpu *cpu = &console->cpu;

  u8 lo, hi, v, x, y, z, max;
  lo = ram[cpu->pc];
  hi = ram[cpu->pc + 1];
  v = lo >> 4;
  x = lo & 0x0f;
  y = hi >> 4;
  z = hi & 0x0f;
  max = cpu->r[x] > cpu->r[y] ? cpu->r[x] : cpu->r[y];

  // Main switch
  // The opcodes are stored in big endian in chip-8
  switch (v) {
  case 0x00:
    if (hi == 0xe0) { // CLS
      // clearScreen();
    } else if (hi == 0xee) { // RET
      cpu->pc = stack[cpu->sp];
      cpu->sp--;
      return 0;
    } else
      printf("invalid operation\n");
    break;
  case 0x01: // JMP
    cpu->pc = (x << 8) + hi;
    return 1;
  case 0x02: // CALL
    cpu->sp++;
    stack[cpu->sp] = cpu->pc + 2;
    cpu->pc = (x << 8) + hi;
    return 2;
  case 0x03:
    // SE
    if (cpu->r[x] == hi) {
      cpu->pc += 4;
      return 3;
    }
    break;
  case 0x04:
    // SNE
    if (cpu->r[x] != hi) {
      cpu->pc += 4;
      return 4;
    }
    break;
  case 0x05:
    // SE
    if (cpu->r[x] == cpu->r[y]) {
      cpu->pc += 4;
      return 5;
    }
    break;
  case 0x06:
    // LD
    cpu->r[x] = hi;
    break;
  case 0x07:
    // ADD
    cpu->r[x] += hi;
    break;
  case 0x08:
    switch (z) {
    case 0x00:
      // LD
      cpu->r[x] = cpu->r[y];
      break;
    case 0x01:
      // OR
      cpu->r[x] |= cpu->r[y];
      break;
    case 0x02:
      // AND
      cpu->r[x] &= cpu->r[y];
      break;
    case 0x03:
      // XOR
      cpu->r[x] ^= cpu->r[y];
      break;
    case 0x04:
      // ADD
      cpu->f = (cpu->r[x] + cpu->r[y]) > max ? 0 : 1;
      cpu->r[x] += cpu->r[y];
      break;
    case 0x05:
      // SUB
      cpu->f = (cpu->r[x] == max) ? 1 : 0;
      cpu->r[x] -= cpu->r[y];
      break;
    case 0x06:
      // SHR
      cpu->f = (cpu->r[x] & 1) ? 1 : 0;
      cpu->r[x] >>= 1;
      break;
    case 0x07:
      // SUBN
      cpu->f = (cpu->r[y] == max) ? 1 : 0;
      cpu->r[x] = cpu->r[y] - cpu->r[x];
      break;
    case 0x08:
      // SHL
      cpu->f = (cpu->r[x] & 0x80) ? 1 : 0;
      cpu->r[x] <<= 1;
      break;
    case 0x0e:
      // SHL
      cpu->f = (cpu->r[x] & 0x80) ? 1 : 0;
      cpu->r[x] <<= 1;
      break;
    default:
      printf("invalid operation\n");
    }
    break;
  case 0x09:
    // SNE
    if (cpu->r[x] == cpu->r[y]) {
      cpu->pc += 4;
      return 9;
    }
    break;
  case 0x0a:
    // LD
    cpu->i = (x << 8) + hi;
    break;
  case 0x0b:
    // JMP
    cpu->pc = (x << 8) + hi + cpu->r[0];
    break;
  case 0x0c:
    // RND
    cpu->r[x] = ((uint8_t)rand()) & hi;
    break;
  case 0x0d:
    // DRW
    // cpu->f = drawcpu->sprite(cpu->r[x], cpu->r[y], z);
    break;
  case 0x0e:
    if (hi == 0x9e) {
      printf("SKP  V%x, %x\n", x, cpu->r[x]);
      if (keypad[cpu->r[x]]) {
        cpu->pc += 4;
        return 0;
      }
      break;
    } else if (hi == 0xa1) {
      printf("SKNP V%x, %x\n", x, cpu->r[x]);
      if (keypad[cpu->r[x]] == 0) {
        cpu->pc += 4;
        return 0;
      }
      break;
    } else
      printf("invalid operation\n");
    break;
  case 0x0f:
    switch (hi) {
    case 0x07:
      // LD
      cpu->r[x] = cpu->dt;
      break;
    case 0x0a:
      // LD
      while (1) {
        for (int i = 0; i < 0x10; i++)
          if (keypad[i] == 1) {
            cpu->r[x] = i;
            cpu->pc += 2;
            return 0;
          }
      }
      break;
    case 0x15:
      // LD
      cpu->dt = cpu->r[x];
      break;
    case 0x18:
      // LD
      cpu->st = cpu->r[x];
      break;
    case 0x1E:
      // ADD
      cpu->i += cpu->r[x];
      break;
    case 0x29:
      // LD
      cpu->i = cpu->r[x] * 5;
      break;
    case 0x33:
      // LD
      ram[cpu->i] = cpu->r[x] / 100;
      ram[cpu->i + 1] = (cpu->r[x] % 100) / 10;
      ram[cpu->i + 2] = cpu->r[x] % 10;
      break;
    case 0x55:
      // LD
      for (int j = 0; j <= x; j++)
        ram[cpu->i + j] = cpu->r[x + j];
      break;
    case 0x65:
      // LD
      for (int j = 0; j <= x; j++)
        cpu->r[x + j] = ram[cpu->i + j];
      break;
    default:
      printf("invalid operation\n");
    }
    break;
  default:
    printf("invalid operation\n");
  }

  cpu->pc += 2;

  return 0;
}