#include "game.h"

long item_icons[25];

void draw_an_item(long where, short icon, long slot, long count)
{
    char buf[4];

    draw_rle_icon(where, SELECTED_ICON_X, slot * 23 + 42, item_icons[icon]);
    my_sprintf(buf, 3, count);
    font_draw(SELECTED_ICON_X_END, slot * 23 + 38, buf, where);
}
