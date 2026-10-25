#include "game.h"

void start_continue(void)
{
    if (!continuing) {
        continuing = 1;
        zelda_anim_step = 0;
        zelda_thrusting = 1;
        thrust_lunge = 0;
        go_find_hero_action(6);
        anim_frame_idx = 0;
    }
}
