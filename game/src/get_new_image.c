#include "game.h"

void get_new_image(long sprite_idx)
{
    if (macros[sprite_idx]->animate)
        ++(&anim_frame_idx)[sprite_idx];
    macros[sprite_idx]->animate = 1;
    if (sprite_actions[sprite_idx][(&hero_action)[sprite_idx]].frames[(&anim_frame_idx)[sprite_idx]] == 999)
        (&anim_frame_idx)[sprite_idx] = 0;
    (&cur_sprite)[sprite_idx] = sprite_frames[sprite_idx * 24 + sprite_actions[sprite_idx][(&hero_action)[sprite_idx]].frames[(&anim_frame_idx)[sprite_idx]]];
    sprite_frame_no[sprite_idx] = sprite_actions[sprite_idx][(&hero_action)[sprite_idx]].frames[(&anim_frame_idx)[sprite_idx]];
}
