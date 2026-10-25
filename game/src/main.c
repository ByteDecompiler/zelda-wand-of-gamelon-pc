#include "game.h"

struct area_entry {
    short room;
    short entry_x;
    short entry_y;
    short scroll_x;
    short scroll_y;
};
extern struct area_entry area_entries[];
extern struct area_entry area_exits[];
extern char exit_base[];
extern long in_area;
extern long area_deaths;
extern unsigned char resume_area;
void intercept(void (*handler)());
void signal_handler();
long playfield(long room_no, long track_no, long load_enemies, long unused, long start_px, long start_py, long view_px, long view_py, unsigned char resume, unsigned char prev_room);

main()
{
    long zelda_start_x;
    long zelda_start_y;
    long view_x;
    long exit_code;
    long first;
    long view_y;
    unsigned char last_room;
    char *name;

    exit_code = 0;
    intercept(signal_handler);
    name = csd_devname(2, 1);
    audio_path = open(name, 2);
    free(name);
    sattenuation(0);
    pre_bumper_video_init();
    play_bumper();
    post_bumper_video_init();
    rtr_index = 0;
    open_rtrs();
    init_random();
    init_audio();
    play_title();
    initial_load();
    initialize_cursor();
    dc_wrfct(video_path, fct_a, 13, 64, &((long *)sprite_palette)[1]);
    dc_wrfct(video_path, fct_a, 0x4e, 64, &((long *)sprite_palette)[0x42]);
    font_ready = 0;
    load_menu_icons(menu_icons_buf);
    init_sprite_macros();
    setup_projectile_types();
    verify_save_space();
    in_area = 0;
    set_life_hearts();
    for (;;) {
        if (!in_area) {
            load_world(0);
            map_x = saved_map_x;
            map_y = saved_map_y;
            init_map();
            plane_B_over_A();
            music_track = 0x4c;
            if (!music_paused) {
                play_overworld_music();
                sattenuation(0);
            }
            area_deaths = 0;
            if (exit_code == -1 || exit_code == 22)
                back_on_overworld();
            else {
                fade_up_plane_B();
                if (!game_started)
                    do_mmenu_code();
            }
            room_number = scroll_world();
            stop_overworld_music();
            gc_hide(video_path);
#ifdef ANIMATION_TESTS
            if (room_number == 17) {
                play_background_animation(1);
                exit_code = 0;
                continue;
            }
            if (room_number == 16) {
                play_all_animations();
                exit_code = 0;
                continue;
            }
            if (room_number == 15) {
                play_animations();
                exit_code = 0;
                continue;
            }
#endif
            zelda_start_x = area_entries[(short)room_number].entry_x;
            zelda_start_y = area_entries[(short)room_number].entry_y;
            view_x = area_entries[(short)room_number].scroll_x;
            view_y = area_entries[(short)room_number].scroll_y;
            room_number = area_entries[(short)room_number].room;
            last_room = 0;
            music_track = room_number;
            first = 0;
            noshow_label();
            damage_taken = 0;
            fade_out_plane_AB();
            in_area = 1;
            area_has_exp = 1;
        } else {
            set_dark_room();
            if (!resume_area)
                load_playfield(room_number, (unsigned char)area_has_exp);
            first = 1;
            exit_code = playfield(room_number, music_track, first, 0, zelda_start_x, zelda_start_y + 1, view_x, view_y, resume_area, last_room);
            resume_area = 0;
            area_has_exp = 0;
            if (exit_code != 23)
                last_room = room_number;
            if (exit_code >= 0 && exit_code <= 5) {
                zelda_start_x = area_exits[exit_base[(short)room_number] + exit_code].entry_x;
                zelda_start_y = area_exits[exit_base[(short)room_number] + exit_code].entry_y;
                view_x = area_exits[exit_base[(short)room_number] + exit_code].scroll_x;
                view_y = area_exits[exit_base[(short)room_number] + exit_code].scroll_y;
                first = 0;
                room_number = area_exits[exit_base[(short)room_number] + exit_code].room;
                music_track = room_number;
            } else if (exit_code < 0)
                in_area = 0;
            else if (exit_code == 22)
                in_area = 0;
            else if (exit_code == 23)
                resume_area = 1;
        }
    }
}
