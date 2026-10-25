#include "game.h"

char tool_list[27];

void draw_initial_subscreen(long where)
{
    char i;

    clear_416x240(where);
    draw_border(where);
    for (i = 0; i < tool_count; ++i)
        if (item_counts[tool_list[i]] > 0) {
            draw_item(i, where);
            switch (tool_list[i]) {
            case 7:
            case 8:
                draw_item_quantity(i, where);
            }
        }
    draw_an_item(where, 21, 3, item_counts[21]);
    draw_an_item(where, 20, 2, max_hearts);
    draw_an_item(where, 22, 4, item_counts[22]);
    draw_other_items(where);
    draw_sword(where);
    draw_shield(where);
}
