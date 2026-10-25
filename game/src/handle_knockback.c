#include "game.h"

void handle_knockback(void)
{
    get_sprite_block(zelda_x, zelda_y, &jump_info);
    if (jump_info.left || jump_info.right) {
        knockback[0] = 0;
        return;
    }
    if (knockback[0] > 0) {
        if (!zelda_thrusting && !rope_out && !jump_rising && !jump_falling && !climbing_blk48 && !climbing_blk25) {
            if (hero_actions[hero_action].id != 16) {
                go_find_hero_action(16);
                anim_frame_idx = 0;
                cur_sprite = sprite_frames[hero_actions[hero_action].frames[anim_frame_idx]];
            }
            move_1_block(0, 1, 0);
            knocked = 1;
        }
        --knockback[0];
    } else if (knockback[0] < 0) {
        if (!zelda_thrusting && !rope_out && !jump_rising && !jump_falling && !climbing_blk48 && !climbing_blk25) {
            if (hero_actions[hero_action].id != 16) {
                go_find_hero_action(16);
                anim_frame_idx = 0;
                cur_sprite = sprite_frames[hero_actions[hero_action].frames[anim_frame_idx]];
            }
            move_1_block(0, -1, 0);
            knocked = 1;
        }
        ++knockback[0];
    } else if (knocked) {
        knocked = 0;
        if (!zelda_thrusting && !rope_out && !jump_rising && !jump_falling && !climbing_blk48 && !climbing_blk25) {
            go_find_hero_action(0);
            anim_frame_idx = 0;
            cur_sprite = sprite_frames[hero_actions[hero_action].frames[anim_frame_idx]];
        }
    }
}
