#include "game.h"

void call_subscreen(long a, long b, struct motion m, long track);
void lantern_brightness(long scrolled);

long sprite_frame_no[21];
long sprite_drawn_frame[21];

long drop_sprite(long track, long enemies, long zelda_x0, long zelda_y0, unsigned char resume, unsigned char last)
{
    long door;
    long scrolled;
    char started;
    char first;
    long i;
    long buf_a;
    long layer;
    long buf_b;

    scrolled = 0;
    started = 0;
    first = 1;
    in_combat = 0;
    gc_hide(video_path);
    init_sprites();
    hero_action = init_link(zelda_x0, zelda_y0);
    anim_frame_idx = 0;
    if (enemies)
        init_enemies();
    dc_llnk(video_path, lct_head_a, 2, lct_plane_a1, 0);
    plane_a_buffer = 0;
    buf_a = (1 - plane_a_buffer) * 0x18600 + 32;
    buf_b = plane_a_buffer * 0x18600 + 32;
    init_environment_projectiles(room_number);
    create_playfield_tools(room_number);
    if (!resume) {
        if (room_number == 45)
            dark_room = 0;
    } else
        wait_for_line_event();
    if (!resume) {
        start_boss_first_anim(room_number);
        if (game_won >= 0) {
            back_on_playfield(buf_a);
            play_animation(game_won);
            attack_damage[0] = 0;
            button_pressed = 0;
            game_won = -1;
            first = 0;
        }
        if (room_number == 45)
            dark_room = 1;
    }
    draw_tools(32);
    start_music(track);
    for (layer = 5; layer >= 1; --layer) {
        if (layer == 3)
            draw_rle_sprite(plane_a_mem + 32, 0, 0x1a0);
        else
            for (i = 0; i < 21; ++i)
                if (i && (unsigned char)macros[i]->layer == layer && sprite_ptrs[i] && check_if_on_screen(i)) {
                    port_call_sprite((&cur_sprite)[i], (&zelda_sprite_x)[i] * 4 - pf_x_px, (&zelda_sprite_y)[i] * 4 - pf_y_px,
                                                   plane_a_mem + 32, line_table);
                    (&saved_zelda_x)[i] = (&zelda_sprite_x)[i] - pf_x;
                    (&saved_zelda_y)[i] = (&zelda_sprite_y)[i] - pf_y;
                    sprite_drawn[i] = 1;
                }
    }
    draw_life_hearts(buf_a);
    draw_life_hearts(buf_b);
    if (first)
        back_on_playfield(buf_a);
    wait_for_line_event();
    zelda_idle_timer = 0;
    ((struct motion *)&menu_event)->fc = 0;
    jump_rising = 0;
    bg_cycles_on = 1;
    sprite_cycles_on = 1;
    if (bg_cycles_on) {
        for (i = 0; i < 6; ++i)
            bg_cycle_timer[i] = bg_cycle_delay[i];
        read_bg_cycle_colors();
    }
    if (sprite_cycles_on)
        read_sprite_cycle_colors2();
    zelda_target_x = zelda_screen_x;
    zelda_target_y = zelda_screen_y;
    init_combat();
    frame_flag = 0;
    frame_count = 0;
    frame_count_b = 0;
    if (frame_alarm != -1)
        alm_delete(frame_alarm);
    frame_alarm = alm_cycle(0x1f9, 0x80000010);
    button_held = 0;
    button_pressed = 0;
    glove_flash = 0;
    pt_coord(video_path, &pointer_buttons, &pointer_x, &pointer_y);
    pointer_x >>= 1;
    pointer_y >>= 1;
    old_pointer_x = pointer_x;
    old_pointer_y = pointer_y;
    pointer_buttons = 0;
    hero_killed = 0;
    anim_delay = 0;
    demo_started = 0;
    bomb_placed = 0;
    bread_timer = 0;
    for (;;) {
        while (!frame_flag)
            port_idle();
        frame_flag = 0;
        if (block_ptrs[0]->track && sprite_ptrs[0]) {
            fade_out_plane_AB();
            switch (block_ptrs[0]->track) {
            case 94:
                return 0;
            case 79:
                return 1;
            case 84:
                return 2;
            case 91:
                return 3;
            case 89:
                return 4;
            case 77:
                return 5;
            default:
                return 0;
            }
        }
        get_sprite_block(zelda_x, zelda_y, &jump_info);
        if ((jump_info.deadly || hero_killed) && sprite_ptrs[0]) {
            kill_sprite(0);
            play_sound(12);
        }
        if ((jump_info.deadly || hero_killed) && !sprite_killed[0]) {
            check_for_animations(area_deaths);
            hero_killed = 0;
            ++area_deaths;
            if (area_deaths < 3 && init_death() == 23) {
                fade_out_plane_AB();
                damage_taken = 0;
                erase_riding_paths();
                return 23;
            }
            fade_out_plane_AB();
            return 22;
        }
        if (music_paused) {
            if (play_done && voice_ready)
                start_voice_over(*voice_ptr++);
        } else if (play_done)
            restart_music();
        if (started && !demo_started)
            check_stick(&menu_event);
        screen_dimmer(menu_event || (pointer_buttons &= 3), 0x34bc, !(unsigned char)dark_room, 1, 0);
        if (pointer_buttons == 1 && sprite_ptrs[0]) {
            if (!button_held)
                button_pressed = 1;
            else
                pointer_buttons = 0;
            ++button_held;
        } else if (pointer_buttons == 2 && sprite_ptrs[0]) {
            if (!button_held)
                button_pressed = 2;
            else
                pointer_buttons = 0;
            ++button_held;
        } else if (!pointer_buttons)
            button_held = 0;
        if (pointer_buttons == 1)
            init_sword_thrust();
        else if (pointer_buttons == 2 && started && sprite_ptrs[0]) {
            get_sprite_block(zelda_x, zelda_y - 1, &jump_info);
            if (jump_info.door) {
                if (can_open_door(old_get_sprite_block(zelda_x, zelda_y - 1), room_number)) {
                    fade_out_plane_AB();
                    switch (door = old_get_sprite_block(zelda_x, zelda_y - 1)) {
                    case 94:
                        return 0;
                    case 79:
                        return 1;
                    case 84:
                        return 2;
                    case 91:
                        return 3;
                    case 89:
                        return 4;
                    case 77:
                        return 5;
                    default:
                        return 0;
                    }
                }
                pointer_buttons = 0;
                button_pressed = 0;
            } else if (zelda_crouching) {
                get_sprite_block(zelda_x, zelda_y, &jump_info);
                if (!jump_info.right && !jump_info.left && !jump_info.deadly && !jump_info_ground) {
                    menu_event = 0;
                    call_subscreen(plane_a_mem + buf_b, plane_a_mem + buf_a, *(struct motion *)&menu_event, track);
                    frame_count = 0;
                    pointer_buttons = 0;
                    pt_coord(video_path, &pointer_buttons, &pointer_x, &pointer_y);
                    pointer_x >>= 1;
                    pointer_y >>= 1;
                    old_pointer_x = pointer_x;
                    old_pointer_y = pointer_y;
                    menu_event = 0;
                    button_pressed = 0;
                    zelda_crouching = 0;
                    go_find_hero_action(0);
                    anim_frame_idx = 0;
                    cur_sprite = sprite_frames[hero_actions[hero_action].frames[anim_frame_idx]];
                }
            }
        }
        if (demo_started) {
            fade_out_plane_AB();
            return -1;
        }
        if (sprite_ptrs[0] && started)
            move_link();
        old_pf_x_px = pf_x_px;
        old_pf_y_px = pf_y_px;
        if (zelda_moved)
            zelda_idle_timer = 0;
        scrolled = check_screen_edges();
        zelda_target_x = zelda_screen_x;
        zelda_target_y = zelda_screen_y;
        if (started) {
            check_enemy_motion();
            check_enemy_combat();
        }
        buf_a = (1 - plane_a_buffer) * 0x18600 + 32;
        buf_b = plane_a_buffer * 0x18600 + 32;
        update_tools();
        draw_tools(buf_a);
        for (layer = 5; layer >= 1; --layer) {
            if (layer == 3) {
                draw_rope(buf_a);
                if ((unsigned char)sprite_killed[0] > 0) {
                    if (death_frames[(unsigned char)(sprite_killed[0] - 1)] != 999) {
                        port_call_sprite((long)proj_draw[death_frames[(unsigned char)(sprite_killed[0] - 1)]], zelda_sprite_x * 4, zelda_sprite_y * 4,
                                                                                      plane_a_mem + buf_a, line_table);
                        ++sprite_killed[0];
                    } else
                        sprite_killed[0] = 0;
                } else if (zelda_dir == 2) {
                    if (in_combat) {
                        ++combat_frames;
                        combat_frame = combat_frames;
                        draw_rle_sprite_flash(plane_a_mem + buf_a, 0, 0x1a0);
                        if (combat_frames == 64)
                            in_combat = 0;
                    } else
                        draw_rle_sprite(plane_a_mem + buf_a, 0, 0x1a0);
                } else if (in_combat) {
                    ++combat_frames;
                    combat_frame = combat_frames;
                    draw_rle_sprite_flash_flipped(plane_a_mem + buf_a, 0, 0x1a0);
                    if (combat_frames == 64)
                        in_combat = 0;
                } else
                    draw_rle_sprite_flipped(plane_a_mem + buf_a, 0, 0x1a0);
            } else
                for (i = 0; i < 21; ++i)
                    if (i && (unsigned char)macros[i]->layer == layer) {
                        if (sprite_ptrs[i] == (char *)1) {
                            if (check_if_on_screen(i))
                                port_call_sprite((&cur_sprite)[i], (&zelda_sprite_x)[i] * 4 - pf_x_px,
                                                               (&zelda_sprite_y)[i] * 4 - pf_y_px, plane_a_mem + buf_a,
                                                               line_table);
                        } else if ((unsigned char)sprite_killed[i] > 0) {
                            if (death_frames[(unsigned char)(sprite_killed[i] - 1)] != 999) {
                                if (check_if_on_screen(i))
                                    port_call_sprite((long)proj_draw[death_frames[(unsigned char)(sprite_killed[i] - 1)]],
                                        (&zelda_sprite_x)[i] * 4 - pf_x_px, (&zelda_sprite_y)[i] * 4 - pf_y_px, plane_a_mem + buf_a,
                                        line_table);
                                ++sprite_killed[i];
                            } else {
                                sprite_killed[i] = 0;
                                sprite_gone[i] = 1;
                                start_anim(room_number, i, 99);
                            }
                        }
                    }
        }
        if (bread_timer)
            --bread_timer;
        if (glove_flash) {
            if (glove_flash == 5)
                attack_damage[0] = 0;
            if (glove_flash == 1)
                power_glove_blast();
            else if (glove_flash % 2)
                power_glove_effect();
            else
                convert_palette();
            --glove_flash;
        }
        update_projectiles();
        draw_projectiles(buf_a);
        draw_life_hearts(buf_a);
        if (not_enough_rubies)
            display_rubies_sign(buf_a);
        if (got_sign_up)
            display_got_sign(buf_a);
        if (dark_room) {
            dim_lantern();
            lantern_brightness(scrolled);
        }
        if (scrolled)
            update_pf_lct();
        scrolled = 0;
        flip_A();
        line_event = 0;
        if (not_enough_rubies)
            wait_rubies_sign();
        if (got_sign_up)
            wait_got_sign();
        check_projectiles();
        check_sprite_collisions();
        check_tool_collision();
        if (button_pressed && sprite_ptrs[0] && started)
            init_weapon();
        while (!line_event)
            port_idle();
        if (anim_delay > 0)
            --anim_delay;
        if (!anim_delay && game_won >= 0) {
            stop_music();
            if (game_won == 55) {
                win_game();
                return -2;
            }
            if (play_animation(game_won) && music_paused) {
                demo_ptr = demo_buf;
                help_aborted = 1;
                demo_started = 1;
            }
            start_music(track);
            clear_projectiles();
            attack_damage[0] = 0;
            button_pressed = 0;
            game_won = -1;
            end_thrust_after_anim();
            if (heart_bonus) {
                ++max_hearts;
                heart_bonus = 0;
            }
            if (anim_restores_life) {
                if (!hero_killed)
                    restore_life();
                anim_restores_life = 0;
            }
            if (anim_flag_239c) {
                sprite_asleep[4] = 0;
                sprite_ptrs[4] = (char *)1;
                sprite_asleep[5] = 0;
                sprite_ptrs[5] = (char *)1;
                anim_flag_239c = 0;
            }
        }
        if (bg_cycles_on) {
            rotate_bg_cycle();
            dc_wrfct(video_path, fct_b, 0x7e, 16, bg_cycle);
        }
        if (sprite_cycles_on) {
            rotate_sprite_cycle();
            dc_wrfct(video_path, fct_a, 0x6e, 16, cycle_colors);
        }
        if (!locked_sign_done)
            take_out_locked_sign(buf_b);
        if (rubies_sign_wait == 1)
            rubies_sign_wait = 2;
        else if (rubies_sign_wait == 2)
            take_out_rubies_sign(buf_b);
        if (got_sign_wait == 1)
            got_sign_wait = 2;
        else if (got_sign_wait == 2)
            take_out_got_sign(buf_b);
        erase_sprite_rows(saved_zelda_x, saved_zelda_y, plane_a_mem + buf_b, 0, 0x37);
        saved_zelda_y = zelda_sprite_y;
        saved_zelda_x = zelda_sprite_x;
        for (i = 0; i < 21; ++i) {
            if (!i)
                continue;
            if (sprite_drawn[i] && check_if_was_on_screen(i)) {
                if ((unsigned char)sprite_killed[i] > 0 || sprite_gone[i])
                    erase_sprite_rows((&saved_zelda_x)[i], (&saved_zelda_y)[i], plane_a_mem + buf_b, 0, 0x37);
                else
                    erase_sprite_rows((&saved_zelda_x)[i], (&saved_zelda_y)[i], plane_a_mem + buf_b,
                            ((unsigned char *)frame_color_hi)[i * 24 + sprite_drawn_frame[i]],
                            ((unsigned char *)frame_color_lo)[i * 24 + sprite_drawn_frame[i]]);
            }
            if (!sprite_ptrs[i] && !sprite_killed[i])
                sprite_drawn[i] = 0;
            else {
                (&saved_zelda_x)[i] = (&zelda_sprite_x)[i] - pf_x;
                (&saved_zelda_y)[i] = (&zelda_sprite_y)[i] - pf_y;
                sprite_drawn[i] = 1;
            }
            sprite_drawn_frame[i] = sprite_frame_no[i];
        }
        erase_projectiles(buf_b);
        erase_rope(buf_b);
        erase_tools(buf_b);
        erase_life_hearts(buf_b);
        started = 1;
    }
}
