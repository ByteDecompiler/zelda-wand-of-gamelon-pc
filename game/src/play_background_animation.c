#include "game.h"

long bg_frames_a[3];

void play_background_animation(long which)
{
    long buttons;
    long x;
    long y;
    long aborted;
    long shown;
    long i;
    long k;
    long bg_index;
    long done;
    long *bg_tab;

    buttons = 0;
    aborted = 0;
    bg_index = 0;
    done = 0;
    done = 0;
    shown = 0;
    gc_hide(video_path);
    fade_out_plane_AB();
    abort_rtrs();
    if (!which)
        memmove(plane_a_buf0 + 0xa800, plane_b_buf2, 0x1c000);
    load_backgrounds(which);
    ss_abort(anim_path);
    if (which == 1) {
        bg_tab = bg_frames_a;
        i = 0x37;
        k = 13;
    } else {
        bg_tab = bg_frames_b;
        i = 0x33;
        k = 16;
    }
    dc_wrfct(video_path, fct_a, 13, 64, clut_table_lo);
    dc_wrfct(video_path, fct_a, 0x4e, 64, clut_table_hi);
    dc_wrfct(video_path, fct_b, 13, 64, clut_table_lo);
    dc_wrfct(video_path, fct_b, 0x4e, 64, clut_table_hi);
    set_pcb(0, 0x1f5, k, 3, 1, 0, 0, (long)data_cil);
    zero_data_cil();
    ((struct pcl **)pcb->data)[1] = bg_pcl;
    set_pcl(bg_pcl, 0, 0, 0, 0x1f8, (char *)pcl_b, plane_a_buf0, 7, (long)init_error_buffer(0), 0);
    set_pcl(pcl_b, 0, 0, 0, 0x1f8, (char *)pcl_c, plane_a_buf0 + 0x3800, 7, (long)init_error_buffer(1), 0);
    set_pcl(pcl_c, 0, 0, 0, 0x1f8, (char *)bg_pcl, plane_a_buf0 + 0x7000, 7, (long)init_error_buffer(2), 0);
    set_pcl(pcl_d, 0, 0, 0, 0x1f7, 0, (long)palette_file, 0xff, 0, 0);
    set_icf(lct_plane_a1, 0, 0);
    set_icf(lct_plane_b1, 1, 0);
    play_done = 0;
    voice_done = 0;
    play_failed = 0;
    bg_loaded = 0;
    anim_flip = 1;
    animation_flip_A();
    lseek(anim_path, (i - 1) * 75 * 20 * 0x800, 0);
    debounce_trig();
    set_fctA_rl7();
    set_fctB_rl7();
    memset(plane_a_buf0, 0, 0xa800);
    plane_b_buffer = 1;
    flip_backgrounds(bg_tab[bg_index++] * 0x800);
    set_icf(lct_plane_a2, 0, 0x3f);
    set_icf(lct_plane_b2, 1, 0x3f);
    plane_A_over_B();
    attenuation(0);
    ss_play(anim_path, pcb);
    buttons = 0;
    k = 0;
    i = 0;
    while (pcb->rec > 0 && !play_failed) {
        if (bg_loaded) {
            if (anim_flip)
                dc_wrli(video_path, lct_plane_a1, 0, 0, 0x40000000 | (plane_a_buf0 + i * 0x3800 & 0xffffff));
            else
                dc_wrli(video_path, lct_plane_a2, 0, 0, 0x40000000 | (plane_a_buf0 + i * 0x3800 & 0xffffff));
            (&bg_pcl)[i]->ctrl = 0;
            (&bg_pcl)[i]->cnt = 0;
            ++i;
            if (i > 2)
                i = 0;
            animation_flip_A();
            bg_loaded = 0;
            if (k == 2 && bg_index < 15) {
                flip_backgrounds(bg_tab[bg_index++] * 0x800);
                k = 0;
            }
            if (k == 1)
                k = 2;
            if (!shown) {
                set_icf(lct_plane_a1, 0, 0x3f);
                set_icf(lct_plane_b1, 1, 0x3f);
                shown = 1;
            }
            if (!which && bg_index >= 15 && !done) {
                dc_wrfct(video_path, fct_b, 13, 64, palette_in_lo);
                dc_wrfct(video_path, fct_b, 0x4e, 64, palette_in_hi);
                dc_wrfi(video_path, fct_b, 6, 0x78000400);
                map_cursor_x = MAP_CURSOR_START_X;
                saved_map_cursor_x = MAP_CURSOR_START_X;
                map_cursor_y = 80;
                saved_map_cursor_y = 80;
                map_x = 0;
                saved_map_x = 0;
                map_y = 0;
                saved_map_y = 0;
                init_map();
                ++bg_index;
                done = 1;
            }
        } else {
            pt_coord(video_path, &buttons, &x, &y);
            if (buttons & 3) {
                aborted = 1;
                mkt_demo_end = 1;
                break;
            }
        }
        if (play_done) {
            k = 1;
            play_done = 0;
        }
    }
    attenuation(0xff);
    ss_abort(anim_path);
    while (buttons & 3)
        pt_coord(video_path, &buttons, &x, &y);
    play_done = 1;
    if ((which || !done) && shown)
        fade_out_plane_AB();
    plane_B_over_A();
    wait_for_line_event();
    set_fctA_clut7();
    dc_wrfct(video_path, fct_a, 13, 64, &((long *)sprite_palette)[1]);
    dc_wrfct(video_path, fct_a, 0x4e, 64, &((long *)sprite_palette)[0x42]);
    if (!which)
        memmove(plane_b_buf2, plane_a_buf0 + 0xa800, 0x1c000);
    if (!done) {
        dc_wrfi(video_path, fct_b, 6, 0x78000400);
        if (!which) {
            dc_wrfct(video_path, fct_b, 13, 64, palette_in_lo);
            dc_wrfct(video_path, fct_b, 0x4e, 64, palette_in_hi);
            init_map();
            fade_up_plane_B();
        }
    }
    if (play_failed && !aborted) {
        clean_disc();
        plane_B_over_A();
    }
    if (!which)
        sattenuation(0);
    set_icf(lct_plane_a1, 0, 0x3f);
    set_icf(lct_plane_a2, 0, 0x3f);
}
