#include "game.h"

void (*proj_draw[48])();

void load_playfield(long room, long full)
{
    long *id_table;
    long i;
    long cmap_longs;
    long gfx_addr;
    short *src;
    struct enemy_gfx *gfx_hdr;

    disc_errors = 0;
    do {
        abort_rtrs();
        set_pcb(0, 0x1f5, full ? 2 : 1, 0x1f, 0, (char *)video_cil, 0, (long)data_cil);
        zero_data_cil();
        zero_video_cil();
        ((struct pcl **)pcb->data)[0] = bg_pcl;
        ((struct pcl **)pcb->data)[1] = pcl_b;
        ((struct pcl **)pcb->data)[2] = pcl_c;
        ((struct pcl **)pcb->data)[3] = pcl_d;
        ((struct pcl **)pcb->data)[4] = pcl_e;
        ((struct pcl **)pcb->video)[4] = pcl_f;
        set_pcl(bg_pcl, 0, 0, 0, 0x1f7, 0, enemy_list, 0xff, 0, 0);
        set_pcl(pcl_b, 0, 0, 0, 0x1f7, 0, env_proj_buf, 0xff, 0, 0);
        set_pcl(pcl_c, 0, 0, 0, 0x1f7, 0, room_gfx, 0xff, 0, 0);
        set_pcl(pcl_d, 0, 0, 0, 0x1f7, 0, (long)block_map, 0xff, 0, 0);
        set_pcl(pcl_e, 0, 0, 0, 0x1f7, 0, (long)palette_file, 0xff, 0, 0);
        set_pcl(pcl_f, 0, 0, 0, 0x1f7, 0, plane_b_buf0, 0xff, 0, 0);
        play_done = 0;
        voice_done = 0;
        play_failed = 0;
        lseek(data_path, (room * 0xc0 + ROOM_SECTOR) * 0x800, 0);
        ss_play(data_path, pcb);
        while (!play_done && !voice_done)
            port_idle();
        if (!play_failed) {
            reset_pcl(bg_pcl, 0, (char *)room_gfx, 0);
            reset_pcl(pcl_b, 0, (char *)plane_a_buf1, 0);
            if (full) {
                reset_pcl(pcl_c, 0, palette_file, 0);
                reset_pcl(pcl_d, 0, (char *)proj_gfx_buf, 0);
                reset_pcl(pcl_e, 0, (char *)tool_imagery_buf, 0);
            } else {
                reset_pcl(pcl_c, 0, 0, 0);
                reset_pcl(pcl_d, 0, 0, 0);
                reset_pcl(pcl_e, 0, 0, 0);
            }
            src = (short *)room_gfx;
            for (i = 0; i < 6; ++i) {
                bg_cycle_start[i] = port_be16(&src[0]);
                bg_cycle_len[i] = port_be16(&src[6]);
                bg_cycle_delay[i] = port_be16(&src[12]);
                ++src;
            }
            for (i = 0; i < 0x180; ++i)
                palette_out[i] = palette_file[i + 4];
            convert_palette();
            while (pcb->rec > 0 && !play_failed)
                port_idle();
        }
    } while (play_error());
    port_fix_room();
    id_table = (long *)enemy_list;
    gfx_addr = room_gfx;
    for (i = 0; id_table[i] != -1; ++i) {
        enemy_types[i].gfx = (struct enemy_gfx *)gfx_addr;
        enemy_types[i].id = id_table[i];
        gfx_hdr = (struct enemy_gfx *)gfx_addr;
        gfx_addr = gfx_addr + gfx_hdr->f8 + gfx_hdr->f4;
    }
    cmap_longs = 0x15c;
    for (i = 0; i < cmap_longs; ++i) {
        ((long *)cmaps[1])[i] = ((long *)plane_a_buf1)[i];
        ((long *)cmaps[2])[i] = ((long *)plane_a_buf1)[i + cmap_longs];
        ((long *)cmaps[3])[i] = ((long *)plane_a_buf1)[i + cmap_longs + cmap_longs];
    }
    if (full) {
        cmap_longs = 0x15c;
        for (i = 0; i < cmap_longs; ++i)
            ((long *)proj_cmap_base)[i] = ((long *)palette_file)[i];
        gfx_hdr = (struct enemy_gfx *)proj_gfx_buf;
        gfx_addr = (long)gfx_hdr + 0x9c;
        for (i = 0; i < 24; ++i) {
            proj_draw[i] = (void (*)())(gfx_addr + gfx_hdr->frames[i]);
            proj_cmaps[i] = proj_cmap_base + i * 0x3a;
        }
        gfx_hdr = (struct enemy_gfx *)(proj_gfx_buf + gfx_hdr->f8 + gfx_hdr->f4);
        gfx_addr = (long)gfx_hdr + 0x9c;
        for (i = 0; i < 24; ++i)
            proj_draw[i + 24] = (void (*)())(gfx_addr + gfx_hdr->frames[i]);
        load_tool_imagery(tool_imagery_buf);
    }
}
