#include "game.h"

void mmenu_scroll_up_intermediate(void)
{
    long y;

    for (y = 239; y >= font_height + 35; y -= 3)
        scroll_position(y);
    menu_scrolling = 0;
}
