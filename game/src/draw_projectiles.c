#include "game.h"

void (*proj_draw[48])();

void draw_projectiles(long where)
{
    long i;
    long image_no;
    register struct proj_def *type_def;

    for (i = 0; i < 50; ++i)
        if (projectiles[i].state == 1 && projectiles[i].shown) {
            type_def = &proj_defs[projectiles[i].type];
            if ((image_no = projectiles[i].anim[projectiles[i].frame]) == 0xff) {
                projectiles[i].frame = 0;
                image_no = *projectiles[i].anim;
            }
            port_call_sprite((long)proj_draw[image_no], projectiles[i].map_x * 4, projectiles[i].map_y * 4, plane_a_mem + where, line_table);
        }
}
