#include "game.h"

void flip_world_lcts(long *lct)
{
    if (plane_b_buffer == 1) {
        dc_wrlct(video_path, lct_plane_b1, 0, 0, 0xf0, 1, lct);
        dc_llnk(video_path, lct_head_b, 2, lct_plane_b1, 0);
    } else {
        dc_wrlct(video_path, lct_plane_b2, 0, 0, 0xf0, 1, lct);
        dc_llnk(video_path, lct_head_b, 2, lct_plane_b2, 0);
    }
    plane_b_buffer = 1 - plane_b_buffer;
}
