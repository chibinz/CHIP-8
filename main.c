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

  struct mfb_window *window = mfb_open("CHIP-8", 64, 32);

  console chip = console_new();
  console_load_rom(&chip, argv[1]);

  mfb_update(window, chip.fb);
  mfb_close(window);

  return 0;
}
