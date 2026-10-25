#include "game.h"

struct tool tools[20];

void sprite_hits_tool(unsigned char tool_idx, char skip)
{
    char result;
    unsigned char tool_kind;
    unsigned char tool_state;

    result = 0;
    if (skip)
        return;
    tool_kind = tools[tool_idx].kind;
    if (tools[tool_idx].state < 100) {
        tool_state = tools[tool_idx].state;
        result = 1;
        result = pick_up_perm_tool(tool_state);
        if (!tool_kind) {
            damage_taken = 0;
            ++max_hearts;
        }
    } else {
        tool_state = tools[tool_idx].state - 100;
        if (room_number == 18) {
            switch (tool_kind) {
            case 6:
                result = buy_item(7, 20, 99);
                break;
            case 8:
                result = buy_item(8, 10, 99);
                break;
            case 7:
                result = buy_item(22, 5, 99);
                break;
            }
        } else {
            result = 1;
            switch (tool_kind) {
            case 2:
                more_items(21, 1, 999);
                break;
            case 3:
                more_items(21, 5, 999);
                break;
            case 4:
                more_items(21, 10, 999);
                break;
            case 7:
                more_items(22, 1, 99);
                break;
            case 0:
                damage_taken -= 2;
                if (damage_taken < 0)
                    damage_taken = 0;
                break;
            case 10:
                if (item_counts[12])
                    water_used = 1;
                damage_taken = 0;
                break;
            }
        }
    }
    if (result == 4)
        game_won = 3;
    if (result == 1 || result == 2) {
        tools[tool_idx].state = 0xff;
        tools[tool_idx].mode = 1;
    }
}
