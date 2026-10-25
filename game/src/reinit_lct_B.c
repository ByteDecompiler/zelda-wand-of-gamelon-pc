#include "game.h"

void reinit_lct_B(void)
{
    long line_addr;
    unsigned long i;

    line_addr = 0x40000000 | (plane_b_buf0 & 0xffffff);
    line_addr += pf_x_px;
    for (i = 0; i < line_count; ++i) {
        lct_b[i] = line_addr;
        line_addr += line_step;
    }
    dc_wrlct(video_path, lct_plane_b1, 0, 0, 0xf0, 1, &lct_b[pf_y_px]);
    dc_wrlct(video_path, lct_plane_b2, 0, 0, 0xf0, 1, &lct_b[pf_y_px]);
}
