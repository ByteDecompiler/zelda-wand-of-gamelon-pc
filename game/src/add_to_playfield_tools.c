#include "game.h"

struct tool tools[20];

long add_to_playfield_tools(unsigned char kind, char sub, unsigned short x, unsigned short y, char tool_kind, short timer)
{
    long i;

    for (i = 0; i < 20; ++i)
        if (tools[i].state == 0xff && !tools[i].mode) {
            tools[i].state = sub + kind * 100;
            tools[i].x = x;
            tools[i].y = y;
            tools[i].screen_x = x - pf_x;
            tools[i].screen_y = y - pf_y;
            tools[i].draw_x = tools[i].screen_x;
            tools[i].draw_y = tools[i].screen_y;
            tools[i].kind = tool_kind;
            tools[i].timer = timer;
            tools[i].active = 0;
            tools[i].drawn = 0;
            tools[i].mode = 0;
            return 1;
        }
    return 0;
}
