#include "game.h"

void lob_bomb(void)
{
    long x;
    long y;
    long land_x;
    long throw_vx;
    long speed;
    long dy;

    dy = 0;
    if (bomb_placed)
        return;
    if (zelda_crouching)
        dy = 2;
    else if (jump_rising)
        dy = -4;
    y = zelda_sprite_y + 8 + pf_y + dy;
    throw_vx = 16;
    speed = 14;
    if (zelda_dir == 2) {
        x = zelda_sprite_x + 4 + pf_x;
        if (thrust_lunge)
            x += 4;
        land_x = check_map_row(x, y, 6);
        init_projectile(25, land_x, y - 2, throw_vx, -speed, 2, 1, 0, 0, 0, 1);
    } else {
        x = zelda_sprite_x + 14 + pf_x;
        if (thrust_lunge)
            x -= 4;
        land_x = check_map_row(x, y, -6);
        init_projectile(25, land_x + -2, y - 2, -throw_vx, -speed, 2, 1, 0, 0, 0, 1);
    }
    bomb_placed = 1;
    bomb_bounced = 0;
}
