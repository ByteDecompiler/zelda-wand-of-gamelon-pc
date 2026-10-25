#include "game.h"

long death_loop(void)
{
    long played;

    played = 0;
    for (;;) {
        while (!frame_flag)
            port_idle();
        frame_flag = 0;
        check_stick(&menu_event);
        if (pointer_buttons == 1 || pointer_buttons == 2) {
            start_continue();
            if (!played) {
                play_sound(0);
                played = 1;
            }
        }
        if (!continuing && --deaths <= 0)
            return 22;
        if (continuing)
            update_death_pose();
        death_x = (1 - plane_a_buffer) * 0x18600 + 32;
        death_y = plane_a_buffer * 0x18600 + 32;
        draw_death_number(death_x);
        draw_rle_sprite(plane_a_mem + death_x, 0, 0x1a0);
        flip_A();
        wait_for_line_event();
        if (death_done)
            return 23;
        death_loop_bottom();
    }
}
