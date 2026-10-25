#include "game.h"

void start_voice_over(long voice_no)
{
    abort_rtrs();
    restore_smaps();
    attenuation(0);
    set_pcb(0, 0x1f5, 1, (&music_chan)[voice_no % 16], (&music_chan)[voice_no % 16], 0, 0, 0);
    voice_done = 0;
    play_done = 0;
    play_failed = 0;
    if (voice_no < 16)
        lseek(data_path, (voice_no + VOICE_SECTOR) * 0x800, 0);
    else
        lseek(data_path, (voice_no - 16 + VOICE_SECTOR_2) * 0x800, 0);
    ss_play(data_path, pcb);
}
