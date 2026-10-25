#include "game.h"

void check_which_upward_motion(struct motion *move_info)
{
    check_for_climb(-1);
    if (climbing_blk48 || climbing_blk25) {
        if (climbing_blk48)
            go_find_hero_action(12);
        if (climbing_blk25)
            go_find_hero_action(11);
        move_info->next = 0;
        old_zelda_dir = zelda_dir;
        zelda_dir = 2;
        zelda_sprite_x = zelda_screen_x - 8;
        anim_frame_idx = 0;
        --zelda_y;
        --zelda_screen_y;
        --zelda_sprite_y;
        zelda_moved = 1;
    } else {
        get_sprite_block(zelda_x, zelda_y - 1, &jump_info);
        if (!jump_info_solid) {
            if ((unsigned char)jump_wait_timer < 10)
                jump_info_solid = 1;
            else {
                jump_wait_timer = 10;
                go_find_hero_action(4);
                anim_frame_idx = 0;
                zelda_moved = 1;
                zelda_anim_step = 0;
                jump_rising = 1;
                jump_step = 0;
                move_info->next = move_info->dir;
                move_info->prev = move_info->dir;
            }
        }
    }
}
