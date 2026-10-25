#include "game.h"

void check_collisions(long sprite_idx, long cmap_idx, long proj_idx, long proj_frame)
{
    long sx;
    long sy;
    long lo_bits;
    long hi_bits;
    long width;
    short mask;
    long dx;
    long dy;
    unsigned char *frame;
    char *hit_row;
    char *shield_row;

    if (!projectiles[proj_idx].damage)
        return;
    sx = (&zelda_sprite_x)[sprite_idx];
    sy = (&zelda_sprite_y)[sprite_idx];
    if (sprite_idx) {
        sx -= pf_x;
        sy -= pf_y;
    }
    frame = (unsigned char *)proj_cmaps[(short)proj_frame];
    width = frame[0x39];
    if (!width--)
        return;
    dx = (long)frame[0x38] % 18 + projectiles[proj_idx].map_x - sx;
    dy = (long)frame[0x38] / 18 + projectiles[proj_idx].map_y - sy;
    if (!sprite_idx && zelda_dir == 6)
        dx = 17 - (dx + width);
    if (dy < 0 || dy >= 14 || dx + width <= 0 || dx >= 17)
        return;
    if (dx <= 0) {
        width = width + dx - 1;
        dx = 1;
    }
    lo_bits = 16 - dx;
    hi_bits = 16 - (dx + width);
    if (hi_bits < 0)
        hi_bits = 0;
    mask = low_bits_mask[(short)lo_bits] & high_bits_mask[(short)hi_bits];
    hit_row = (char *)cmaps[(short)cmap_idx];
    hit_row += get_image_number(sprite_idx) * 0x3a;
    hit_row += dy * 2;
    shield_row = hit_row + 0x1cL;
    if (!sprite_idx && zelda_dir == 6)
        dx = 17 - (dx + width);
    dx = dx + sx + width / 2;
    dy += sy;
    if (mask & (short)port_be16(shield_row)) {
        if (projectiles[proj_idx].type == 25) {
            if (!bomb_bounced) {
                projectiles[proj_idx].vx = -projectiles[proj_idx].vx;
                bomb_bounced = 1;
            }
            return;
        }
        projectiles[proj_idx].state = 6;
        init_miss(dx, dy);
        if (!sprite_idx) {
            play_sound(13);
            if (room_number == 8 && shield_level)
                reflect_projectile(proj_idx);
        } else
            play_sound(6);
        return;
    }
    if (mask & (short)port_be16(hit_row)) {
        projectiles[proj_idx].state = 6;
        if (!sprite_idx) {
            init_hit(dx, dy);
            hero_hit(projectiles[proj_idx].damage);
            play_sound(12);
            return;
        }
        if (projectiles[proj_idx].type == 25) {
            if (check_vulnerablility(sprite_idx, projectiles[proj_idx].type))
                start_anim(room_number, sprite_idx, projectiles[proj_idx].type);
            if (game_won == -1) {
                init_bomb_hit(dx + pf_x, dy + pf_y, 1);
                play_sound(8);
            } else
                bomb_placed = 0;
            return;
        }
        if (check_vulnerablility(sprite_idx, projectiles[proj_idx].type)) {
            (&damage_taken)[sprite_idx] += projectiles[proj_idx].damage;
            start_anim(room_number, sprite_idx, projectiles[proj_idx].type);
            if (game_won == -1) {
                init_hit(dx, dy);
                play_sound(5);
            }
        } else {
            init_miss(dx, dy);
            play_sound(6);
        }
    }
}
