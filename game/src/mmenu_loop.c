#include "game.h"

void mmenu_loop(void)
{
    while (!mm_done) {
        if (!game_started && !mkt_demo && ++mm_idle_frames == 1400) {
            mkt_demo_end = 0;
            mkt_demo = 1;
            mkt_demo_on = 1;
            mkt_demo_ptr = mkt_demo_buf;
            mm_new_game = 1;
            mm_done = 1;
            break;
        }
        wait_for_line_event();
        if (play_done && !mkt_demo)
            play_overworld_music();
        check_stick(&menu_event);
        if (mkt_demo_end) {
            mkt_demo = 0;
            mkt_demo_end = 0;
            mm_idle_frames = 0;
        }
        screen_dimmer(menu_event || (pointer_buttons &= 3) != 0, 0xd2f0, disc_errors != 3, !(unsigned char)game_started, 1);
        if (pointer_buttons == 1 || pointer_buttons == 2) {
            if (!mkt_demo) {
                mm_idle_frames = 0;
                mkt_demo_on = 0;
            }
            debounce_trig();
            mmenu_choice(mm_cursor);
            continue;
        }
        switch (menu_event) {
        case 1:
        case 7:
        case 8:
            if (!mkt_demo) {
                mm_idle_frames = 0;
                mkt_demo_on = 0;
            }
            if (mm_cursor && can_save_restore(-1))
                --mm_cursor;
            break;
        case 3:
        case 4:
        case 5:
            if (!mkt_demo) {
                mm_idle_frames = 0;
                mkt_demo_on = 0;
            }
            if (mm_row_count - 1 != mm_cursor && can_save_restore(1))
                ++mm_cursor;
            break;
        case 0:
            break;
        }
        if (mm_choice == 4 && disc_errors == 3 && mm_cursor > 1)
            mm_cursor = 1;
        if (mm_cursor != mm_old_cursor) {
            mmenu_highlight_option(mm_cursor, mm_hilite_color);
            mmenu_highlight_option(mm_old_cursor, mm_text_color);
            mm_old_cursor = mm_cursor;
            wait_for_menu_option();
        }
    }
}
