#include "game.h"

void get_new_macro_image(long sprite_index, unsigned char flip, unsigned short code)
{
    short frame;
    short shift;

    frame = code & 0x1f;
    frame += flip;
    (&cur_sprite)[sprite_index] = sprite_frames[sprite_index * 24 + frame];
    sprite_frame_no[sprite_index] = frame;
    shift = code >> 8 & 0xf;
    if (code & 0x40)
        shift = -shift;
    if (!flip) {
        sprite_pos[sprite_index][2] += shift;
        sprite_draw_xs[sprite_index] += shift;
    } else {
        sprite_pos[sprite_index][2] -= shift;
        sprite_draw_xs[sprite_index] -= shift;
    }
    shift = code >> 12;
    if (code & 0x80)
        shift = -shift;
    sprite_pos[sprite_index][3] += shift;
    sprite_draw_ys[sprite_index] += shift;
}
