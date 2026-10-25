#include "game.h"

void display_clean_disc_sign(long y)
{
    draw_outlined_box(plane_a_mem + y, CLEAN_BOX_X, 0x55, CLEAN_BOX_X_END, 0x96, 1, 3);
    font_draw(CLEAN_TEXT1_X, 0x5a, clean_text1, plane_a_mem + y);
    font_draw(CLEAN_TEXT2_X, 0x73, clean_text2, plane_a_mem + y);
}
