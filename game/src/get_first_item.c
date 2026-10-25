#include "game.h"

char tool_list[27];

long get_first_item(long refill)
{
    register char i;

    i = 99;
    if (refill)
        fill_tool_list();
    if (selected_item != 99 && item_counts[selected_item] == 0)
        selected_item = 99;
    if (selected_item == 99) {
        if (tool_count == 0)
            i = 99;
        else
            i = 0;
        if (i == 99)
            selected_item = 99;
        else
            selected_item = tool_list[i];
    } else {
        for (i = 0; tool_list[i] != selected_item; ++i)
            ;
    }
    return i;
}
