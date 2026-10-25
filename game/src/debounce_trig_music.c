#include "game.h"

void debounce_trig_music(void)
{
    long buttons, x, y;

    for (;;) {
        pt_coord(video_path, &buttons, &x, &y);
        buttons &= 3;
        if (!buttons)
            break;
        if (play_done && !music_paused)
            restart_music();
    }
}
