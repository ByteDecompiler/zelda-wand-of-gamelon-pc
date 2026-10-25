#include "game.h"

void load_piracy(void)
{
    long i;

    ss_abort(anim_path);
    set_pcb(0, 0x1f5, 1, 2, 0, (char *)video_cil, 0, (long)data_cil);
    for (i = 0; i < 32; ++i)
        video_cil[i] = 0;
    for (i = 0; i < 32; ++i)
        data_cil[i] = 0;
    ((struct pcl **)pcb->video)[1] = bg_pcl;
    ((struct pcl **)pcb->data)[1] = pcl_b;
    set_pcl(bg_pcl, 0, 0, 0, 0, 0, plane_b_buf0, 0xff, 0, 0);
    set_pcl(pcl_b, 0, 0, 0, 0, 0, (long)palette_file, 0xff, 0, 0);
    play_done = 0;
    voice_done = 0;
    play_failed = 0;
    lseek(anim_path, ((is_pal ? 0x41 : 0x3d) - 1) * 75 * 20 * 0x800, 0);
    ss_play(anim_path, pcb);
    while (!play_done)
        port_idle();
    for (i = 0; i < 0x180; ++i)
        palette_out[i] = palette_file[i + 4];
    convert_palette();
}
