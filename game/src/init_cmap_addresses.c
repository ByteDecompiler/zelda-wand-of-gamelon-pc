#include "game.h"

void init_cmap_addresses(void)
{
    long i;
    long j;

    cmaps[0] = zelda_cmap;
    for (i = 1, j = 0; i < 4; ++i, ++j)
        cmaps[i] = enemy_cmaps + j * 0x570;
    for (j = 0; i < 6; ++i, ++j)
        cmaps[i] = proj_cmap_base + j * 0x570;
}
