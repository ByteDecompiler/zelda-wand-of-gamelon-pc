#include "game.h"

void check_projectiles(void)
{
    long i;
    long j;
    long frame_code;
    register struct proj_def *pdef;

    for (i = 0; i < 50; ++i)
        if (projectiles[i].state == 1) {
            pdef = &proj_defs[projectiles[i].type];
            if ((frame_code = projectiles[i].anim[projectiles[i].frame]) == 0xff) {
                projectiles[i].frame = 0;
                frame_code = *projectiles[i].anim;
            }
            if (projectiles[i].state == 1 && projectiles[i].shown) {
                if (!pdef->owner) {
                    for (j = 0; j < 21; ++j)
                        if (projectiles[i].state == 1 && sprite_ptrs[j] && j)
                            check_collisions(j, sprite_gfx[j], i, frame_code);
                } else if (sprite_ptrs[0] && pdef->owner != 3)
                    check_collisions(0, 0, i, frame_code);
            }
            if (projectiles[i].state == 1 && (pdef->stops_at_walls || pdef->bounces))
                check_map(i, frame_code);
        }
}
