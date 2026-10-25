#include "game.h"

#ifdef ANIMATION_TESTS

void play_animations(void)
{
    long buttons;
    long x;
    long y;
    long count;
    long i;

    count = 22;
    for (i = 0; i < count; ++i) {
        if (play_animation(anim_sequence[i]))
            break;
        pt_coord(video_path, &buttons, &x, &y);
        if (buttons & 3)
            break;
    }
    fade_out_plane_B();
}

#endif
