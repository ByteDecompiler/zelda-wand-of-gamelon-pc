#include "game.h"

short help_save[8];

void do_mmenu_code(void)
{
    do {
        mmenu_init();
        if (mm_quit) {
            roll_credits();
            quit_game(0);
        } else if (mm_new_game) {
            play_background_animation(0);
            if (mkt_demo) {
                if (mkt_demo_end) {
                    set_mkt_demo_off();
                    continue;
                }
                do_mmenu_code();
                if (mkt_demo_end)
                    set_mkt_demo_off();
                continue;
            }
            mkt_demo_end = 0;
            play_done = 1;
            door_open = 0;
            cheats = 0;
            initial_item_quantity();
            initial_areas();
            selected_item = 99;
            set_life_hearts();
        } else if (mm_help) {
            music_paused = 1;
            demo_ptr = help_demo;
            voice_ptr = voice_buf;
            voice_wait = 0;
            help_on = 0;
            set_life_hearts();
            help_save[0] = item_counts[0];
            help_save[1] = item_counts[22];
            help_save[2] = item_counts[8];
            help_save[3] = rubies;
            help_save[4] = sword_level2;
            help_save[6] = sword_level3;
            help_save[5] = item_counts[7];
            help_save[7] = selected_item;
            selected_item = 99;
            item_counts[0] = 1;
            if (item_counts[22] < 3)
                item_counts[22] = 3;
            if (item_counts[8] < 6)
                item_counts[8] = 6;
            if (item_counts[7] < 3)
                item_counts[7] = 3;
            if (rubies < 38)
                rubies = 38;
            sword_level2 = 0;
            sword_level3 = 0;
            pointer_x = 0xc0;
            pointer_y = 0x78;
            pt_pos(video_path, pointer_x << 1, pointer_y << 1);
            old_pointer_x = pointer_x;
            old_pointer_y = pointer_y;
            if (play_animation(0x32)) {
                set_ov_help_off();
                help_aborted = 1;
                do_mmenu_code();
                continue;
            }
            mkt_demo_on = 0;
            if (delayed_wait(5)) {
                set_ov_help_off();
                help_aborted = 1;
                do_mmenu_code();
                continue;
            }
            pointer_buttons = 0;
            voice_ready = 1;
            gc_show(video_path);
            return;
        }
        if (!music_paused && !mkt_demo) {
            if (!game_started) {
                saved_map_cursor_x = 0x80;
                saved_map_cursor_y = 60;
            }
            game_started = 1;
        }
        gc_show(video_path);
    } while (mkt_demo_on);
}
