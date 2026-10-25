#include "game.h"

void init_hit(long x, long y)
{
    init_projectile(26, x + pf_x - 4, y + pf_y - 3, 0, 0, 0, 1, 0, 0, 3, 0);
}
