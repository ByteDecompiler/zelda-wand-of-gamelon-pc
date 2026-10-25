#include "game.h"

void initial_load(void)
{
    long i;

    disc_errors = 0;
    do {
        abort_rtrs();
        set_pcb(0, 0x1f5, 1, 0x7f, 0, 0, 0, (long)data_cil);
        zero_data_cil();
        ((struct pcl **)pcb->data)[0] = bg_pcl;
        ((struct pcl **)pcb->data)[1] = pcl_b;
        ((struct pcl **)pcb->data)[2] = pcl_c;
        ((struct pcl **)pcb->data)[3] = pcl_d;
        ((struct pcl **)pcb->data)[4] = pcl_e;
        ((struct pcl **)pcb->data)[5] = pcl_f;
        set_pcl(bg_pcl, 0, 0, 0, 0x1f7, 0, plane_a_buf1, 0xff, 0, 0);
        set_pcl(pcl_b, 0, 0, 0, 0x1f7, 0, plane_a_buf1 + 0x800, 0xff, 0, 0);
        set_pcl(pcl_c, 0, 0, 0, 0x1f7, 0, menu_icons_buf, 0xff, 0, 0);
        set_pcl(pcl_d, 0, 0, 0, 0x1f7, 0, (long)macro_data, 0xff, 0, 0);
        set_pcl(pcl_e, 0, 0, 0, 0x1f7, 0, zelda_imagery, 0xff, 0, 0);
        set_pcl(pcl_f, 0, 0, 0, 0x1f7, 0, plane_a_buf1 + 0x1000, 0xff, 0, 0);
        play_done = 0;
        play_failed = 0;
        lseek(data_path, STARTUP_SECTOR * 0x800, 0);
        ss_play(data_path, pcb);
        while (!play_done)
            port_idle();
    } while (play_error());
    *(char *)(plane_a_buf1 + 5) = 0;
    *(char *)(plane_a_buf1 + 6) = 0;
    *(char *)(plane_a_buf1 + 7) = 0;
    port_swap_longs(plane_a_buf1, 0x82);
    port_swap_longs(plane_a_buf1 + 0x800, 0x82);
    port_fix_macros((long)macro_data);
    for (i = 0; i < 0x82; ++i)
        clut_table[i] = ((long *)plane_a_buf1)[i];
    for (i = 0; i < 0x82; ++i)
        ((long *)sprite_palette)[i] = ((long *)(plane_a_buf1 + 0x800))[i];
    for (i = 0; i < 0x570; ++i)
        ((char *)cmaps[0])[i] = ((char *)(plane_a_buf1 + 0x1000))[i];
    port_hero_loaded();
}
