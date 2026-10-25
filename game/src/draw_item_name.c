#include "game.h"

struct item_name item_names[25];
char tool_list[27];

void draw_item_name(char slot, long color)
{
    if (!*item_names[tool_list[slot]].line2)
        font_draw(ITEM_NAME_X, (font_height + 1) / 2 + 24, item_names[tool_list[slot]].line1, color);
    else {
        font_draw(ITEM_NAME_X, 0x1c, item_names[tool_list[slot]].line1, color);
        font_draw(ITEM_NAME_X, font_height + 25, item_names[tool_list[slot]].line2, color);
    }
}
