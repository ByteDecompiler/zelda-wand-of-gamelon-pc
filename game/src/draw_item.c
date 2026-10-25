#include "game.h"

char tool_list[27];
long item_icons[25];

void draw_item(char slot, long where)
{
    long x;
    long y;

    if (slot < 5) {
        x = ICON_BOX_X + 3;
        y = slot * 29 + 0x31;
    } else {
        x = ICON_BOX_X2 + 3;
        y = (slot - 5) * 29 + 0x31;
    }
    draw_rle_icon(where, x, y, item_icons[tool_list[slot]]);
}
