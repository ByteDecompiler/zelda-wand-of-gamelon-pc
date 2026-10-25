#include "game.h"

long sprite_macros[21];

long move_type_2(long sprite_index)
{
    unsigned short code;
    long state;

    ++(&anim_frame_idx)[sprite_index];
    while ((code = *(unsigned short *)(sprite_macros[sprite_index] + (&anim_frame_idx)[sprite_index] * 2)) == 0xffff) {
        (&anim_frame_idx)[sprite_index] = 0;
        if (!--((struct macro_b *)macros[sprite_index])->loops_left) {
            state = macros[sprite_index]->state;
            ((struct macro_b *)macros[sprite_index])->loops_left = ((struct macro_b *)macros[sprite_index])->loops;
            if (((struct macro_b *)macros[sprite_index])->dir_mode == 10) {
                if (macros[sprite_index]->state == 2) {
                    macros[sprite_index]->state = 6;
                    ((struct macro_b *)macros[sprite_index])->frame_offset = 3;
                    sprite_draw_xs[sprite_index] = sprite_pos[sprite_index][2] - 9;
                } else {
                    macros[sprite_index]->state = 2;
                    ((struct macro_b *)macros[sprite_index])->frame_offset = 0;
                    sprite_draw_xs[sprite_index] = sprite_pos[sprite_index][2] - 8;
                }
            }
            if (check_for_alternate_macro(sprite_index, state))
                return 1;
        }
    }
    get_new_macro_image(sprite_index, ((struct macro_b *)macros[sprite_index])->frame_offset, code);
    return 0;
}
