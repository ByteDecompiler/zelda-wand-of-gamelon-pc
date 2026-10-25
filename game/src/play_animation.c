#include "game.h"

void lantern_brightness();

long play_animation(long anim_number)
{
    long buttons;
    long x;
    long y;
    long i;
    long shown;
    long aborted;

    aborted = 0;
    shown = 0;
    gc_hide(video_path);
    if (in_area)
        convert_palette();
    abort_rtrs();
    plane_B_over_A();
    wait_for_line_event();
    set_icf(lct_plane_a1, 0, 0x3f);
    set_icf(lct_plane_a2, 0, 0x3f);
    set_pcb(0, 0x1f5, 1, 3, 1, 0, 0, (long)data_cil);
    zero_data_cil();
    ((struct pcl **)pcb->data)[1] = bg_pcl;
    set_pcl(bg_pcl, 0, 0, 0, 0x1f8, (char *)pcl_b, plane_a_buf0, 7, (long)init_error_buffer(0), 0);
    set_pcl(pcl_b, 0, 0, 0, 0x1f8, (char *)pcl_c, plane_a_buf0 + 0x3800, 7, (long)init_error_buffer(1), 0);
    set_pcl(pcl_c, 0, 0, 0, 0x1f8, (char *)bg_pcl, plane_a_buf0 + 0x7000, 7, (long)init_error_buffer(2), 0);
    play_done = 0;
    voice_done = 0;
    play_failed = 0;
    bg_loaded = 0;
    anim_flip = 1;
    animation_flip_A();
    wait_for_line_event();
    lseek(anim_path, (anim_number - 1) * 75 * 20 * 0x800, 0);
    debounce_trig();
    clear_416x240(plane_a_buf0 + 32);
    clear_416x240(plane_a_buf1);
    dc_wrfct(video_path, fct_a, 13, 64, clut_table_lo);
    dc_wrfct(video_path, fct_a, 0x4e, 64, clut_table_hi);
    set_fctA_rl7();
    attenuation(0);
    ss_play(anim_path, pcb);
    buttons = 0;
    i = 0;
    while (!play_done) {
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
            wait_for_line_event();
            if (!shown) {
                plane_A_over_B();
                shown = 1;
            }
        } else {
            pt_coord(video_path, &buttons, &x, &y);
            if (buttons & 3) {
                aborted = 1;
                break;
            }
        }
    }
    attenuation(0xff);
    ss_abort(anim_path);
    while (buttons & 3)
        pt_coord(video_path, &buttons, &x, &y);
    play_done = 1;
    if (anim_number == 25 && in_area) {
        dim_lantern();
        lantern_brightness();
    }
    plane_B_over_A();
    wait_for_line_event();
    clear_416x240(plane_a_buf0 + 32);
    clear_416x240(plane_a_buf1);
    wait_for_line_event();
    set_fctA_clut7();
    dc_wrfct(video_path, fct_a, 13, 64, &((long *)sprite_palette)[1]);
    dc_wrfct(video_path, fct_a, 0x4e, 64, &((long *)sprite_palette)[0x42]);
    wait_for_line_event();
    if (play_failed && !aborted) {
        clean_disc();
        plane_B_over_A();
        clear_416x240(plane_a_buf1);
    }
    if (in_area)
        plane_A_over_B();
    attenuation(0);
    return aborted;
}
