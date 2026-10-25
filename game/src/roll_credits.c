#include "game.h"

void roll_credits(void)
{
    long buttons;
    long x;
    long y;
    register long i;
    register long plane;
    long count;
    register long line;
    long (*lct_tables)[240];

    dc_wrli(video_path, lct_plane_a1, 0x196, 1, 0x10000000);
    dc_wrli(video_path, lct_plane_a2, 0x196, 1, 0x10000000);
    dc_wrli(video_path, lct_plane_a1, 20, 1, 0x60000000);
    dc_wrli(video_path, lct_plane_a2, 20, 1, 0x60000000);
    dc_wrfi(video_path, fct_a, 5, 0xc2000001);
    gc_hide(video_path);
    lct_tables = (long (*)[240])plane_a_buf1;
    for (i = 0; i < 240; ++i) {
        lct_tables[0][i] = 0x40000000 | (plane_b_buf0 + i * 0x180 & 0xffffff);
        lct_tables[1][i] = 0x40000000 | (plane_b_buf0 + 0x16b20 + i * 0x180 & 0xffffff);
        lct_tables[2][i] = 0x40000000 | (plane_b_buf0 + 0x2d640 + i * 0x180 & 0xffffff);
        lct_tables[3][i] = 0x40000000 | (plane_b_buf0 + i * 0x180 & 0xffffff);
    }
    lct_tables[0][0] = 0x40000000 | (plane_b_buf0 + 0x180 & 0xffffff);
    lct_tables[1][0] = 0x40000000 | (plane_b_buf0 + 0x180 & 0xffffff);
    lct_tables[2][0] = 0x40000000 | (plane_b_buf0 + 0x180 & 0xffffff);
    lct_tables[3][0] = 0x40000000 | (plane_b_buf0 + 0x180 & 0xffffff);
    dc_wrlct(video_path, lct_plane_b1, 0, 0, 240, 1, lct_tables[0]);
    dc_llnk(video_path, lct_head_b, 2, lct_plane_b1, 0);
    start_playing_credits();
    while (!play_done)
        port_idle();
    play_done = 0;
    for (i = 0; i < 0x180; ++i)
        palette_out[i] = palette_file[i + 4];
    convert_palette();
    sattenuation(0);
    fade_up_plane_B();
    while (!play_done)
        port_idle();
    play_done = 0;
    plane = lct_plane_b2;
    line = 0;
    count = CREDITS_LINES;
    while (pcb->rec > 0 && !play_failed) {
        if (pcb->rec == 1)
            --count;
        if (count > 0) {
            dc_wrlct(video_path, plane, 0, 0, 240, 1, &lct_tables[0][line++]);
            dc_llnk(video_path, lct_head_b, 2, plane, 0);
            plane = plane == lct_plane_b1 ? lct_plane_b2 : lct_plane_b1;
        }
        if (line == 250)
            bg_pcl->cnt = 0;
        if (line == 0x2d0)
            line = 0;
        wait_for_line_event();
        pt_coord(video_path, &buttons, &x, &y);
        if (buttons & 3)
            pcb->rec = 0;
    }
    plane = plane == lct_plane_b1 ? lct_plane_b2 : lct_plane_b1;
    for (i = 0; i < 63; i += 2) {
        wait_for_line_event();
        sattenuation(i * 4);
        set_icf(plane, 1, 63 - i);
    }
    set_icf(plane, 1, 0);
    sattenuation(0xff);
    wait_for_line_event();
    dc_wrlct(video_path, plane, 0, 0, 240, 1, lct_tables[0]);
    load_piracy();
    fade_up_plane_B();
    buttons = 0;
    count = 0;
    while (count++ < 0xa0 && !(buttons & 3)) {
        pt_coord(video_path, &buttons, &x, &y);
        wait_for_line_event();
        wait_for_line_event();
        wait_for_line_event();
    }
    fade_out_plane_B();
    count = 0;
    while (count++ < 10) {
        pt_coord(video_path, &buttons, &x, &y);
        wait_for_line_event();
        wait_for_line_event();
        wait_for_line_event();
    }
}
