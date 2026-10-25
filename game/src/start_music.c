#include "game.h"

long smap_saved[2];

void start_music(long track)
{
    long i;

    abort_rtrs();
    music_track = track;
    if (music_paused) {
        attenuation(0);
        return;
    }
    restore_smaps();
    music_side = 0;
    cur_channel = 0;
    channel_queue = 0;
    for (i = 0; i < 16; ++i)
        cil_b[i] = 0;
    cil_b[track % 8] = (long)&sig1_flags;
    music_pcb.stat = 0;
    music_pcb.sig = 0x131;
    music_pcb.rec = 1;
    music_pcb.chan = (&music_chan)[track % 8];
    music_pcb.achan = 0;
    music_pcb.video = 0;
    music_pcb.audio = (char *)cil_b;
    music_pcb.data = 0;
    sig_blocks[1].flags = 0;
    sig_blocks[1].a = 0;
    sig_blocks[1].b = 0;
    sig_blocks[1].sig = 0x132;
    sig_blocks[1].next = &sig0_flags;
    sig_blocks[1].mem = sound_mem[0];
    sig_blocks[1].one = 1;
    sig_blocks[1].err = init_error_buffer(0);
    sig1_count = 0;
    sig_blocks[0].flags = 0;
    sig_blocks[0].a = 0;
    sig_blocks[0].b = 0;
    sig_blocks[0].sig = 0x132;
    sig_blocks[0].next = &sig1_flags;
    sig_blocks[0].mem = sound_mem[1];
    sig_blocks[0].one = 1;
    sig_blocks[0].err = init_error_buffer(1);
    sig0_count = 0;
    stopsound(-1);
    play_done = 0;
    last_side = -1;
    attenuation(0);
    lseek(music_path, (track / 8 * 75 * 120 + track % 8) * 0x800, 0);
    sigmask(1);
    music_flags |= 0x20;
    sigmask(-1);
    ss_play(music_path, &music_pcb);
    while (!music_side && !play_done)
        port_idle();
    memset(smap_saved[0], 0, 0x900);
    wait_for_line_event();
    wait_for_line_event();
    music_wait = 0;
    kill(audio_task, audio_signal);
}
