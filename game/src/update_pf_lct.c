#include "game.h"

void update_pf_lct(void)
{
    if (pf_plane_b_buffer == 1) {
        dc_wrlct(video_path, lct_plane_b1, 0, 0, 0xf0, 1, &lct_b[pf_y_px]);
        dc_llnk(video_path, lct_head_b, 2, lct_plane_b1, 0);
    } else {
        dc_wrlct(video_path, lct_plane_b2, 0, 0, 0xf0, 1, &lct_b[pf_y_px]);
        dc_llnk(video_path, lct_head_b, 2, lct_plane_b2, 0);
    }
    pf_plane_b_buffer = 1 - pf_plane_b_buffer;
}
