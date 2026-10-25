#include "game.h"

void play_title(void)
{
    long i;
    long j;
    long pressed;
    long shown;

    pressed = 0;
    shown = 0;
    abort_rtrs();
    set_pcb(0, 0x1f5, 1, 7, 0, (char *)video_cil, 0, (long)data_cil);
    zero_data_cil();
    zero_video_cil();
    ((struct pcl **)pcb->data)[0] = bg_pcl;
    ((struct pcl **)pcb->video)[0] = pcl_b;
    ((struct pcl **)pcb->data)[1] = pcl_c;
    ((struct pcl **)pcb->video)[1] = pcl_d;
    ((struct pcl **)pcb->data)[2] = pcl_e;
    set_pcl(bg_pcl, 0, 0, 0, 0, 0, plane_b_buf1, 0xff, 0, 0);
    set_pcl(pcl_b, 0, 0, 0, 0, 0, plane_b_buf0, 0xff, 0, 0);
    set_pcl(pcl_c, 0, 0, 0, 0, 0, plane_a_buf1, 0xff, 0, 0);
    set_pcl(pcl_d, 0, 0, 0, 0, 0, plane_a_buf1 + 0x800, 0xff, 0, 0);
    set_pcl(pcl_e, 0, 0, 0, 0, 0, plane_b_buf2, 0xff, 0, 0);
    play_done = 0;
    play_failed = 0;
    lseek(anim_path, 0xa9ec000, 0);
    ss_play(anim_path, pcb);
    while (!play_done)
        port_idle();
    install_palette(fct_a, (char *)plane_b_buf2, (char *)clut_table);
    install_palette(fct_b, (char *)(plane_b_buf1 + 4), (char *)palette_in);
    set_fctA_rl7();
    set_fctB_rl7();
    dc_wrli(video_path, lct_plane_b1, 0, 0, 0x40000000 | (plane_b_buf0 & 0xffffff));
    dc_llnk(video_path, lct_head_b, 2, lct_plane_b1, 0);
    attenuation(0);
    plane_B_over_A();
    set_pcb(0, 0x1f5, 4, 3, 1, 0, 0, (long)data_cil);
    zero_data_cil();
    ((struct pcl **)pcb->data)[1] = bg_pcl;
    set_pcl(bg_pcl, 0, 0, 0, 0x1f8, (char *)pcl_b, plane_a_buf0, 7, 0, 0);
    set_pcl(pcl_b, 0, 0, 0, 0x1f8, (char *)pcl_c, plane_a_buf0 + 0x3800, 7, 0, 0);
    set_pcl(pcl_c, 0, 0, 0, 0x1f8, (char *)bg_pcl, plane_a_buf0 + 0x7000, 7, 0, 0);
    set_icf(lct_plane_a1, 0, 0x35);
    set_icf(lct_plane_a2, 0, 0x35);
    play_done = 0;
    voice_done = 0;
    play_failed = 0;
    bg_loaded = 0;
    anim_flip = 1;
    animation_flip_A();
    lseek(anim_path, 0xacda000, 0);
    memset(plane_a_buf0, 0, 0xa800);
    ss_play(anim_path, pcb);
    for (i = 0; i <= 0x35; ++i) {
        wait_for_line_event();
        set_icf(lct_plane_b1, 1, i);
        set_icf(lct_plane_b2, 1, i);
    }
    while (!play_done)
        if (pressed = check_buttons())
            break;
    play_done = 0;
    for (i = 10; i <= 0x3f; ++i) {
        wait_for_line_event();
        set_icf(lct_plane_b1, 1, 0x3f - i);
        set_icf(lct_plane_b2, 1, 0x3f - i);
    }
    if (!pressed) {
        j = 0;
        while (!play_done) {
            if (bg_loaded) {
                if (anim_flip)
                    dc_wrli(video_path, lct_plane_a1, 0, 0, 0x40000000 | (plane_a_buf0 + j * 0x3800 & 0xffffff));
                else
                    dc_wrli(video_path, lct_plane_a2, 0, 0, 0x40000000 | (plane_a_buf0 + j * 0x3800 & 0xffffff));
                (&bg_pcl)[j]->ctrl = 0;
                (&bg_pcl)[j]->cnt = 0;
                ++j;
                if (j > 2)
                    j = 0;
                animation_flip_A();
                bg_loaded = 0;
                wait_for_line_event();
                if (!shown) {
                    plane_A_over_B();
                    shown = 1;
                }
            } else if (pressed = check_buttons()) {
                fade_out_plane_A();
                goto done;
            }
        }
        dc_wrli(video_path, lct_plane_b1, 0, 0, 0x40000000 | (plane_b_buf0 + 0x1b3c & 0xffffff));
        install_palette(fct_b, (char *)(plane_b_buf1 + 0x804), (char *)palette_in);
        dc_wrfi(video_path, fct_a, 1, 0xc1000808);
        for (i = 0; i <= 0x35; ++i) {
            wait_for_line_event();
            set_icf(lct_plane_a1, 0, 0x35 - i);
            set_icf(lct_plane_a2, 0, 0x35 - i);
            set_icf(lct_plane_b1, 1, i);
            set_icf(lct_plane_b2, 1, i);
        }
        install_palette(fct_a, (char *)(plane_a_buf1 + 4), (char *)clut_table);
        dc_wrli(video_path, lct_plane_a1, 0, 0, 0x40000000 | (plane_a_buf1 + 0x800 & 0xffffff));
        dc_wrli(video_path, lct_plane_a2, 0, 0, 0x40000000 | (plane_a_buf1 + 0x800 & 0xffffff));
        play_done = 0;
        while (!play_done)
            if (pressed = check_buttons()) {
                fade_out_plane_B();
                goto done;
            }
        for (i = 0; i <= 0x35; ++i) {
            wait_for_line_event();
            set_icf(lct_plane_a1, 0, i);
            set_icf(lct_plane_a2, 0, i);
            set_icf(lct_plane_b1, 1, 0x35 - i);
            set_icf(lct_plane_b2, 1, 0x35 - i);
        }
        for (i = 0; i < 120; ++i)
            wait_for_line_event();
        for (i = 0; i <= 0x35; ++i) {
            wait_for_line_event();
            set_icf(lct_plane_a1, 0, 0x35 - i);
            set_icf(lct_plane_a2, 0, 0x35 - i);
        }
    }
done:
    dc_wrfi(video_path, fct_a, 1, 0xc1800801);
    set_fctA_clut7();
    dc_wrfi(video_path, fct_b, 6, 0x78000400);
    wait_for_line_event();
    plane_B_over_A();
    wait_for_line_event();
    set_icf(lct_plane_a1, 0, 0x3f);
    set_icf(lct_plane_a2, 0, 0x3f);
}
