#include "game.h"

struct tool_spot tool_spots[55];
struct tool tools[20];

void create_playfield_tools(long room)
{
    long i;

    for (i = 0; i < 20; ++i) {
        tools[i].state = 0xff;
        tools[i].x = 0;
        tools[i].y = 0;
        tools[i].screen_x = 0;
        tools[i].screen_y = 0;
        tools[i].draw_x = 0;
        tools[i].draw_y = 0;
        tools[i].kind = 0;
        tools[i].timer = 0;
        tools[i].active = 0;
        tools[i].drawn = 0;
        tools[i].mode = 0;
    }
    if (room_number == 18) {
        add_bombs_to_buy();
        add_ropes_to_buy();
        add_oil_to_buy();
    }
    for (i = 0; i < 55; ++i)
        if (tool_spots[i].room == room && !check_if_have_tool(i))
            add_to_playfield_tools(0, i, tool_spots[i].x, tool_spots[i].y, tool_spots[i].kind, 0);
}
