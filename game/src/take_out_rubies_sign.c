#include "game.h"

void take_out_rubies_sign(long y)
{
    draw_outlined_box(plane_a_mem + y, 128, 85, 260, 150, 0, 0);
    rubies_sign_wait = 0;
}
