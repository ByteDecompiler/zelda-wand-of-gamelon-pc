#include "game.h"

void take_out_got_sign(long y)
{
    draw_outlined_box(plane_a_mem + y, got_sign_x, 100, got_sign_w, 138, 0, 0);
    got_sign_wait = 0;
}
