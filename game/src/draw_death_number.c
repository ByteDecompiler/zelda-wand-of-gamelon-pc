#include "game.h"

void draw_death_number(long y)
{
    char buf[4];
    long color;

    my_sprintf(buf, 2, deaths / 16);
    color = deaths % 16 / 2;
    color += 1;
    color_font_draw(DEATH_NUMBER_X, 0x68, buf, plane_a_mem + y, color);
}
