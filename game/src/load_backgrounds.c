#include "game.h"

void load_backgrounds(long which)
{
    long pcl_size;
    long rec;

    abort_rtrs();
    if (which == 1) {
        rec = 58;
        pcl_size = 49;
    } else if (which == 0) {
        rec = 57;
        pcl_size = 56;
    } else {
        rec = 60;
        pcl_size = 0xff;
    }
    play_done = 0;
    play_failed = 0;
    set_pcb(0, 0x1f5, 1, 2, 0, (char *)video_cil, 0, (long)data_cil);
    zero_video_cil();
    zero_data_cil();
    if (which == 2)
        ((struct pcl **)pcb->video)[1] = bg_pcl;
    else
        ((struct pcl **)pcb->data)[1] = bg_pcl;
    set_pcl(bg_pcl, 0, 0, 0, 0x1f7, 0, plane_b_buf2, pcl_size, 0, 0);
    lseek(anim_path, (rec - 1) * 75 * 20 * 0x800, 0);
    ss_play(anim_path, pcb);
    while (!play_done && !play_failed)
        port_idle();
}
