#include "game.h"

void play_overworld_music(void)
{
    if (disc_errors == 3)
        return;
    abort_rtrs();
    set_pcb(0, 0x1f5, 1, music_chan, music_chan, 0, 0, 0);
    play_done = 0;
    play_failed = 0;
    lseek(music_path, 0xafc8000, 0);
    ss_play(music_path, pcb);
}
