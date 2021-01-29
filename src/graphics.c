#include <string.h>

#include "types.h"

void fb_clear(u8 *fb) { memset(fb, 0, 32 * 64); }

u8 fb_draw_sprite(u8 *fb, u8 *sprite, u8 vx, u8 vy, u8 n) {
  u8 overflow = 0, pixel, wy, wx;

  for (usize y = 0; y < n; y++) {
    for (usize x = 0; x < 8; x++) {
      pixel = sprite[y] & (1 << (7 - x)) ? 0xff : 0;
      wy = (vy + y) % 32;
      wx = (vx + x) % 64;
      overflow |= pixel & fb[wy * 64 + wx];
      fb[wy * 64 + wx] ^= pixel;
    }
  }

  return overflow == 0xff;
}