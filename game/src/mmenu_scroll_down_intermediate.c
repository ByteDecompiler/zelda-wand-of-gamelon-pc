#include "game.h"

void mmenu_scroll_down_intermediate(void)
{
    long y;

    for (y = font_height + 35; y < 0xf0; y += 3)
        scroll_position(y);
    scroll_position(0xef);
    menu_scrolling = 1;
}
