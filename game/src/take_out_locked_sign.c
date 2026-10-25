#include "game.h"

void take_out_locked_sign(long y)
{
    draw_outlined_box(plane_a_mem + y, LOCKED_BOX_X, 100, LOCKED_BOX_X_END, 138, 0, 0);
    locked_sign_done = 1;
}
