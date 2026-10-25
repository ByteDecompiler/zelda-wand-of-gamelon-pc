#include "game.h"

void init_death(void)
{
    death_x = (1 - plane_a_buffer) * 0x18600 + 32;
    death_y = plane_a_buffer * 0x18600 + 32;
    zelda_anim_step = 0;
    jump_rising = 0;
    jump_falling = 0;
    zelda_crouching = 0;
    climbing_blk48 = 0;
    climbing_blk25 = 0;
    zelda_thrusting = 0;
    zelda_riding = 0;
    go_find_hero_action(0);
    anim_frame_idx = 0;
    cur_sprite = sprite_frames[hero_actions[hero_action].frames[anim_frame_idx]];
    deaths = 88;
    deaths_shown = deaths;
    clear_416x240(plane_a_mem + death_x);
    draw_initial_death_screen(death_x);
    flip_A();
    wait_for_line_event();
    death_loop_bottom();
    death_x = (1 - plane_a_buffer) * 0x18600 + 32;
    death_y = plane_a_buffer * 0x18600 + 32;
    clear_416x240(plane_a_mem + death_x);
    draw_initial_death_screen(death_x);
    continuing = 0;
    death_done = 0;
    death_loop();
}
