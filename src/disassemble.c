#include <assert.h>
#include <stdio.h>

#include "console.h"

#define dis(mnemonic, fmt, ...) printf(mnemonic "\t" fmt "\n", __VA_ARGS__)

void disassemble_rom(u8 *rom, usize start, usize len) {
  assert(start % 2 == 0);

  for (usize i = 0; i < (len / 2) * 2; i += 2) {
    printf("%03lx: ", i);
    disassemble(rom[start + i], rom[start + i + 1]);
  }
}

void disassemble(u8 lo, u8 hi) {
  u8 v, x, y, z;
  v = lo >> 4;
  x = lo & 0x0f;
  y = hi >> 4;
  z = hi & 0x0f;

  printf("%02x%02x ", lo, hi);

  switch (v) {
  case 0x00:
    if (hi == 0xe0)
      printf("CLS\n");
    else if (hi == 0xee)
      printf("RET\n");
    else
      invalid();
    break;
  case 0x01:
    dis("JMP", "%03x", (x << 8) + hi);
    break;
  case 0x02:
    dis("CALL", "%03x", (x << 8) + hi);
    break;
  case 0x03:
    dis("SE", "V%x %02x", x, hi);
    break;
  case 0x04:
    dis("SNE", "V%x %02x", x, hi);
    break;
  case 0x05:
    dis("SE", "V%x V%x", x, y);
    break;
  case 0x06:
    dis("LD", "V%x %02x", x, hi);
    break;
  case 0x07:
    dis("ADD", "V%x %02x", x, hi);
    break;
  case 0x08:
    switch (z) {
    case 0x00:
      dis("LD", "V%x V%x", x, y);
      break;
    case 0x01:
      dis("OR", "V%x V%x", x, y);
      break;
    case 0x02:
      dis("AND", "V%x V%x", x, y);
      break;
    case 0x03:
      dis("XOR", "V%x V%x", x, y);
      break;
    case 0x04:
      dis("ADD", "V%x V%x", x, y);
      break;
    case 0x05:
      dis("SUB", "V%x V%x", x, y);
      break;
    case 0x06:
      dis("SHR", "V%x", x);
      break;
    case 0x07:
      dis("SUBN", "V%x V%x", x, y);
      break;
    case 0x08:
      dis("SHL", "V%x V%x", x, y);
      break;
    case 0x0e:
      dis("SHL", "V%x V%x", x, y);
      break;
    default:
      invalid();
    }
    break;
  case 0x09:
    dis("SNE", "V%x V%x", x, y);
    break;
  case 0x0a:
    dis("LD", "I %03x", (x << 8) + hi);
    break;
  case 0x0b:
    dis("JMP", "%03x + V0", (x << 8) + hi);
    break;
  case 0x0c:
    dis("RND", "V%x %02x", x, hi);
    break;
  case 0x0d:
    dis("DRW", "V%x V%x %x", x, y, z);
    break;
  case 0x0e:
    if (hi == 0x9e)
      dis("SKP", "V%x", x);
    else if (hi == 0xa1)
      dis("SKNP", "V%x", x);
    else
      invalid();
    break;
  case 0x0f:
    switch (hi) {
    case 0x07:
      dis("LD", "V%x DT", x);
      break;
    case 0x0a:
      dis("LD", "V%x K", x);
      break;
    case 0x15:
      dis("LD", "DT V%x", x);
      break;
    case 0x18:
      dis("LD", "ST V%x", x);
      break;
    case 0x1E:
      dis("ADD", "I V%x", x);
      break;
    case 0x29:
      dis("LD", "F V%x", x);
      break;
    case 0x33:
      dis("LD", "B V%x", x);
      break;
    case 0x55:
      dis("LD", "[I] V%x", x);
      break;
    case 0x65:
      dis("LD", "V%x [I]", x);
      break;
    default:
      invalid();
    }
    break;
  default:
    invalid();
  }
}
