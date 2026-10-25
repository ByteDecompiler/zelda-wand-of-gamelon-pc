#include "game.h"

void clear_projectiles(void)
{
    long i;

    for (i = 0; i < 50; ++i)
        if (projectiles[i].state == 1)
            projectiles[i].state = 6;
}
