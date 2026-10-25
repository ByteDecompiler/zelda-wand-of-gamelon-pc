#include "game.h"

void highlight_item(char item, long where)
{
    icon_to_from_save_buffer(item, where, 1);
    draw_highlighted_icon(item, where);
    draw_selected_icon(item, where);
    draw_item_name(item, where);
}
