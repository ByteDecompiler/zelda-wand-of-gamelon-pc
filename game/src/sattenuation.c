#include "game.h"

void sattenuation(long level)
{
    music_volume = level;
    if (level < 0xff)
        level /= 12;
    sc_atn(audio_path, 0xff00ff00 | level << 24 | level << 16 | level << 8 | level);
}
