#include "game.h"

void erase_projectiles(long where)
{
    long i;

    for (i = 0; i < 50; ++i)
        if (projectiles[i].state != 4 && projectiles[i].visible)
            erase_32x32(projectiles[i].x, projectiles[i].y, plane_a_mem + where);
}
