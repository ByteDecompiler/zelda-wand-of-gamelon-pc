#include "game.h"

long sprite_macros[21];

void init_5_or_6(long sprite_no, long macro_index, long dx, long dy, long size)
{
    write_sprite_blocks(sprite_pos[sprite_no][2] - dx, sprite_pos[sprite_no][3] - dy, size, &((char *)path_blocks)[sprite_no * 18], 3);
    if (sprite_asleep[sprite_no])
        restore_sprite_blocks(sprite_pos[sprite_no][2] - dx, sprite_pos[sprite_no][3] - dy, size, &((char *)path_blocks)[sprite_no * 18]);
    sprite_draw_ys[sprite_no] = sprite_pos[sprite_no][3] - 14;
    (&anim_frame_idx)[sprite_no] = 0;
    sprite_macros[sprite_no] = macro_ptrs[macro_index - 1];
    (&cur_sprite)[sprite_no] = sprite_frames[sprite_no * 24 + get_image_number(sprite_no)];
    sprite_frame_no[sprite_no] = get_image_number(sprite_no);
}
