#include <string.h>

#include "types.h"

void fb_clear(u8 *fb) { memset(fb, 0, 32 * 64); }

u8 fb_draw_sprite(u8 *fb, u8 *sprite, u8 vx, u8 vy, u8 n) {
  u8 overflow = 0;
  u8 pixel = 0;

  for (usize y = 0; y < n; y++) {
    for (usize x = 0; x < 8; x++) {
      pixel = sprite[y] & (1 << (7 - x)) ? 0xff : 0;
      overflow |= pixel & fb[(y + vy) * 64 + vx + x];
      fb[(y + vy) * 64 + vx + x] ^= pixel;
    }
  }

  return overflow == 0xff;
}