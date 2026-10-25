#include "game.h"

short help_save[8];

void set_help_off(void)
{
    music_paused = 0;
    help_on = 0;
    item_counts[0] = help_save[0];
    item_counts[22] = help_save[1];
    item_counts[8] = help_save[2];
    rubies = help_save[3];
    sword_level2 = help_save[4];
    sword_level3 = help_save[6];
    item_counts[7] = help_save[5];
    selected_item = help_save[7];
    mkt_demo = 0;
}
