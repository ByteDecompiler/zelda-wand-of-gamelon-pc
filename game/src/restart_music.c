#include "game.h"

void restart_music(void)
{
    music_pcb.stat = 0;
    music_pcb.sig = 0x131;
    music_pcb.rec = 1;
    init_error_buffer(0);
    init_error_buffer(1);
    play_done = 0;
    lseek(music_path, (music_track / 8 * 75 * 120 + music_track % 8) * 0x800, 0);
    ss_play(music_path, &music_pcb);
}
