#include "game.h"

void init_rtr(void)
{
    long i;

    pcb = (struct pcb *)ctl_tables;
    video_cil = (long *)(ctl_tables + 26);
    cil_b = (long *)(ctl_tables + 0x9a);
    data_cil = (long *)(ctl_tables + 0xda);
    bg_pcl = (struct pcl *)(ctl_tables + 0x15a);
    pcl_b = (struct pcl *)(ctl_tables + 0x176);
    pcl_c = (struct pcl *)(ctl_tables + 0x192);
    pcl_d = (struct pcl *)(ctl_tables + 0x1ae);
    pcl_e = (struct pcl *)(ctl_tables + 0x1ca);
    pcl_f = (struct pcl *)(ctl_tables + 0x1e6);
    for (i = 0; i < 32; ++i) {
        video_cil[i] = 0;
        data_cil[i] = 0;
    }
    for (i = 0; i < 16; ++i)
        cil_b[i] = 0;
    set_pcb(0, 0x1f5, 0, 0, 0, (char *)video_cil, (char *)cil_b, (long)data_cil);
    set_pcl(bg_pcl, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    set_pcl(pcl_b, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    set_pcl(pcl_c, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    set_pcl(pcl_d, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    set_pcl(pcl_e, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    set_pcl(pcl_f, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}
