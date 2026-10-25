#include "game.h"

void mmenu_scroll_down(void)
{
    long y;

    scroll_position(0);
    plane_A_over_B();
    for (y = 0; y < 0xf0; y += 3) {
        if (game_started && !help_ended)
            sattenuation(y / 2);
        scroll_position(y);
    }
    scroll_position(0xef);
    help_ended = 0;
    mm_menu_shown = 1;
    menu_scrolling = 1;
    if (game_started)
        sattenuation(0x7f);
}
