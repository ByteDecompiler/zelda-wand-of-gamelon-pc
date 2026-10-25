#include "game.h"

void check_lamp_highlight(char item)
{
    if (selected_item == 2 || selected_item == 1 || selected_item == 0)
        selected_item = item;
}
