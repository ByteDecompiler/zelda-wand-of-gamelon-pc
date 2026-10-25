#include "game.h"

struct tool tools[20];

void erase_tools(long where)
{
    unsigned char i;

    for (i = 0; i < 20; ++i) {
        if (tools[i].drawn || tools[i].mode)
            erase_16x16(tools[i].draw_x, tools[i].draw_y, plane_a_mem + where);
        if (tools[i].mode == 1)
            ++tools[i].mode;
        else if (tools[i].mode == 2) {
            tools[i].mode = 0;
            tools[i].active = 0;
            tools[i].drawn = 0;
        }
    }
}
