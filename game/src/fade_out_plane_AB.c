#include "game.h"

void fade_out_plane_AB(void)
{
    long status[1];
    long level;

    for (level = 63; level >= 0; level -= 1) {
        set_icf(lct_plane_a1, 0, level);
        set_icf(lct_plane_a2, 0, level);
        dc_rdlct(video_path, lct_plane_b1, 0, 1, 1, 1, status);
        if ((0xdc000000 | (level & 0x3f)) < status[0]) {
            set_icf(lct_plane_b1, 1, level);
            set_icf(lct_plane_b2, 1, level);
        }
    }
}
