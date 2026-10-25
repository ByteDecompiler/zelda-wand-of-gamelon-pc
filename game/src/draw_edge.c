#include "game.h"

long menu_icon_tab[24];

void draw_edge(short icon, long x, long y, long where)
{
    draw_rle_icon(where, x, y, menu_icon_tab[icon]);
}
