#include "game.h"

long projectile_on_field(long type)
{
    long i;

    for (i = 0; i < 10; ++i)
        if (projectiles[i].type == type && projectiles[i].state == 1)
            return 1;
    return 0;
}
