#include "game.h"

void end_thrust(void)
{
    anim_frame_idx = 0;
    zelda_thrusting = 0;
    fall_timer = 0;
    get_sprite_block(zelda_x, zelda_y, &jump_info);
    if (zelda_crouching)
        go_find_hero_action(9);
    else if (jump_rising)
        go_find_hero_action(4);
    else if (jump_falling || jump_info_ground || jump_info.left || jump_info.right) {
        go_find_hero_action(4);
        anim_frame_idx = 1;
    } else
        go_find_hero_action(0);
    cur_sprite = sprite_frames[hero_actions[hero_action].frames[anim_frame_idx]];
}
