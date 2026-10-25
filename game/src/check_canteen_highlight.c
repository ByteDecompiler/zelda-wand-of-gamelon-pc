#include "game.h"

void check_canteen_highlight(char item)
{
    if (selected_item == 11 || selected_item == 12)
        selected_item = item;
}
