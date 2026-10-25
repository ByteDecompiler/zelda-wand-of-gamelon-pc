#include "game.h"

void scroll_off_subscreen(long where, long buf)
{
    long y;
    long line_addr;
    long i;

    for (y = 0xf0; y > 0; y -= 8) {
        if (!music_paused)
            attenuation(y / 2);
        line_addr = 0x40000000 | ((0xf0 - y) * 0x1a0 + where & 0xffffff);
        for (i = 0; i < y; ++i) {
            lct_a[i] = line_addr;
            line_addr += 0x1a0;
        }
        line_addr = 0x40000000 | (buf + y * 0x1a0 & 0xffffff);
        for (i = y; i < 0xf0; ++i) {
            lct_a[i] = line_addr;
            line_addr += 0x1a0;
        }
        if (plane_a_buffer == 1) {
            dc_wrlct(video_path, lct_plane_a1, 0, 0, 0xf0, 1, lct_a);
            dc_llnk(video_path, lct_head_a, 2, lct_plane_a1, 0);
        } else {
            dc_wrlct(video_path, lct_plane_a2, 0, 0, 0xf0, 1, lct_a);
            dc_llnk(video_path, lct_head_a, 2, lct_plane_a2, 0);
        }
        plane_a_buffer = 1 - plane_a_buffer;
        wait_for_line_event();
    }
    clear_416x240(where);
    attenuation(0);
}
