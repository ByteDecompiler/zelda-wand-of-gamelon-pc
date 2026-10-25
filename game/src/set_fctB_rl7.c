#include "game.h"

void set_fctB_rl7(void)
{
    unsigned long i;

    dc_wrfi(video_path, fct_b, 6, 0x78000402);
    lct_b[0] = 0x40000000 | (plane_b_buf0 & 0xffffff);
    for (i = 1; i < 0xf0; ++i)
        lct_b[i] = 0x10000000;
    dc_wrlct(video_path, lct_plane_b1, 0, 0, 0xf0, 1, lct_b);
    lct_b[0] = 0x40000000 | (plane_b_buf1 & 0xffffff);
    dc_wrlct(video_path, lct_plane_b2, 0, 0, 0xf0, 1, lct_b);
}
