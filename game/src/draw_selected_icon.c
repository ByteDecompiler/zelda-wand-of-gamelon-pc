#include "game.h"

char tool_list[27];
long item_icons[25];

void draw_selected_icon(char slot, long where)
{
    draw_rle_icon(where, SELECTED_ICON_X, (font_height + 1) / 2 + 28, item_icons[tool_list[slot]]);
}
