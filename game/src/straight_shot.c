#include "game.h"

void straight_shot(long type, long shot_damage)
{
    long dy;
    long y;
    long x;

    dy = 0;
    dy = -1;
    if (zelda_crouching)
        dy = 0;
    else if (jump_rising)
        dy = -4;
    y = zelda_sprite_y + 8 + pf_y + dy;
    if (zelda_dir == 2) {
        x = check_map_row(zelda_sprite_x + 4 + pf_x, y, 11);
        if (x < zelda_sprite_x + 14 + pf_x)
            init_miss(x - pf_x, y - pf_y);
        else
            init_projectile(type, x - 3, y - 2, 32, 0, 0, 1, 0, 0, 0, shot_damage);
    } else {
        x = check_map_row(zelda_sprite_x + 13 + pf_x, y, -11);
        if (x > zelda_sprite_x + 3 + pf_x)
            init_miss(x - pf_x, y - pf_y);
        else
            init_projectile(type, x - 3, y - 2, -32, 0, 0, 1, 0, 0, 0, shot_damage);
    }
}
