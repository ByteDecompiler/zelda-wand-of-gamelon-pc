#include "game.h"

void abort_rtrs(void)
{
    ss_abort(music_path);
    ss_abort(data_path);
    ss_abort(anim_path);
}
