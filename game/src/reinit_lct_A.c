#include "game.h"

void reinit_lct_A(void)
{
    long line_addr;
    unsigned long i;

    line_addr = 0x40000000 | (plane_a_buf0 + 32 & 0xffffff);
    for (i = 0; i < 0xf0; ++i) {
        lct_a[i] = line_addr;
        line_addr += 0x1a0;
    }
    dc_wrlct(video_path, lct_plane_a1, 0, 0, 0xf0, 1, lct_a);
    line_addr = 0x40000000 | (plane_a_buf1 & 0xffffff);
    for (i = 0; i < 0xf0; ++i) {
        lct_a[i] = line_addr;
        line_addr += 0x1a0;
    }
    dc_wrlct(video_path, lct_plane_a2, 0, 0, 0xf0, 1, lct_a);
}
