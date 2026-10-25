#include "game.h"

void plane_A_over_B(void)
{
    dc_wrfi(video_path, fct_a, 5, 0xc2000000);
}
