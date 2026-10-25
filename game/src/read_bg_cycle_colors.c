#include "game.h"

void read_bg_cycle_colors(void)
{
    long i;

    for (i = 0; i < 16; ++i)
        bg_cycle[i] = ((i + 48 & 0x3f) << 24) + 0x80000000
            | (palette_out[(i + 48) * 3 + 0xc0] & 0xff) << 16
            | (palette_out[(i + 48) * 3 + 0xc1] & 0xff) << 8
            | palette_out[(i + 48) * 3 + 0xc2] & 0xff;
}
