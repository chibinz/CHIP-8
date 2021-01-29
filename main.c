#include <stdio.h>
#include <stdlib.h>

#include "MiniFB.h"
#include "console.h"

static inline void scale_u32(u8 *orig, u32 *scaled, usize scale) {
  usize width = 64 * scale;
  usize height = 32 * scale;

  for (usize y = 0; y < height; y++) {
    for (usize x = 0; x < width; x++) {
      scaled[y * width + x] = (u32)orig[(y / scale) * 64 + x / scale];
    }
  }
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
  u32 *scaled = (u32 *)malloc(64 * 32 * scale * scale * sizeof(u32));

  struct mfb_window *window = mfb_open("CHIP-8", 64 * scale, 32 * scale);

  while (mfb_update(window, scaled) >= 0) {
    console_tick(&chip);
    scale_u32(chip.fb, scaled, scale);
  }

  mfb_close(window);
  free(scaled);

  return 0;
}
