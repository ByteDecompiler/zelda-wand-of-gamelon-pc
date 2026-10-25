#include "game.h"

char tool_list[27];

void fill_tool_list(void)
{
    long i;

    tool_count = 0;
    for (i = 0; i < 10; ++i)
        tool_list[i] = 99;
    add_to_tool_list(8);
    add_to_tool_list(7);
    add_to_tool_list(13);
    if (item_counts[2]) {
        add_to_tool_list(2);
        check_lamp_highlight(2);
    } else if (item_counts[1]) {
        add_to_tool_list(1);
        check_lamp_highlight(1);
    } else {
        add_to_tool_list(0);
        check_lamp_highlight(0);
    }
    add_to_tool_list(6);
    add_to_tool_list(3);
    add_to_tool_list(4);
    add_to_tool_list(5);
    add_to_tool_list(9);
    add_to_tool_list(10);
    if (water_used) {
        add_to_tool_list(11);
        check_canteen_highlight(11);
    } else {
        add_to_tool_list(12);
        check_canteen_highlight(12);
    }
}
