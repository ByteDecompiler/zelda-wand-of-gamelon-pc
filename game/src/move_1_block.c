#include "game.h"

void move_1_block(long sprite_idx, long dx, long stop_at_edge)
{
    get_sprite_block(sprite_pos[sprite_idx][2] + dx, sprite_pos[sprite_idx][3] - 1, block_ptrs[sprite_idx]);
    if (!block_ptrs[sprite_idx]->solid) {
        if (block_ptrs[sprite_idx]->step) {
            do {
                --sprite_pos[sprite_idx][3];
                --sprite_pos[sprite_idx][1];
                --(&zelda_sprite_y)[sprite_idx];
                get_sprite_block(sprite_pos[sprite_idx][2] + dx, sprite_pos[sprite_idx][3] - 1, block_ptrs[sprite_idx]);
            } while (block_ptrs[sprite_idx]->step == 1);
            block_ptrs[sprite_idx]->solid = 0;
        } else if (block_ptrs[sprite_idx]->f12 || block_ptrs[sprite_idx]->f13 || block_ptrs[sprite_idx]->f14 || block_ptrs[sprite_idx]->f15
                   || block_ptrs[sprite_idx]->track || block_ptrs[sprite_idx]->deadly) {
            --sprite_pos[sprite_idx][3];
            --sprite_pos[sprite_idx][1];
            --(&zelda_sprite_y)[sprite_idx];
        } else {
            get_sprite_block(sprite_pos[sprite_idx][2] + dx, sprite_pos[sprite_idx][3], block_ptrs[sprite_idx]);
            if (!block_ptrs[sprite_idx]->solid && !block_ptrs[sprite_idx]->floor && !block_ptrs[sprite_idx]->climb_a && !block_ptrs[sprite_idx]->climb_b
                    && !block_ptrs[sprite_idx]->track && !block_ptrs[sprite_idx]->deadly) {
                get_sprite_block(sprite_pos[sprite_idx][2] + dx, sprite_pos[sprite_idx][3] + 1, block_ptrs[sprite_idx]);
                if (block_ptrs[sprite_idx]->left || block_ptrs[sprite_idx]->right) {
                    ++sprite_pos[sprite_idx][3];
                    ++sprite_pos[sprite_idx][1];
                    ++(&zelda_sprite_y)[sprite_idx];
                } else if (!block_ptrs[sprite_idx]->solid) {
                    if (stop_at_edge && (block_ptrs[sprite_idx]->ground || (block_ptrs[sprite_idx]->f1b || block_ptrs[sprite_idx]->f1c) && macros[sprite_idx]->kind != 5)) {
                        macros[sprite_idx]->moved = 0;
                        return;
                    }
                    ++sprite_pos[sprite_idx][3];
                    ++sprite_pos[sprite_idx][1];
                    ++(&zelda_sprite_y)[sprite_idx];
                }
            }
        }
    }
    if (block_ptrs[sprite_idx]->solid) {
        macros[sprite_idx]->animate = 0;
        macros[sprite_idx]->moved = 0;
    } else {
        sprite_pos[sprite_idx][2] += dx;
        sprite_pos[sprite_idx][0] += dx;
        (&zelda_sprite_x)[sprite_idx] += dx;
        macros[sprite_idx]->moved = 1;
    }
}
