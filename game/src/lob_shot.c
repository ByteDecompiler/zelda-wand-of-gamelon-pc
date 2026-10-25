#include "game.h"

void lob_shot(long type, long shot_damage)
{
    long dy;
    long y;
    long wall_x;
    long x;

    dy = 0;
    if (zelda_crouching)
        dy = 2;
    else if (jump_rising)
        dy = -4;
    y = zelda_sprite_y + 8 + pf_y + dy;
    if (zelda_dir == 2) {
        x = zelda_sprite_x + 4 + pf_x;
        if (thrust_lunge)
            x += 4;
        wall_x = check_map_row(x, y, 11);
        if (wall_x < x + 10)
            init_miss(wall_x - pf_x, y - pf_y);
        else
            init_projectile(type, wall_x - 3, y - 2, 20, -10, 2, 1, 0, 0, 0, shot_damage);
    } else {
        x = zelda_sprite_x + 13 + pf_x;
        if (thrust_lunge)
            x -= 4;
        wall_x = check_map_row(x, y, -11);
        if (wall_x > x - 10)
            init_miss(wall_x - pf_x, y - pf_y);
        else
            init_projectile(type, wall_x - 3, y - 2, -20, -10, 2, 1, 0, 0, 0, shot_damage);
    }
}
