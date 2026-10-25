#include "game.h"

void restore_sprite_blocks(long x, long y, long count, char *src)
{
    register unsigned char *dst;

    dst = block_map + (x + y * map_width);
    while (count--)
        *dst++ = *src++;
}
