#include "game.h"

void init_type_8(long sprite_no, register struct macro_def *def)
{
    register char *save;
    long base;
    long i;

    if (!((struct macro_def6 *)def)->state) {
        sprite_ptrs[sprite_no] = 0;
        sprite_drawn[sprite_no] = 0;
        sprite_asleep[sprite_no] = 0;
        return;
    }
    ((struct macro_b *)macros[sprite_no])->macro_no = ((struct macro_def6 *)def)->state;
    ((struct macro_def6 *)def)->kind = 2;
    ((struct macro_b *)macros[sprite_no])->frame_offset = ((struct macro_def6 *)def)->kind;
    base = sprite_pos[sprite_no][2] - 9 + (sprite_pos[sprite_no][3] - 1) * map_width;
    save = &((char *)path_blocks)[sprite_no * 18];
    for (i = 0; i < 18; ++i) {
        save[i] = block_map[base + i];
        if (block_map[base + i] == 0x5e || block_map[base + i] == 0x4f || block_map[base + i] == 0x54
                || block_map[base + i] == 0x5b || block_map[base + i] == 0x59 || block_map[base + i] == 0x4d)
            block_map[base + i] = 0;
    }
    (&anim_frame_idx)[sprite_no] = 0;
    sprite_macros[sprite_no] = macro_ptrs[(unsigned char)(((struct macro_def6 *)def)->state - 1)];
    sprite_draw_xs[sprite_no] = sprite_pos[sprite_no][2] - 8;
    ((struct macro_b *)macros[sprite_no])->alt2_macro = 0;
    macros[sprite_no]->state = 2;
    sprite_draw_ys[sprite_no] = sprite_pos[sprite_no][3] - 14;
    (&cur_sprite)[sprite_no] = sprite_frames[sprite_no * 24 + get_image_number(sprite_no)];
    sprite_frame_no[sprite_no] = get_image_number(sprite_no);
}
