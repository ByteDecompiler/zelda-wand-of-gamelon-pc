#include "game.h"

long move_types_1_and_3(long sprite_idx, long phase)
{
    register long state;

    macros[sprite_idx]->animate = 1;
    if (macros[sprite_idx]->hi == macros[sprite_idx]->lo) {
        if (macros[sprite_idx]->kind == 3) {
            if (check_for_alternate_macro(sprite_idx, macros[sprite_idx]->state))
                return 1;
        }
        return 0;
    }
    if (sprite_pos[sprite_idx][2] == macros[sprite_idx]->target) {
        state = macros[sprite_idx]->state;
        if (macros[sprite_idx]->state == 2) {
            (&zelda_sprite_x)[sprite_idx] = sprite_pos[sprite_idx][2] - 9;
            macros[sprite_idx]->animate = 0;
            macros[sprite_idx]->state = 6;
            go_find_action(sprite_idx, 2);
            if (sprite_pos[sprite_idx][2] == (short)macros[sprite_idx]->lo)
                macros[sprite_idx]->target = macros[sprite_idx]->lo;
            else
                get_random_bound(sprite_idx, 6);
            if (macros[sprite_idx]->target == sprite_pos[sprite_idx][2])
                return 0;
        } else {
            (&zelda_sprite_x)[sprite_idx] = sprite_pos[sprite_idx][2] - 8;
            macros[sprite_idx]->animate = 0;
            macros[sprite_idx]->state = 2;
            go_find_action(sprite_idx, 3);
            if (sprite_pos[sprite_idx][2] == (short)macros[sprite_idx]->hi)
                macros[sprite_idx]->target = macros[sprite_idx]->hi;
            else
                get_random_bound(sprite_idx, 2);
            if (macros[sprite_idx]->target == sprite_pos[sprite_idx][2])
                return 0;
        }
        if (macros[sprite_idx]->kind == 3 && check_for_alternate_macro(sprite_idx, state))
            return 1;
    }
    macros[sprite_idx]->animate = 1;
    if (macros[sprite_idx]->state == 2) {
        move_1_block(sprite_idx, 1, 0);
        finish_move(6, 9, 2, -1, sprite_idx);
    } else if (macros[sprite_idx]->state == 6) {
        move_1_block(sprite_idx, -1, 0);
        finish_move(2, 8, 3, 1, sprite_idx);
    }
    if (macros[sprite_idx]->animate && !phase)
        macros[sprite_idx]->animate = 0;
    return 0;
}
