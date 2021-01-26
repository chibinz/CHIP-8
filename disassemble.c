#include <stdio.h>

#include "console.h"

void disassemble(u16 instr) {
  u8 lo, hi, v, x, y, z;
  lo = (u8)(instr >> 8);
  hi = (u8)(instr);
  v = lo >> 4;
  x = lo & 0x0f;
  y = hi >> 4;
  z = hi & 0x0f;

  // Print instruction
  printf("%02x %02x ", lo, hi);

  // Main switch
  // Opcodes are stored in big endian in chip-8
  switch (v) {
  case 0x00:
    if (hi == 0xe0)
      printf("CLS\n");
    else if (hi == 0xee)
      printf("RET\n");
    else
      printf("invalid operation\n");
    break;
  case 0x01:
    printf("JMP  %03x\n", (x << 8) + hi);
    break;
  case 0x02:
    printf("CALL %03x\n", (x << 8) + hi);
    break;
  case 0x03:
    printf("SE   V%x %02x\n", x, hi);
    break;
  case 0x04:
    printf("SNE  V%x %02x\n", x, hi);
    break;
  case 0x05:
    printf("SE   V%x V%x\n", x, y);
    break;
  case 0x06:
    printf("LD   V%x %02x\n", x, hi);
    break;
  case 0x07:
    printf("ADD  V%x %02x\n", x, hi);
    break;
  case 0x08:
    switch (z) {
    case 0x00:
      printf("LD   V%x V%x\n", x, y);
      break;
    case 0x01:
      printf("OR   V%x V%x\n", x, y);
      break;
    case 0x02:
      printf("AND  V%x V%x\n", x, y);
      break;
    case 0x03:
      printf("XOR  V%x V%x\n", x, y);
      break;
    case 0x04:
      printf("ADD  V%x V%x\n", x, y);
      break;
    case 0x05:
      printf("SUB  V%x V%x\n", x, y);
      break;
    case 0x06:
      printf("SHR  V%x\n", x);
      break;
    case 0x07:
      printf("SUBN V%x V%x\n", x, y);
      break;
    case 0x08:
      printf("SHL  V%x V%x\n", x, y);
      break;
    case 0x0e:
      printf("SHL  V%x V%x\n", x, y);
      break;
    default:
      printf("invalid operation\n");
    }
    break;
  case 0x09:
    printf("SNE  V%x V%x\n", x, y);
    break;
  case 0x0a:
    printf("LD   I %03x\n", (x << 8) + hi);
    break;
  case 0x0b:
    printf("JMP  %03x + V0\n", (x << 8) + hi);
    break;
  case 0x0c:
    printf("RND  V%x %02x\n", x, hi);
    break;
  case 0x0d:
    printf("DRW  V%x V%x %x\n", x, y, z);
    break;
  case 0x0e:
    if (hi == 0x9e)
      printf("SKP  V%x\n", x);
    else if (hi == 0xa1)
      printf("SKNP V%x\n", x);
    else
      printf("invalid operation\n");
    break;
  case 0x0f:
    switch (hi) {
    case 0x07:
      printf("LD   V%x DT\n", x);
      break;
    case 0x0a:
      printf("LD   V%x K\n", x);
      break;
    case 0x15:
      printf("LD   DT V%x\n", x);
      break;
    case 0x18:
      printf("LD   ST V%x\n", x);
      break;
    case 0x1E:
      printf("ADD  I V%x\n", x);
      break;
    case 0x29:
      printf("LD   F V%x\n", x);
      break;
    case 0x33:
      printf("LD   B V%x\n", x);
      break;
    case 0x55:
      printf("LD   [I] V%x\n", x);
      break;
    case 0x65:
      printf("LD   V%x [I]\n", x);
      break;
    default:
      printf("invalid operation\n");
    }
    break;
  default:
    printf("invalid operation\n");
  }
}
