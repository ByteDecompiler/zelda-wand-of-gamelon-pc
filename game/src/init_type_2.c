#include "game.h"

void init_type_2(long sprite_no, struct macro_def *def)
{
    register struct macro_def2 *def2;

    def2 = (struct macro_def2 *)def;
    if (!def2->macro_no || !def2->loops) {
        sprite_ptrs[sprite_no] = 0;
        sprite_drawn[sprite_no] = 0;
        sprite_asleep[sprite_no] = 0;
        return;
    }
    ((struct macro_b *)macros[sprite_no])->macro_no = def2->macro_no;
    ((struct macro_b *)macros[sprite_no])->loops = def2->loops;
    ((struct macro_b *)macros[sprite_no])->loops_left = def2->loops;
    if (def2->dir_mode != 6 && def2->dir_mode != 10 && def2->dir_mode != 2)
        def2->dir_mode = 2;
    ((struct macro_b *)macros[sprite_no])->dir_mode = def2->dir_mode;
    ((struct macro_b *)macros[sprite_no])->alt2_macro = def2->alt_macro;
    ((struct macro_b *)macros[sprite_no])->b2b = def2->alt_speed;
    if (((struct macro_b *)macros[sprite_no])->b2b < 1)
        ((struct macro_b *)macros[sprite_no])->b2b = 1;
    if (((struct macro_b *)macros[sprite_no])->b2b > 8)
        ((struct macro_b *)macros[sprite_no])->b2b = 8;
    ((struct macro_b *)macros[sprite_no])->alt2_repeat = def2->alt_repeat;
    ((struct macro_b *)macros[sprite_no])->alt2_chance = def2->alt_chance;
    if (def2->alt_dir != 6 && def2->alt_dir != 10 && def2->alt_dir != 2)
        def2->alt_dir = 2;
    ((struct macro_b *)macros[sprite_no])->b2a = def2->alt_dir;
    (&anim_frame_idx)[sprite_no] = 0;
    sprite_macros[sprite_no] = macro_ptrs[(char)(def2->macro_no - 1)];
    if (((struct macro_b *)macros[sprite_no])->dir_mode == 6) {
        (&zelda_sprite_x)[sprite_no] = sprite_pos[sprite_no][2] - 9;
        ((struct macro_b *)macros[sprite_no])->frame_offset = 3;
        macros[sprite_no]->state = 6;
    } else {
        (&zelda_sprite_x)[sprite_no] = sprite_pos[sprite_no][2] - 8;
        ((struct macro_b *)macros[sprite_no])->frame_offset = 0;
        macros[sprite_no]->state = 2;
    }
    (&zelda_sprite_y)[sprite_no] = sprite_pos[sprite_no][3] - 14;
    (&cur_sprite)[sprite_no] = sprite_frames[sprite_no * 24 + get_image_number(sprite_no)];
    sprite_frame_no[sprite_no] = get_image_number(sprite_no);
}
