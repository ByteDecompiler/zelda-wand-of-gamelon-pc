#include "game.h"

void pre_bumper_video_init(void)
{
    char *devparam;
    long path;

    bumper_name = csd_devname(3, 1);
    video_path = open(bumper_name, 3);
    devparam = csd_devparam(bumper_name);
    if (findstr(1, devparam, str_625)) {
        is_pal = 1;
        dc_set_display(video_path, 0);
    } else {
        is_pal = 0;
        if (findstr(1, devparam, str_tv))
            dc_set_display(video_path, 0);
        else
            dc_set_display(video_path, 1);
    }
    free(devparam);
    prepare_mod_dir();
    path = open(cd_dev_name, 0xc3);
    cd_setstat_39(path);
    close(path);
    dc_intl(video_path, 0);
    fct_a = dc_crfct(video_path, 0, 0x96, 0);
    fct_b = dc_crfct(video_path, 1, 0x96, 0);
    lct_plane_a1 = dc_crlct(video_path, 0, 0x1e0, 0);
    lct_plane_a2 = dc_crlct(video_path, 0, 0x1e0, 0);
    lct_plane_b1 = dc_crlct(video_path, 1, 0x1e0, 0);
    lct_plane_b2 = dc_crlct(video_path, 1, 0x1e0, 0);
    lct_head_a = dc_crlct(video_path, 0, 4, 0);
    lct_head_b = dc_crlct(video_path, 1, 4, 0);
    if (is_pal) {
        lct_pal_a = dc_crlct(video_path, 0, 40, 0);
        lct_pal_b = dc_crlct(video_path, 1, 40, 0);
    }
    plane_a_mem = (long)srqcmem(0x47420, 0x80);
    plane_b_mem = (long)srqcmem(0x4d800, 0x81);
    plane_a_buf0 = plane_a_mem;
    plane_a_data = (plane_a_buf1 = plane_a_buf0 + 0x18620) + 0x18600;
    plane_b_buf0 = plane_b_mem;
    plane_b_buf1 = plane_b_mem + 0x16800;
    plane_b_buf2 = plane_b_mem + 0x2d000;
    plane_b_data = plane_b_mem + 0x43800;
    clear_buffers();
    dc_wrfct(video_path, fct_a, 0, 12, dcp_regs_a);
    dc_wrfct(video_path, fct_b, 0, 12, dcp_regs_b);
    dc_flnk(video_path, fct_a, lct_plane_a1, 0);
    dc_flnk(video_path, fct_b, lct_plane_b1, 0);
    dc_llnk(video_path, lct_plane_a1, 0x1de, lct_plane_a1, 0x1de);
    dc_llnk(video_path, lct_plane_b1, 0x1de, lct_plane_b1, 0x1de);
    dc_exec(video_path, fct_a, fct_b);
    ctl_tables = plane_a_data;
    init_rtr();
}
