#include "game.h"

void flip_backgrounds(long off)
{
    if (plane_b_buffer == 1) {
        dc_wrli(video_path, lct_plane_b1, 0, 0, 0x40000000 | (plane_b_buf2 + off & 0xffffff));
        dc_llnk(video_path, lct_head_b, 2, lct_plane_b1, 0);
    } else {
        dc_wrli(video_path, lct_plane_b2, 0, 0, 0x40000000 | (plane_b_buf2 + off & 0xffffff));
        dc_llnk(video_path, lct_head_b, 2, lct_plane_b2, 0);
    }
    plane_b_buffer = 1 - plane_b_buffer;
}
