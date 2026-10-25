#include "game.h"

void stop_overworld_music(void)
{
    long level;

    for (level = music_volume; level < 255; level += 8) {
        wait_for_line_event();
        sattenuation(level);
    }
    sattenuation(255);
    ss_abort(music_path);
}
