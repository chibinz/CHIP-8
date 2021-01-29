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

static inline void set_keys(struct mfb_window *window, u8 *keypad) {
  u8 *kb = mfb_get_key_buffer(window);

  keypad[0x0] = kb[KB_KEY_X];
  keypad[0x1] = kb[KB_KEY_1];
  keypad[0x2] = kb[KB_KEY_2];
  keypad[0x3] = kb[KB_KEY_3];
  keypad[0x4] = kb[KB_KEY_Q];
  keypad[0x5] = kb[KB_KEY_W];
  keypad[0x6] = kb[KB_KEY_E];
  keypad[0x7] = kb[KB_KEY_A];
  keypad[0x8] = kb[KB_KEY_S];
  keypad[0x9] = kb[KB_KEY_D];
  keypad[0xa] = kb[KB_KEY_Z];
  keypad[0xb] = kb[KB_KEY_C];
  keypad[0xc] = kb[KB_KEY_4];
  keypad[0xd] = kb[KB_KEY_R];
  keypad[0xe] = kb[KB_KEY_F];
  keypad[0xf] = kb[KB_KEY_V];
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
    set_keys(window, chip.keypad);
    scale_u32(chip.fb, scaled, scale);
    console_tick(&chip);
  }

  mfb_close(window);
  free(scaled);

  return 0;
}
