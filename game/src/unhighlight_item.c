#include "game.h"

void unhighlight_item(char item, long where)
{
    erase_item_name(item, where);
    erase_selected_icon(where);
    icon_to_from_save_buffer(item, where, 0);
}
