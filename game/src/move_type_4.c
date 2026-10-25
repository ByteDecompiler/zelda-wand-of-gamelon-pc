#include "game.h"

long move_type_4(long sprite_no, long phase)
{
    long i;
    long stopped;

    stopped = 0;
    get_sprite_block(sprite_pos[sprite_no][2], sprite_pos[sprite_no][3], block_ptrs[sprite_no]);
    if (!block_ptrs[sprite_no]->ground) {
        if (macros[sprite_no]->state == 2) {
            move_1_block(sprite_no, 1, 0);
            if (sprite_pos[sprite_no][3] > map_height_px - 1) {
                sprite_pos[sprite_no][3] = map_height_px - 1;
                (&zelda_sprite_y)[sprite_no] = sprite_pos[sprite_no][3] - 14;
                --sprite_pos[sprite_no][2];
                --(&zelda_sprite_x)[sprite_no];
            }
        } else {
            move_1_block(sprite_no, -1, 0);
            if (sprite_pos[sprite_no][3] > map_height_px - 1) {
                sprite_pos[sprite_no][3] = map_height_px - 1;
                (&zelda_sprite_y)[sprite_no] = sprite_pos[sprite_no][3] - 14;
                ++sprite_pos[sprite_no][2];
                ++(&zelda_sprite_x)[sprite_no];
            }
        }
    } else {
        for (i = 0; i < 3; ++i) {
            if (sprite_pos[sprite_no][3] >= map_height_px - 1 || block_ptrs[sprite_no]->floor || block_ptrs[sprite_no]->solid
                    || block_ptrs[sprite_no]->climb_a || block_ptrs[sprite_no]->climb_b)
                break;
            ++sprite_pos[sprite_no][3];
            ++(&zelda_sprite_y)[sprite_no];
            get_sprite_block(sprite_pos[sprite_no][2], sprite_pos[sprite_no][3], block_ptrs[sprite_no]);
        }
        if (block_ptrs[sprite_no]->floor || block_ptrs[sprite_no]->solid || block_ptrs[sprite_no]->climb_a || block_ptrs[sprite_no]->climb_b) {
            if (macros[sprite_no]->lo & macros[sprite_no]->hi) {
                (&zelda_sprite_x)[sprite_no] = sprite_pos[sprite_no][2] - 9;
                macros[sprite_no]->state = 6;
                go_find_action(sprite_no, 2);
            } else {
                (&zelda_sprite_x)[sprite_no] = sprite_pos[sprite_no][2] - 8;
                macros[sprite_no]->state = 2;
                go_find_action(sprite_no, 3);
            }
            (&anim_frame_idx)[sprite_no] = 0;
            macros[sprite_no]->hi <<= 1;
        } else
            stopped = 1;
        macros[sprite_no]->animate = 0;
    }
    if (old_get_sprite_block(sprite_pos[sprite_no][2], sprite_pos[sprite_no][3]) == 0x4a) {
        if (macros[sprite_no]->lives && --macros[sprite_no]->lives == 0) {
            sprite_ptrs[sprite_no] = 0;
            return 1;
        }
        reinit_sprite(sprite_no);
        return 1;
    }
    if (macros[sprite_no]->animate && !phase)
        macros[sprite_no]->animate = 0;
    if ((unsigned char)macros[sprite_no]->step_delay > 1 && !stopped)
        macros[sprite_no]->animate = 1;
    return 0;
}
