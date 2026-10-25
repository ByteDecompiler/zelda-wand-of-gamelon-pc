#include "game.h"

struct tool tools[20];

void update_tools(void)
{
    unsigned char i;

    for (i = 0; i < 20; ++i)
        if (tools[i].state != 0xff) {
            if (tools[i].timer > 0)
                --tools[i].timer;
            if (tools[i].timer == 1) {
                if (tools[i].state == 0x6a)
                    explode_bomb(i);
                else if (tools[i].kind == 0x19)
                    add_bombs_to_buy();
                else if (tools[i].kind == 0x1a)
                    add_ropes_to_buy();
                else if (tools[i].kind == 0x1b)
                    add_oil_to_buy();
                delete_tool(i);
            }
        }
}
