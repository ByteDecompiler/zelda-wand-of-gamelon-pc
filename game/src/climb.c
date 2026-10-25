#include "game.h"

void climb(unsigned char up_action, unsigned char climb_action, long ladder_flag, struct climb_dir *dir_state)
{
    long i;

    if (dir_state->next)
        dir_state->dir = dir_state->next;
    if (dir_state->dir == 8 || dir_state->dir == 1 || dir_state->dir == 7) {
        get_sprite_block(zelda_x, zelda_y - 1, &jump_info);
        if (jump_info.track) {
            --zelda_y;
            --zelda_screen_y;
            --zelda_sprite_y;
        } else if (*(long *)ladder_flag) {
            get_sprite_block(zelda_x, zelda_y - 11, &jump_info);
            if (*(long *)ladder_flag) {
                --zelda_y;
                --zelda_screen_y;
                --zelda_sprite_y;
                zelda_moved = 1;
                if (climb_toggle)
                    zelda_anim_step = 1;
                else
                    zelda_anim_step = 0;
                climb_toggle = 1 - climb_toggle;
            } else {
                if ((unsigned char)hero_actions[hero_action].id == up_action) {
                    zelda_y -= 3;
                    zelda_screen_y -= 3;
                    zelda_sprite_y -= 3;
                    go_find_hero_action(climb_action);
                    anim_frame_idx = 0;
                    dir_state->next = 8;
                } else {
                    zelda_anim_step = 0;
                    zelda_y -= 3;
                    zelda_screen_y -= 3;
                    zelda_sprite_y -= 3;
                    zelda_anim_step = 1;
                }
                zelda_moved = 1;
            }
            get_sprite_block(zelda_x, zelda_y - 1, &jump_info);
            if (!*(long *)ladder_flag) {
                climbing_blk48 = 0;
                climbing_blk25 = 0;
                jump_wait_timer = 0;
                zelda_dir = old_zelda_dir;
                go_find_hero_action(0);
                anim_frame_idx = 0;
            }
        }
    } else if (dir_state->dir == 4 || dir_state->dir == 3 || dir_state->dir == 5) {
        get_sprite_block(zelda_x, zelda_y + 1, &jump_info);
        if (jump_info.track) {
            ++zelda_y;
            ++zelda_screen_y;
            ++zelda_sprite_y;
        } else {
            if (*(long *)ladder_flag) {
                get_sprite_block(zelda_x, zelda_y - 11, &jump_info);
                if (*(long *)ladder_flag) {
                    go_find_hero_action(up_action);
                    ++zelda_y;
                    ++zelda_screen_y;
                    ++zelda_sprite_y;
                    if (climb_toggle)
                        zelda_anim_step = 1;
                    else
                        zelda_anim_step = 0;
                    dir_state->next = 0;
                } else {
                    zelda_anim_step = 0;
                    for (i = 0; i < 3; ++i) {
                        ++zelda_y;
                        ++zelda_screen_y;
                        ++zelda_sprite_y;
                        get_sprite_block(zelda_x, zelda_y + 1, &jump_info);
                        if (!*(long *)ladder_flag)
                            break;
                    }
                    zelda_anim_step = 1;
                }
                climb_toggle = 1 - climb_toggle;
                zelda_moved = 1;
            }
            get_sprite_block(zelda_x, zelda_y + 1, &jump_info);
            if (!*(long *)ladder_flag) {
                if (!old_get_sprite_block(zelda_x, zelda_y + 1) && !old_get_sprite_block(zelda_x + 1, zelda_y - 1)
                        && !old_get_sprite_block(zelda_x + 1, zelda_y) && !old_get_sprite_block(zelda_x + 1, zelda_y + 1)
                        && !old_get_sprite_block(zelda_x - 1, zelda_y - 1) && !old_get_sprite_block(zelda_x - 1, zelda_y)
                        && !old_get_sprite_block(zelda_x - 1, zelda_y + 1)) {
                    ++zelda_y;
                    ++zelda_screen_y;
                    ++zelda_sprite_y;
                    climbing_blk48 = 0;
                    climbing_blk25 = 0;
                    zelda_dir = old_zelda_dir;
                } else {
                    climbing_blk48 = 0;
                    climbing_blk25 = 0;
                    zelda_dir = old_zelda_dir;
                    go_find_hero_action(0);
                    anim_frame_idx = 0;
                    crouch_wait_timer = 0;
                }
            }
        }
    } else if (dir_state->dir == 2) {
        for (i = 1; i <= 3; ++i) {
            get_sprite_block(zelda_x + i, zelda_y - 1, &jump_info);
            if (jump_info.left) {
                --zelda_y;
                --zelda_screen_y;
                --zelda_sprite_y;
                set_link_position_and_dir(i);
                climbing_blk48 = 0;
                climbing_blk25 = 0;
                go_find_hero_action(4);
                anim_frame_idx = 1;
                break;
            }
            if (jump_info_solid)
                break;
        }
        if (!jump_info_solid) {
            jump_off();
            dir_state->next = dir_state->dir;
            dir_state->prev = dir_state->dir;
            set_link_position_and_dir(3);
        }
    } else if (dir_state->dir == 6) {
        for (i = 1; i <= 3; ++i) {
            get_sprite_block(zelda_x - i, zelda_y - 1, &jump_info);
            if (jump_info.right) {
                --zelda_y;
                --zelda_screen_y;
                --zelda_sprite_y;
                set_link_position_and_dir(-i);
                climbing_blk48 = 0;
                climbing_blk25 = 0;
                go_find_hero_action(4);
                anim_frame_idx = 1;
                break;
            }
            if (jump_info_solid)
                break;
        }
        if (!jump_info_solid) {
            jump_off();
            dir_state->next = dir_state->dir;
            dir_state->prev = dir_state->dir;
            set_link_position_and_dir(-3);
        }
    }
}
