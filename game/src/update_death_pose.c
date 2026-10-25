#include "game.h"

void update_death_pose(void)
{
    if (zelda_anim_step)
        anim_frame_idx += 1;
    zelda_anim_step = 1;
    if (zelda_thrusting && hero_actions[hero_action].frames[anim_frame_idx] == 999) {
        anim_frame_idx = 0;
        death_done = 1;
    }
    cur_sprite = sprite_frames[hero_actions[hero_action].frames[anim_frame_idx]];
}
