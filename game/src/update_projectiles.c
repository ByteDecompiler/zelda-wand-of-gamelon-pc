#include "game.h"

void update_projectiles(void)
{
    long i;
    long k;
    struct proj_def *type_def;

    for (i = 0; i < 50; ++i) {
        if (projectiles[i].state == 5) {
            if ((unsigned char)projectiles[i].repeats > 1) {
                --projectiles[i].repeats;
                projectiles[i].state = 0;
            } else if (!projectiles[i].repeats)
                projectiles[i].state = 0;
            else
                projectiles[i].state = 4;
        }
        if (projectiles[i].state == 4)
            continue;
        type_def = &proj_defs[projectiles[i].type];
        if ((unsigned char)projectiles[i].wait > 0)
            --projectiles[i].wait;
        if (!projectiles[i].state) {
            if (projectiles[i].wait)
                continue;
            projectiles[i].wait = projectiles[i].interval;
            projectiles[i].state = 1;
            projectiles[i].map_x = projectiles[i].x = projectiles[i].start_x - pf_x;
            projectiles[i].map_y = projectiles[i].y = projectiles[i].start_y - pf_y;
            projectiles[i].fx = projectiles[i].start_x * 8;
            projectiles[i].fy = projectiles[i].start_y * 8;
            projectiles[i].vx = projectiles[i].start_vx;
            projectiles[i].vy = projectiles[i].start_vy;
            if (projectiles[i].vx >= 0)
                projectiles[i].anim = (unsigned char *)type_def->anim_right;
            else
                projectiles[i].anim = (unsigned char *)type_def->anim_left;
            for (k = 0; projectiles[i].anim[k + 1] != 0xff; ++k)
                ;
            projectiles[i].frame = k;
            projectiles[i].visible = 0;
            projectiles[i].shown = 0;
            projectiles[i].life = projectiles[i].lifetime;
        }
        if (projectiles[i].state == 6)
            projectiles[i].state = 5;
        if (projectiles[i].state == 1 && (unsigned char)projectiles[i].lifetime > 0 && !projectiles[i].life--)
            projectiles[i].state = 5;
        projectiles[i].x = projectiles[i].map_x;
        projectiles[i].y = projectiles[i].map_y;
        projectiles[i].fx += projectiles[i].vx;
        projectiles[i].fy += projectiles[i].vy;
        projectiles[i].vy += projectiles[i].gravity;
        if (projectiles[i].vy > 24 && (projectiles[i].type == 25 || projectiles[i].type == 6))
            projectiles[i].vy = 24;
        projectiles[i].map_x = projectiles[i].fx / 8 - pf_x;
        projectiles[i].map_y = projectiles[i].fy / 8 - pf_y;
        projectiles[i].visible = projectiles[i].shown;
        if ((long)projectiles[i].map_x < -8 || projectiles[i].map_x > 95 || projectiles[i].map_y < 0
                || projectiles[i].map_y > 51) {
            if (!type_def->offscreen_ok)
                projectiles[i].state = 5;
            projectiles[i].shown = 0;
        } else
            projectiles[i].shown = 1;
        if (!projectiles[i].shown) {
            if (projectiles[i].fx < proj_min_x || projectiles[i].fx > proj_max_x || projectiles[i].fy < proj_min_y
                    || projectiles[i].fy > proj_max_y) {
                projectiles[i].state = 5;
                if (projectiles[i].type == 25)
                    bomb_placed = 0;
            }
        } else
            ++projectiles[i].frame;
    }
}
