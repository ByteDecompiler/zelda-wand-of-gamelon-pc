#include "game.h"

void animation_flip_A(void)
{
    if (anim_flip)
        dc_llnk(video_path, lct_head_a, 2, lct_plane_a1, 0);
    else
        dc_llnk(video_path, lct_head_a, 2, lct_plane_a2, 0);
    anim_flip = 1 - anim_flip;
}
