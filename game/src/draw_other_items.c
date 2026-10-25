#include "game.h"

long item_icons[25];

void draw_other_items(long where)
{
    long i;
    long x;
    long y;

    x = SELECTED_ICON_X;
    y = 0xac;
    for (i = 14; i <= 19; ++i)
        if (item_counts[i] > 0) {
            if (i < 17)
                draw_rle_icon(where, (i - 14) * QUEST_ITEM_STEP * 13 / 6 + x, y, item_icons[i]);
            else
                draw_rle_icon(where, (i - 17) * QUEST_ITEM_STEP * 13 / 6 + x, y + 29, item_icons[i]);
        }
}
