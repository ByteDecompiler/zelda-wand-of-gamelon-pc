#include "game.h"

void init_type_5(long sprite_idx, struct macro_def *def)
{
    register struct macro_def5 *def_ptr;

    def_ptr = (struct macro_def5 *)def;
    if (!def_ptr->kind) {
        sprite_asleep[sprite_idx] = 0;
        sprite_drawn[sprite_idx] = 0;
        sprite_ptrs[sprite_idx] = 0;
        return;
    }
    macros[sprite_idx]->state = def_ptr->start_state;
    ((struct macro5 *)macros[sprite_idx])->spread = def_ptr->spread * 10;
    ((struct macro5 *)macros[sprite_idx])->loops = 0xff;
    ((struct macro5 *)macros[sprite_idx])->kind = def_ptr->kind;
    ((struct macro5 *)macros[sprite_idx])->alt_timer = def_ptr->alt_delay;
    if (def_ptr->speed < 1)
        def_ptr->speed = 1;
    if (def_ptr->speed > 8)
        def_ptr->speed = 8;
    if (def_ptr->speed2 < 1)
        def_ptr->speed2 = 1;
    if (def_ptr->speed2 > 8)
        def_ptr->speed2 = 8;
    if (macros[sprite_idx]->state == 2) {
        macros[sprite_idx]->lo = def_ptr->hi;
        sprite_draw_xs[sprite_idx] = sprite_pos[sprite_idx][2] - 8;
        ((struct macro5 *)macros[sprite_idx])->frame_offset = 0;
        init_5_or_6(sprite_idx, def_ptr->kind, 8, 1, 12);
    } else {
        macros[sprite_idx]->lo = def_ptr->lo;
        sprite_draw_xs[sprite_idx] = sprite_pos[sprite_idx][2] - 9;
        ((struct macro5 *)macros[sprite_idx])->frame_offset = 3;
        init_5_or_6(sprite_idx, def_ptr->kind, 3, 1, 12);
    }
}
