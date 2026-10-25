#include "game.h"

struct channel channels[6];
unsigned char sound_chan[16];
extern struct smap *smaps[100000];
extern long sound_ids[100000];
extern struct sound_pcb sound_pcbs[100000];

void stop_music(void)
{
    long i;

    if (music_flags & 0x20) {
        for (i = 0; i < 255; i += 8) {
            wait_for_line_event();
            attenuation(i);
        }
        attenuation(255);
        sigmask(1);
        sm_off(music_path2);
        ss_abort(music_path2);
        music_flags &= ~0x20;
        for (i = 0; i < 6; ++i)
            channels[i].flags &= ~0x30;
        sigmask(-1);
    }
}

long play_sound(long sound_id)
{
    sigmask(1);
    if (!music_paused) {
        if (!(music_flags & 0x20)) {
            sigmask(-1);
            return 0;
        }
        stopsound(sound_chan[sound_id]);
        queue_insert_channel(&channel_queue, &channels[sound_chan[sound_id]]);
        channels[sound_chan[sound_id]].flags |= 0x20;
    } else {
        smaps[0]->data = channels[sound_chan[sound_id]].data;
        smaps[0]->size = sound_len[sound_chan[sound_id]] * 0x900;
        smaps[0]->w = sound_len[sound_chan[sound_id]] * 18;
        smaps[0]->pos = 0;
        sound_pcbs[0].a = 0;
        sound_pcbs[0].b = 0;
        sm_out(music_path2, sound_ids[0], sound_pcbs);
    }
    sigmask(-1);
    return 0;
}

void stopsound(long sound_id)
{
    register struct channel *popped;

    sigmask(1);
    if (sound_id == -1) {
        if (cur_channel) {
            cur_channel->flags &= ~0x30;
            cur_channel = 0;
        }
        if (channel_queue)
            while (popped = queue_pop_channel(&channel_queue))
                popped->flags &= ~0x30;
    } else {
        if (&channels[sound_id] == cur_channel)
            cur_channel = 0;
        if (channel_queue)
            while (popped = queue_pop_channel(&channel_queue))
                popped->flags &= ~0x30;
    }
    sigmask(-1);
}
