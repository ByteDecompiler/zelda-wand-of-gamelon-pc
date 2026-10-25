#include "game.h"

void reflect_projectile(long proj_no)
{
    proj_defs[1].anim_left = proj_defs[projectiles[proj_no].type].anim_left;
    proj_defs[1].anim_right = proj_defs[projectiles[proj_no].type].anim_right;
    init_projectile(1, projectiles[proj_no].map_x + pf_x, projectiles[proj_no].map_y + pf_y, -projectiles[proj_no].vx,
                    -projectiles[proj_no].vy, projectiles[proj_no].gravity, 1, 0, 0, 0, projectiles[proj_no].damage);
}
