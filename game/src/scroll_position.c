#include "game.h"

void scroll_position(long y)
{
    register long i;
    register long line_cmd;

    line_cmd = 0x40000000 | (plane_a_buf0 & 0xffffff);
    i = 0xef;
    while (i > 0xde) {
        lct_a[i] = line_cmd;
        --i;
    }
    if (y > 0xde)
        y = 0xde;
    for (i = 0xde; i > y; --i)
        lct_a[i] = line_cmd;
    i = y;
    for (i = 0; i <= 10 && y > 0; ++i, --y)
        lct_a[y] = (0xde - i) * 0x1a0 + line_cmd;
    for (i = y - 28; i > 0; --i, --y)
        lct_a[y] = (i + 28) * 0x1a0 + line_cmd;
    for (i = 15; y >= 0 && i >= 0; --i, --y)
        lct_a[y] = (i + 13) * 0x1a0 + line_cmd;
    while (y >= 0) {
        lct_a[y] = line_cmd;
        --y;
    }
    wait_for_line_event();
    if (plane_a_buffer == 1) {
        dc_wrlct(video_path, lct_plane_a1, 0, 0, 0xf0, 1, lct_a);
        dc_llnk(video_path, lct_head_a, 2, lct_plane_a1, 0);
    } else {
        dc_wrlct(video_path, lct_plane_a2, 0, 0, 0xf0, 1, lct_a);
        dc_llnk(video_path, lct_head_a, 2, lct_plane_a2, 0);
    }
    flip_A();
}
