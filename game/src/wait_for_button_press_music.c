#include "game.h"

void wait_for_button_press_music(void)
{
    long buttons;
    long x;
    long y;
    long prev_x;
    long prev_y;

    if (music_paused) {
        if (delayed_wait(200)) {
            buttons = 0;
            demo_ptr = demo_buf;
            help_aborted = 1;
            demo_started = 1;
        }
        return;
    }
    debounce_trig_music();
    pt_coord(video_path, &buttons, &x, &y);
    prev_x = x;
    prev_y = y;
    for (;;) {
        wait_for_line_event();
        pt_coord(video_path, &buttons, &x, &y);
        buttons &= 3;
        screen_dimmer(prev_x != x || prev_y != y || (buttons &= 3) != 0, 0xd2f0, dark_room ? 2 : 1, 1, 0);
        prev_x = x;
        prev_y = y;
        if (buttons)
            break;
        if (play_done)
            restart_music();
    }
    debounce_trig_music();
}
