#include "game.h"

void bring_up_locked_sign(long y)
{
    draw_outlined_box(plane_a_mem + y, LOCKED_BOX_X, 100, LOCKED_BOX_X_END, 0x8a, 1, 3);
    font_draw(LOCKED_TEXT_X, 0x68, locked_text, plane_a_mem + y);
    locked_sign_done = 0;
}
