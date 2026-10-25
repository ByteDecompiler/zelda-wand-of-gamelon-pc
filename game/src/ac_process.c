#include "game.h"

long chan_offsets[4];
long smap_saved[2];

long ac_process(long sig)
{
    short side;

    if (sig == audio_signal) {
        if (!(music_flags & 0x20))
            return 0;
        side = !music_side;
        if (side == last_side) {
            memset(smap_saved[side], 0, 0x900);
            if (music_wait) {
                play_done = 1;
                music_wait = 0;
            }
        }
        last_side = side;
        if (!cur_channel) {
            if (channel_queue) {
                chan_pos = 0;
                cur_channel = queue_pop_channel(&channel_queue);
                cur_channel->flags &= ~0x20;
                cur_channel->flags |= 0x10;
            }
        } else if (++chan_pos >= cur_channel->len) {
            cur_channel->flags &= ~0x30;
            cur_channel->next = 0;
            cur_channel = 0;
            if (channel_queue) {
                chan_pos = 0;
                cur_channel = queue_pop_channel(&channel_queue);
                cur_channel->flags &= ~0x20;
                cur_channel->flags |= 0x10;
            }
        }
        if (cur_channel)
            copy_sound(sound_mem[side], cur_channel->data + chan_offsets[chan_pos]);
        sound_pcbs[side].a = 0;
        sound_pcbs[side].b = (short)audio_signal;
        if (sm_out(music_path2, sound_ids[side], &sound_pcbs[side]) == -1) {
            music_flags &= ~0x20;
            return -1;
        }
    }
    return 0;
}
