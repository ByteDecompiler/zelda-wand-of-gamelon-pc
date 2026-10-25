#include "game.h"

void init_playfield(void)
{
    long x;
    unsigned long i;

    x = 0x40000000 | (plane_b_buf0 & 0xffffff);
    x += pf_x_px;
    for (i = 0; i < line_count; ++i) {
        lct_b[i] = x;
        x += line_step;
    }
    dc_wrlct(video_path, lct_plane_b1, 0, 0, 0xf0, 1, &lct_b[pf_y_px]);
}
