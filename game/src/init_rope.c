#include "game.h"

void init_rope(void)
{
    get_sprite_block(zelda_x, zelda_y, &jump_info);
    if ((jump_info.floor || jump_info_solid || jump_info.track || jump_info.climb_b || jump_info.climb_a)
            && !jump_info.deadly && !jump_info.right && !jump_info.left) {
        if (jump_info.climb_b && old_get_sprite_block(zelda_x, zelda_y - 1) == 25)
            return;
        if (jump_info.climb_a && old_get_sprite_block(zelda_x, zelda_y - 1) == 48)
            return;
        rope_out = 1;
        rope_x = zelda_x;
        rope_top = rope_bottom = zelda_y - 10;
        old_zelda_dir = zelda_dir;
        zelda_dir = 2;
        zelda_sprite_x = zelda_screen_x - 8;
        zelda_thrusting = 0;
        go_find_hero_action(15);
        anim_frame_idx = 0;
        cur_sprite = sprite_frames[hero_actions[hero_action].frames[anim_frame_idx]];
        zelda_moved = 1;
        zelda_anim_step = 0;
    }
}
