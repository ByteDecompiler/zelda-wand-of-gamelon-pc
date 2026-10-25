#include "game.h"

extern long dcp_regs_c[];
extern long dcp_regs_d[];

long color_regs[8];

void post_bumper_video_init(void)
{
    clear_buffers();
    plane_a_extra = (long)srqcmem(PLANE_A_EXTRA_SIZE, 0x80);
    align_buffers_and_tables();
    dc_wrfct(video_path, fct_a, 0, 12, dcp_regs_c);
    dc_wrfct(video_path, fct_b, 0, 12, dcp_regs_d);
    plane_B_over_A();
    ((long *)sprite_palette)[0] = 0xc3000000;
    ((long *)sprite_palette)[0x41] = 0xc3000001;
    dc_wrfct(video_path, fct_a, 12, 0x82, (long *)sprite_palette);
    dc_wrfct(video_path, fct_a, 0x8e, 8, color_regs);
    ((long *)sprite_palette)[0] = 0xc3000002;
    ((long *)sprite_palette)[0x41] = 0xc3000003;
    dc_wrfct(video_path, fct_b, 12, 0x82, (long *)sprite_palette);
    dc_wrfct(video_path, fct_b, 0x8e, 8, color_regs);
    ((long *)sprite_palette)[0] = 0xc3000000;
    ((long *)sprite_palette)[0x41] = 0xc3000001;
    dc_wrli(video_path, lct_plane_a1, 0, 0, 0x40000000 | (plane_a_buf0 & 0xffffff));
    dc_wrli(video_path, lct_plane_a2, 0, 0, 0x40000000 | (plane_a_buf1 & 0xffffff));
    dc_wrli(video_path, lct_plane_b1, 0, 0, 0x40000000 | (plane_b_buf0 & 0xffffff));
    dc_wrli(video_path, lct_plane_b2, 0, 0, 0x40000000 | (plane_b_buf0 & 0xffffff));
    set_icf(lct_plane_a1, 0, 0x3f);
    set_icf(lct_plane_a2, 0, 0x3f);
    set_icf(lct_plane_b1, 1, 0);
    set_icf(lct_plane_b2, 1, 0x3f);
    dc_wrli(video_path, lct_plane_a1, 0x196, 1, 0x60000000);
    dc_wrli(video_path, lct_plane_a2, 0x196, 1, 0x60000000);
    dc_wrli(video_path, lct_head_a, 0, 0, 0x40000000 | (plane_a_buf0 & 0xffffff));
    dc_wrli(video_path, lct_head_b, 0, 0, 0x40000000 | (plane_b_buf0 & 0xffffff));
    set_icf(lct_head_a, 0, 0);
    set_icf(lct_head_b, 1, 0);
    dc_llnk(video_path, lct_head_a, 2, lct_plane_a1, 0);
    dc_llnk(video_path, lct_head_b, 2, lct_plane_b1, 0);
    if (is_pal) {
        dc_llnk(video_path, lct_pal_a, 0x26, lct_head_a, 0);
        dc_llnk(video_path, lct_pal_b, 0x26, lct_head_b, 0);
        dc_llnk(video_path, lct_plane_a1, 0x1de, lct_pal_a, 0);
        dc_llnk(video_path, lct_plane_a2, 0x1de, lct_pal_a, 0);
        dc_llnk(video_path, lct_plane_b1, 0x1de, lct_pal_b, 0);
        dc_llnk(video_path, lct_plane_b2, 0x1de, lct_pal_b, 0);
        dc_wrli(video_path, lct_pal_a, 0, 0, 0x40000000 | (plane_a_buf0 & 0xffffff));
        dc_wrli(video_path, lct_pal_b, 0, 0, 0x40000000 | (plane_b_buf0 & 0xffffff));
        set_icf(lct_pal_a, 0, 0);
        set_icf(lct_pal_b, 1, 0);
        dc_flnk(video_path, fct_a, lct_pal_a, 0);
        dc_flnk(video_path, fct_b, lct_pal_b, 0);
    } else {
        dc_flnk(video_path, fct_a, lct_head_a, 0);
        dc_flnk(video_path, fct_b, lct_head_b, 0);
    }
    dc_ssig(video_path, 0x1f4, 0);
    plane_a_buffer = 0;
}
