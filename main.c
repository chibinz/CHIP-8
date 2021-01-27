#include <stdio.h>
#include <stdlib.h>

#include "MiniFB.h"
#include "console.h"

static inline u32 *scale_u32(u8 *orig, usize scale) {
  usize width = 64 * scale;
  usize height = 32 * scale;
  u32 *scaled = (u8 *)malloc(width * height * sizeof(u32));

  for (usize y = 0; y < height; y++) {
    for (usize x = 0; x < width; x++) {
      scaled[y * width + x] = (u32)orig[(y / scale) * 64 + x / scale];
    }
  }

  return scaled;
}

int main(int argc, char **argv) {
  // Check number of arguments
  if (argc != 2) {
    printf("Usage: chip <rom>\n");
    exit(-1);
  }

  console chip = console_new();
  usize len = console_load_rom(&chip, argv[1]);
  disassemble_rom(chip.ram, 0x200, len);

  usize scale = 4;
  u32 *scaled = scale_u32(chip.fb, scale); // fb is u8 *, not u32 *

  struct mfb_window *window = mfb_open("CHIP-8", 64 * scale, 32 * scale);
  mfb_update(window, scaled);
  mfb_close(window);

  free(scaled);

  return 0;
}
