#include <stdio.h>
#include <stdlib.h>

#include "MiniFB.h"
#include "console.h"

int main(int argc, char **argv) {
  // Check number of arguments
  if (argc != 2) {
    printf("Usage: chip <rom>\n");
    exit(-1);
  }

  console chip = console_new();
  usize len = console_load_rom(&chip, argv[1]);
  disassemble_rom(chip.ram, 0x200, len);

  // struct mfb_window *window = mfb_open("CHIP-8", 64, 32);
  // mfb_update(window, chip.fb); // fb is u8 *, not u32 *
  // mfb_close(window);

  return 0;
}
