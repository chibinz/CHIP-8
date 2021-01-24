#include <stdio.h>
#include <stdlib.h>

#include "console.h"

int main(int argc, char **argv) {
  // check number of arguments
  if (argc != 2) {
    printf("Use: interpreter <rom>\n");
    exit(-1);
  }

  return 0;
}
