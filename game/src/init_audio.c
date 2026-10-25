#include "game.h"

long smap_saved[2];
extern long sound_ids[100000];
extern long sound_mem[100000];
extern struct sound_pcb sound_pcbs[100000];
extern struct smap *smaps[100000];

void init_audio(void)
{
    long i;

    if (!(music_flags & 4)) {
        music_path2 = audio_path;
        music_flags = 4;
        music_side = 0;
        cur_channel = 0;
        channel_queue = 0;
        audio_signal = 0x133;
        audio_task = getpid();
        music_path = open(music_file_name, 1);
        for (i = 0; i < 2; ++i) {
            sound_ids[i] = sm_creat(audio_path, 5, 18, &sound_mem[i]);
            sound_pcbs[i].a = 0;
            sound_pcbs[i].b = (short)audio_signal;
            smaps[i] = sm_info(audio_path, sound_ids[i]);
            smap_saved[i] = smaps[i]->data;
        }
        load_effects();
    }
}
