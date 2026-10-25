#include "game.h"

void display_rubies_sign(long y)
{
    draw_outlined_box(plane_a_mem + y, 0x80, 0x55, 0x104, 0x96, 1, 3);
    font_draw(RUBIES_TEXT_X, 0x5a, rubies_text, plane_a_mem + y);
    font_draw(RUBIES_NUM_X, 0x73, rubies_num, plane_a_mem + y);
}
