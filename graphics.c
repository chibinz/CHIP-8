#include <string.h>

#include "types.h"

void fb_clear(u8 *fb) { memset(fb, 0, 32 * 64); }

void fb_draw_sprite(u8 *fb, u8 *sprite) {}