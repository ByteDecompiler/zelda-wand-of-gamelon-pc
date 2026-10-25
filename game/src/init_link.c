#include "game.h"

short zelda_frames_4[3];

long init_link(long x, long y)
{
    register struct imagery *hdr;
    long base;
    long i;

    zelda_screen_x = x - pf_x;
    zelda_screen_y = y - pf_y;
    zelda_x = x;
    zelda_y = y;
    zelda_sprite_x = zelda_screen_x - 8;
    zelda_sprite_y = zelda_screen_y - 14;
    saved_zelda_x = zelda_sprite_x;
    saved_zelda_y = zelda_sprite_y;
    sprite_ptrs[0] = (char *)1;
    sprite_drawn[0] = 1;
    zelda_flag_1df2 = 0;
    jump_rising = 0;
    jump_falling = 0;
    jump_wait_timer = 10;
    zelda_thrusting = 0;
    zelda_crouching = 0;
    crouch_wait_timer = 10;
    climbing_blk48 = 0;
    climbing_blk25 = 0;
    zelda_moved = 0;
    zelda_dir = 2;
    zelda_anim_step = 1;
    thrust_lunge = 0;
    zelda_riding = 0;
    rope_out = 0;
    jump_info.track = 0;
    hdr = (struct imagery *)zelda_imagery;
    base = zelda_imagery + 0x6c;
    for (i = 0; i < 24; ++i)
        sprite_frames[i] = base + hdr->off[i];
    hero_actions[0].id = 0;
    hero_actions[0].frames = zelda_frames_1;
    hero_actions[1].id = 2;
    hero_actions[1].frames = zelda_frames_2;
    hero_actions[2].id = 4;
    hero_actions[2].frames = zelda_frames_4;
    hero_actions[3].id = 5;
    hero_actions[3].frames = zelda_frames_4;
    hero_actions[4].id = 6;
    hero_actions[4].frames = zelda_frames_6;
    hero_actions[5].id = 7;
    hero_actions[5].frames = zelda_frames_7;
    hero_actions[6].id = 8;
    hero_actions[6].frames = zelda_frames_8;
    hero_actions[7].id = 9;
    hero_actions[7].frames = zelda_frames_9;
    hero_actions[8].id = 10;
    hero_actions[8].frames = zelda_frames_10;
    hero_actions[9].id = 11;
    hero_actions[9].frames = zelda_frames_11;
    hero_actions[10].id = 12;
    hero_actions[10].frames = zelda_frames_12;
    hero_actions[11].id = 13;
    hero_actions[11].frames = zelda_frames_13;
    hero_actions[12].id = 14;
    hero_actions[12].frames = zelda_frames_14;
    hero_actions[13].id = 15;
    hero_actions[13].frames = zelda_frames_15;
    hero_actions[14].id = 16;
    hero_actions[14].frames = zelda_frames_16;
    for (i = 0; i < 15; ++i)
        if (!hero_actions[i].id)
            break;
    cur_sprite = sprite_frames[*hero_actions[i].frames];
    return i;
}
