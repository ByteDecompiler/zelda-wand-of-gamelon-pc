#include "game.h"

long scroll_world(void)
{
    long hold_x;
    long hold_y;
    long i;
    long map_moved;
    long view_moved;
    long speed;

    map_moved = 0;
    view_moved = 0;
    hold_x = 0;
    hold_y = 0;
    pointer_buttons = 0;
    map_scroll_dy = 0;
    map_scroll_dx = 0;
    map_cursor_x = saved_map_cursor_x;
    map_cursor_y = saved_map_cursor_y;
    pointer_x = 0xc0;
    pointer_y = 120;
    old_pointer_x = pointer_x;
    old_pointer_y = pointer_y;
    pt_pos(video_path, pointer_x << 1, pointer_y << 1);
    gc_pos(video_path, map_cursor_x << 1, map_cursor_y << 1);
    gc_show(video_path);
    bg_cycles_on = 1;
    for (i = 0; i < 6; ++i) {
        bg_cycle_start[i] = world_cycle_start[i];
        bg_cycle_len[i] = world_cycle_len[i];
        bg_cycle_delay[i] = world_cycle_delay[i];
        bg_cycle_timer[i] = world_cycle_delay[i];
    }
    read_bg_cycle_colors();
    init_map();
    cur_label = -1;
    if (!music_paused || demo_ptr != demo_buf)
        check_world_hotspots();
    for (;;) {
        if (music_paused) {
            if (play_done && voice_ready)
                start_voice_over(*voice_ptr++);
            get_help_instruction();
        } else {
            if (play_done)
                play_overworld_music();
            pt_coord(video_path, &pointer_buttons, &pointer_x, &pointer_y);
            pointer_x >>= 1;
            pointer_y >>= 1;
            pointer_buttons &= 3;
        }
        screen_dimmer(pointer_x != old_pointer_x || pointer_y != old_pointer_y || pointer_buttons, 0xd2f0, 1, 1, 1);
        if (pointer_buttons == 1 && cur_label != -1) {
            init_map();
            saved_map_cursor_x = map_cursor_x;
            saved_map_cursor_y = map_cursor_y;
            saved_map_x = map_x;
            saved_map_y = map_y;
            return (unsigned short)labels[(short)cur_label].group;
        }
        if (pointer_buttons == 2) {
            if (cur_label != -1)
                noshow_label();
            do_mmenu_code();
            cur_label = -1;
            check_world_hotspots();
        }
        speed = 2;
        if (pointer_x == old_pointer_x) {
            if (hold_x > 0)
                --hold_x;
            else
                map_scroll_dx = 0;
        } else {
            hold_x = 5;
            map_scroll_dx = pointer_x > old_pointer_x ? speed : -speed;
        }
        if (pointer_y == old_pointer_y) {
            if (hold_y > 0)
                --hold_y;
            else
                map_scroll_dy = 0;
        } else {
            hold_y = 5;
            map_scroll_dy = pointer_y > old_pointer_y ? speed : -speed;
        }
        if (map_scroll_dx > 0) {
            if (map_cursor_x >= 0x100 && map_x < 0x140) {
                map_x += map_scroll_dx;
                if (map_x > 0x140)
                    map_x = 0x140;
                map_cursor_x = 0x100;
                map_moved = 1;
            } else {
                map_cursor_x += map_scroll_dx;
                if (map_cursor_x > 0x162)
                    map_cursor_x = 0x162;
                view_moved = 1;
            }
        } else if (map_scroll_dx < 0) {
            if (map_cursor_x <= 0x80 && map_x > 0) {
                map_x += map_scroll_dx;
                if (map_x < 0)
                    map_x = 0;
                map_cursor_x = 0x80;
                map_moved = 1;
            } else {
                map_cursor_x += map_scroll_dx;
                if (map_cursor_x < 20)
                    map_cursor_x = 20;
                view_moved = 1;
            }
        }
        if (map_scroll_dy > 0) {
            if (map_cursor_y >= 0xa0 && map_y < 0xc8) {
                map_y += map_scroll_dy;
                if (map_y > 0xc8)
                    map_y = 0xc8;
                map_cursor_y = 0xa0;
                map_moved = 1;
            } else {
                map_cursor_y += map_scroll_dy;
                if (map_cursor_y > 0xe2)
                    map_cursor_y = 0xe2;
                view_moved = 1;
            }
        } else if (map_scroll_dy < 0) {
            if (map_cursor_y <= 80 && map_y > 0) {
                map_y += map_scroll_dy;
                if (map_y < 0)
                    map_y = 0;
                map_cursor_y = 80;
                map_moved = 1;
            } else {
                map_cursor_y += map_scroll_dy;
                if (map_cursor_y < 20)
                    map_cursor_y = 20;
                view_moved = 1;
            }
        }
        if (!map_moved && !view_moved || pointer_x > 0x16c || pointer_x < 20 || pointer_y > 0xdc || pointer_y < 20) {
            pointer_x = 0xc0;
            pointer_y = 120;
            pt_pos(video_path, pointer_x << 1, pointer_y << 1);
        }
        check_world_hotspots();
        map_moved = 1;
        view_moved = 1;
        if (view_moved)
            gc_pos(video_path, map_cursor_x << 1, map_cursor_y << 1);
        if (map_moved)
            move_map();
        old_pointer_x = pointer_x;
        old_pointer_y = pointer_y;
        rotate_bg_cycle();
        dc_wrfct(video_path, fct_b, 0x7e, 16, bg_cycle);
    }
}
