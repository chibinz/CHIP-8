#include <stdio.h>
#include <stdlib.h>

#include "console.h"

int main(int argc, char **argv) {
  // Check number of arguments
  if (argc != 2) {
    printf("Usage: chip <rom>\n");
    exit(-1);
  }

  console chip = console_new();
  console_load_rom(&chip, argv[1]);

  return 0;
}
