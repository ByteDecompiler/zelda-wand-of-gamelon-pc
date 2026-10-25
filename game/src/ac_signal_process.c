#include "game.h"

void ac_signal_process(long sig)
{
    switch (sig) {
    case 0x131:
        play_done = 1;
        music_wait = 0;
        break;
    case 0x132:
        if (music_side) {
            if (sig0_flags & 0x80) {
                play_done = 1;
                music_wait = 0;
            }
            sig0_flags = 0;
            sig0_count = 0;
        } else {
            if (sig1_flags & 0x80) {
                play_done = 1;
                music_wait = 0;
            }
            sig1_flags = 0;
            sig1_count = 0;
        }
        music_side = !music_side;
        music_wait = 1;
        break;
    default:
        ac_process(sig);
    }
}
