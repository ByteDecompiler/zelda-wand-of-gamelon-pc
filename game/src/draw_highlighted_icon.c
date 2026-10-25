#include "game.h"

char tool_list[27];
long item_icons[25];

void draw_highlighted_icon(char slot, long where)
{
    long x;
    long y;
    long x_end;
    long y_end;

    if (slot < 5) {
        x = ICON_BOX_X;
        x_end = x + ICON_BOX_W;
        y = slot * 29 + 46;
        y_end = y + 28;
    } else {
        x = ICON_BOX_X2;
        x_end = x + ICON_BOX_W;
        y = (slot - 5) * 29 + 46;
        y_end = y + 28;
    }
    draw_outlined_box(where, x, y, x_end, y_end, 12, 24);
    if (slot < 5) {
        x = ICON_BOX_X + 3;
        y = slot * 29 + 49;
    } else {
        x = ICON_BOX_X2 + 3;
        y = (slot - 5) * 29 + 49;
    }
    draw_rle_icon(where, x, y, item_icons[tool_list[slot]]);
}
