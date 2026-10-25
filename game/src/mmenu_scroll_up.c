#include "game.h"

void mmenu_scroll_up(void)
{
    long step;
    long intensity;
    long y;

    step = music_volume == 0 ? 1 : 2;
    plane_A_over_B();
    for (y = 0xef; y >= 0; y -= 3) {
        if (mm_quit) {
            intensity = y * 63 / 0xef;
            set_icf(lct_plane_a1, 0, intensity);
            set_icf(lct_plane_a2, 0, intensity);
            set_icf(lct_plane_b1, 1, intensity);
            set_icf(lct_plane_b2, 1, intensity);
            sattenuation(0xff - y / step);
        } else if (mm_help || mm_new_game)
            sattenuation(0xff - y / step);
        else if (game_started)
            sattenuation(y / 2);
        scroll_position(y);
    }
    scroll_position(0);
    plane_B_over_A();
    mm_menu_shown = 0;
    if (mm_quit || mm_new_game || mm_help)
        sattenuation(0xff);
    else
        sattenuation(0);
}
