#include "game.h"

void init_bomb_hit(long x, long y, long direct_hit)
{
    long left;
    long top;
    long right;
    long bottom;
    long i;

    bomb_placed = 0;
    init_projectile(26, x - 4, y - 3, 0, -48, 0, 1, 0, 0, 9, 0);
    init_projectile(26, x - 4, y - 3, -32, -32, 0, 1, 0, 0, 9, 0);
    init_projectile(26, x - 4, y - 3, 32, -32, 0, 1, 0, 0, 9, 0);
    init_projectile(26, x - 4, y - 3, -48, 0, 0, 1, 0, 0, 9, 0);
    init_projectile(26, x - 4, y - 3, 48, 0, 0, 1, 0, 0, 9, 0);
    init_projectile(26, x - 4, y - 3, 0, 48, 0, 1, 0, 0, 9, 0);
    init_projectile(26, x - 4, y - 3, -32, 32, 0, 1, 0, 0, 9, 0);
    init_projectile(26, x - 4, y - 3, 32, 32, 0, 1, 0, 0, 9, 0);
    left = x - 33;
    top = y - 22;
    right = left + 48;
    bottom = top + 30;
    for (i = 1; i < 21; ++i)
        if (sprite_ptrs[i] && check_vulnerablility(i, 25) && left <= sprite_draw_xs[i] && sprite_draw_xs[i] <= right
                && top <= sprite_draw_ys[i] && bottom >= sprite_draw_ys[i])
            (&damage_taken)[i] += 10;
}
