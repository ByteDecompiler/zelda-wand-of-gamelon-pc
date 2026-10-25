#include "game.h"

char tool_list[27];

void draw_item_quantity(char slot, long where)
{
    char buf[4];
    long x;
    long y;

    if (slot < 5) {
        x = ICON_COUNT_X;
        y = slot * 29 + 45;
    } else
        return;
    my_sprintf(buf, 3, item_counts[tool_list[slot]]);
    font_draw(x, y, buf, where);
}
