#include "game.h"

void finish_move(long state, long dx, long action, long step, long sprite_no)
{
    if (block_ptrs[sprite_no]->solid) {
        macros[sprite_no]->animate = 0;
        macros[sprite_no]->state = state;
        sprite_draw_xs[sprite_no] = sprite_pos[sprite_no][2] - dx;
        go_find_action(sprite_no, action);
        return;
    }
    if (sprite_pos[sprite_no][3] > map_height_px - 1) {
        sprite_pos[sprite_no][3] = map_height_px - 1;
        --sprite_draw_ys[sprite_no];
        sprite_pos[sprite_no][2] += step;
        sprite_draw_xs[sprite_no] += step;
        return;
    }
    if (old_get_sprite_block(sprite_pos[sprite_no][2], sprite_pos[sprite_no][3]))
        return;
    while (sprite_pos[sprite_no][3] < map_height_px - 1 && !block_ptrs[sprite_no]->step && !block_ptrs[sprite_no]->solid
           && !block_ptrs[sprite_no]->f12 && !block_ptrs[sprite_no]->f13 && !block_ptrs[sprite_no]->f14 && !block_ptrs[sprite_no]->f15
           && !block_ptrs[sprite_no]->climb_a && !block_ptrs[sprite_no]->climb_b) {
        ++sprite_pos[sprite_no][3];
        ++sprite_draw_ys[sprite_no];
        get_sprite_block(sprite_pos[sprite_no][2], sprite_pos[sprite_no][3], block_ptrs[sprite_no]);
    }
}
