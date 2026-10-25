#include "game.h"

void using_rope(void)
{
    long step;

    switch ((unsigned char)rope_out) {
    case 0:
        break;
    case 1:
        rope_top -= 3;
        if (pf_y + 1 >= rope_top) {
            rope_top = pf_y + 1;
            rope_out = 3;
        }
        if (check_for_rope_path(rope_x, rope_top + 2))
            rope_top += 2;
        else if (check_for_rope_path(rope_x, rope_top + 1))
            rope_top += 1;
        else
            check_for_rope_path(rope_x, rope_top);
        break;
    case 2:
        --rope_count;
        go_find_hero_action(11);
        anim_frame_idx = 0;
        rope_out = 4;
        zelda_moved = 1;
        break;
    case 3:
        if (rope_top >= rope_bottom) {
            rope_out = 6;
            zelda_dir = old_zelda_dir;
            go_find_hero_action(0);
            anim_frame_idx = 0;
            cur_sprite = sprite_frames[hero_actions[hero_action].frames[anim_frame_idx]];
        } else {
            rope_top += 3;
            if (rope_top >= rope_bottom) {
                rope_top = rope_bottom;
                rope_out = 6;
                zelda_dir = old_zelda_dir;
                go_find_hero_action(0);
                anim_frame_idx = 0;
                cur_sprite = sprite_frames[hero_actions[hero_action].frames[anim_frame_idx]];
            }
        }
        break;
    case 4:
        if (zelda_y - 11 <= rope_top) {
            zelda_y -= 3;
            zelda_screen_y -= 3;
            zelda_sprite_y -= 3;
            rope_bottom -= 3;
            zelda_moved = 1;
            rope_out = 5;
            go_find_hero_action(14);
            anim_frame_idx = 0;
        } else {
            zelda_y -= 1;
            zelda_screen_y -= 1;
            zelda_sprite_y -= 1;
            rope_bottom -= 1;
            if (rope_anim_toggle)
                zelda_anim_step = 1;
            else
                zelda_anim_step = 0;
            rope_anim_toggle = 1 - rope_anim_toggle;
        }
        zelda_moved = 1;
        break;
    case 5:
        if (zelda_y <= rope_top) {
            rope_out = 6;
            zelda_dir = old_zelda_dir;
            go_find_hero_action(0);
            anim_frame_idx = 0;
        } else if (zelda_y - rope_top < 3) {
            step = zelda_y - rope_top;
            zelda_y -= step;
            zelda_screen_y -= step;
            zelda_sprite_y -= step;
            rope_bottom -= step;
        } else {
            zelda_y -= 3;
            zelda_screen_y -= 3;
            zelda_sprite_y -= 3;
            rope_bottom -= 3;
        }
        zelda_moved = 1;
        break;
    case 6:
        rope_out = 0;
        break;
    }
}
