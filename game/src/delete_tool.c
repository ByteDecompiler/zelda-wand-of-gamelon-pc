#include "game.h"

struct tool tools[20];

void delete_tool(unsigned char tool_index)
{
    tools[tool_index].state = 0xff;
    if (tools[tool_index].active)
        tools[tool_index].mode = 2;
}
