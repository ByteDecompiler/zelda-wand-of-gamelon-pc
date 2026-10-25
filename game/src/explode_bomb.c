#include "game.h"

struct tool tools[20];

void explode_bomb(unsigned char tool_idx)
{
    tools[tool_idx].state = 0xff;
    bomb_placed = 0;
    play_sound(8);
    init_bomb_hit(tools[tool_idx].x + 2, tools[tool_idx].y + 2, 0);
}
