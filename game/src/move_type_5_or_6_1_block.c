#include "game.h"

long move_type_5_or_6_1_block(long sprite_idx, long dx)
{
    char *flag_a;
    char *flag_b;

    if (macros[sprite_idx]->kind == 5) {
        flag_a = &block_ptrs[sprite_idx]->f1b;
        flag_b = &block_ptrs[sprite_idx]->f1c;
    }
    get_sprite_block(sprite_pos[sprite_idx][2] + dx, sprite_pos[sprite_idx][3] - 1, block_ptrs[sprite_idx]);
    if (block_ptrs[sprite_idx]->track) {
        sprite_pos[sprite_idx][2] += dx;
        (&zelda_sprite_x)[sprite_idx] += dx;
        --sprite_pos[sprite_idx][3];
        --(&zelda_sprite_y)[sprite_idx];
        return 0;
    }
    if (*flag_a || *flag_b) {
        do {
            --sprite_pos[sprite_idx][3];
            --(&zelda_sprite_y)[sprite_idx];
            get_sprite_block(sprite_pos[sprite_idx][2] + dx, sprite_pos[sprite_idx][3] - 1, block_ptrs[sprite_idx]);
        } while (*flag_a || *flag_b);
        sprite_pos[sprite_idx][2] += dx;
        (&zelda_sprite_x)[sprite_idx] += dx;
        return 0;
    }
    get_sprite_block(sprite_pos[sprite_idx][2] + dx, sprite_pos[sprite_idx][3], block_ptrs[sprite_idx]);
    if (block_ptrs[sprite_idx]->track || *flag_a || *flag_b) {
        sprite_pos[sprite_idx][2] += dx;
        (&zelda_sprite_x)[sprite_idx] += dx;
        return 0;
    }
    get_sprite_block(sprite_pos[sprite_idx][2] + dx, sprite_pos[sprite_idx][3] + 1, block_ptrs[sprite_idx]);
    if (block_ptrs[sprite_idx]->track) {
        sprite_pos[sprite_idx][2] += dx;
        (&zelda_sprite_x)[sprite_idx] += dx;
        ++sprite_pos[sprite_idx][3];
        ++(&zelda_sprite_y)[sprite_idx];
        return 0;
    }
    if (*flag_a || *flag_b) {
        do {
            ++sprite_pos[sprite_idx][3];
            ++(&zelda_sprite_y)[sprite_idx];
            get_sprite_block(sprite_pos[sprite_idx][2] + dx, sprite_pos[sprite_idx][3] + 1, block_ptrs[sprite_idx]);
        } while (*flag_a || *flag_b);
        sprite_pos[sprite_idx][2] += dx;
        (&zelda_sprite_x)[sprite_idx] += dx;
        return 0;
    }
    get_sprite_block(sprite_pos[sprite_idx][2], sprite_pos[sprite_idx][3] + 1, block_ptrs[sprite_idx]);
    if (block_ptrs[sprite_idx]->track) {
        ++sprite_pos[sprite_idx][3];
        ++(&zelda_sprite_y)[sprite_idx];
        return 0;
    }
    if (*flag_a || *flag_b) {
        do {
            ++sprite_pos[sprite_idx][3];
            ++(&zelda_sprite_y)[sprite_idx];
            get_sprite_block(sprite_pos[sprite_idx][2], sprite_pos[sprite_idx][3] + 1, block_ptrs[sprite_idx]);
        } while (*flag_a || *flag_b);
        return 0;
    }
    return 1;
}
