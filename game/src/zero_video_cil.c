#include "game.h"

void zero_video_cil(void)
{
    long i;

    for (i = 0; i < 32; ++i)
        video_cil[i] = 0;
}
