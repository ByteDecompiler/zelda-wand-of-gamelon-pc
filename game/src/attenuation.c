#include "game.h"

void attenuation(long level)
{
    music_volume = level;
    if (level < 255)
        level = level / 10;
    sc_atn(audio_path, (level << 24) | (level << 16) | (level << 8) | level);
}
