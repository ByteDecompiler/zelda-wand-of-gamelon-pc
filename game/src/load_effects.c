#include "game.h"

unsigned char sound_len[6];
struct channel channels[6];

void load_effects(void)
{
    long i;
    long sound_offset;

    disc_errors = 0;
    do {
        for (i = 0; i < 16; ++i)
            cil_b[i] = 0;
        cil_b[0] = (long)bg_pcl;
        cil_b[1] = (long)pcl_b;
        cil_b[2] = (long)pcl_c;
        cil_b[3] = (long)pcl_d;
        cil_b[4] = (long)pcl_e;
        cil_b[5] = (long)pcl_f;
        set_pcb(0, 0x1f5, 1, 0x3f, 0, 0, (char *)cil_b, 0);
        for (i = 0; i < 6; ++i) {
            channels[i].next = 0;
            channels[i].f4 = 0;
            channels[i].flags = 0;
            channels[i].len = sound_len[i];
        }
        channels[0].data = sfx_data;
        sound_offset = sound_len[0] * 0x900;
        channels[1].data = sfx_data + sound_offset;
        sound_offset += sound_len[1] * 0x900;
        channels[2].data = sfx_data + sound_offset;
        sound_offset += sound_len[2] * 0x900;
        channels[3].data = sfx_data + sound_offset;
        sound_offset += sound_len[3] * 0x900;
        channels[4].data = sfx4_data;
        channels[5].data = sfx_data + sound_offset;
        set_pcl(bg_pcl, 0, 0, 0, 0x1f7, 0, channels[0].data, 0xff, 0, 0);
        set_pcl(pcl_b, 0, 0, 0, 0x1f7, 0, channels[1].data, 0xff, 0, 0);
        set_pcl(pcl_c, 0, 0, 0, 0x1f7, 0, channels[2].data, 0xff, 0, 0);
        set_pcl(pcl_d, 0, 0, 0, 0x1f7, 0, channels[3].data, 0xff, 0, 0);
        set_pcl(pcl_e, 0, 0, 0, 0x1f7, 0, channels[4].data, 0xff, 0, 0);
        set_pcl(pcl_f, 0, 0, 0, 0x1f7, 0, channels[5].data, 0xff, 0, 0);
        play_done = 0;
        play_failed = 0;
        lseek(data_path, EFFECTS_SECTOR * 0x800, 0);
        ss_play(data_path, pcb);
        while (!play_done)
            port_idle();
    } while (play_error());
}
