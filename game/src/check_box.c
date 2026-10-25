#include "game.h"

long check_box(short *box, long dir, long sprite_idx, long state)
{
    long rect[4];
    long swap;
    long zelda_px;
    long zelda_py;

    rect[0] = (short)port_be16(&box[0]);
    rect[1] = (short)port_be16(&box[1]);
    rect[2] = (short)port_be16(&box[2]);
    rect[3] = (short)port_be16(&box[3]);
    if (dir) {
        if (dir == 2 && state == 6 || dir == 3 && state == 2) {
            swap = -rect[0];
            rect[0] = -rect[2];
            rect[2] = swap;
        }
        rect[0] = rect[0] + sprite_draw_xs[sprite_idx] + 9;
        rect[1] = rect[1] + sprite_draw_ys[sprite_idx] + 7;
        rect[2] = rect[2] + sprite_draw_xs[sprite_idx] + 9;
        rect[3] = rect[3] + sprite_draw_ys[sprite_idx] + 7;
    }
    zelda_px = zelda_sprite_x + 9 + pf_x;
    zelda_py = zelda_sprite_y + 14 + pf_y;
    return zelda_px >= rect[0] && zelda_py >= rect[1] && zelda_px <= rect[2] && zelda_py <= rect[3];
}
