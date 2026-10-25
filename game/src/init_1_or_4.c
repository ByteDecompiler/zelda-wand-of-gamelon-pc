#include "game.h"

short walk_frames_right[5];
short walk_frames_left[5];

void init_1_or_4(long sprite_idx)
{
    sprite_actions[sprite_idx][0].id = 2;
    sprite_actions[sprite_idx][0].frames = walk_frames_left;
    sprite_actions[sprite_idx][1].id = 3;
    sprite_actions[sprite_idx][1].frames = walk_frames_right;
    if (macros[sprite_idx]->state == 2) {
        sprite_draw_xs[sprite_idx] = sprite_pos[sprite_idx][2] - 8;
        (&hero_action)[sprite_idx] = 1;
    } else {
        sprite_draw_xs[sprite_idx] = sprite_pos[sprite_idx][2] - 9;
        (&hero_action)[sprite_idx] = 0;
    }
    (&cur_sprite)[sprite_idx] = sprite_frames[sprite_idx * 24 + sprite_actions[sprite_idx][(&hero_action)[sprite_idx]].frames[(&anim_frame_idx)[sprite_idx]]];
    sprite_frame_no[sprite_idx] = sprite_actions[sprite_idx][(&hero_action)[sprite_idx]].frames[(&anim_frame_idx)[sprite_idx]];
    sprite_draw_ys[sprite_idx] = sprite_pos[sprite_idx][3] - 14;
}
