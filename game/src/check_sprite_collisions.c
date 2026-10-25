#include "game.h"

void check_sprite_collisions(void)
{
    long mask_addr;
    long mask_end;
    long high_bit;
    long low_bit;
    long sy;
    long zelda_hit_w;
    long enemy_hit_w;
    long result;
    long hit_offset;
    register long i;
    register long ex;
    register long ey;
    register long sx;
    unsigned char *zelda_cm;
    unsigned char *enemy_cm;

    if (!sprite_ptrs[0])
        return;
    zelda_cm = (unsigned char *)cmaps[0];
    zelda_cm += get_image_number(0) * 0x3a;
    zelda_hit_w = zelda_cm[0x39];
    if (!zelda_hit_w--)
        attack_damage[0] = 0;
    sx = (long)zelda_cm[0x38] % 18;
    sy = (long)zelda_cm[0x38] / 18;
    if (zelda_dir == 6)
        sx = 17 - (sx + zelda_hit_w);
    sx += zelda_sprite_x + pf_x;
    sy += zelda_sprite_y + pf_y;
    for (i = 1; i < 21; ++i) {
        if (!sprite_ptrs[i])
            continue;
        if (attack_damage[0] && sy >= (&zelda_sprite_y)[i] && (&zelda_sprite_y)[i] + 14 > sy && sx + zelda_hit_w > (&zelda_sprite_x)[i]
                && sx < (&zelda_sprite_x)[i] + 17) {
            high_bit = 15;
            if (sx > (&zelda_sprite_x)[i])
                high_bit -= sx - (&zelda_sprite_x)[i] - 1;
            low_bit = 0;
            if (sx + zelda_hit_w < (&zelda_sprite_x)[i] + 17)
                low_bit += (&zelda_sprite_x)[i] + 17 - (sx + zelda_hit_w) - 1;
            mask_addr = cmaps[(short)sprite_gfx[i]];
            mask_addr += get_image_number(i) * 0x3a;
            mask_addr += (sy - (&zelda_sprite_y)[i]) * 2;
            mask_end = mask_addr + 0x1c;
            get_collision_result(high_bit, low_bit, (unsigned short *)mask_addr, mask_end, !((&zelda_sprite_x)[i] - pf_x < zelda_sprite_x), 0, &result,
                                 &hit_offset);
            if (result == 2) {
                if (button_pressed == 1)
                    attack_damage[0] = 0;
                play_sound(2);
                init_miss((&zelda_sprite_x)[i] + 1 + hit_offset - pf_x, sy - pf_y);
            } else if (result == 1) {
                if (sword_level2 && check_vulnerablility(i, 0x74) || item_counts[23] && check_vulnerablility(i, 0x6c)) {
                    (&damage_taken)[i] += (unsigned char)attack_damage[0];
                    start_anim(room_number, i, 99);
                    if (game_won == -1) {
                        init_hit((&zelda_sprite_x)[i] + 1 + hit_offset - pf_x, sy - pf_y);
                        play_sound(1);
                    }
                } else {
                    init_miss((&zelda_sprite_x)[i] + 1 + hit_offset - pf_x, sy - pf_y);
                    play_sound(2);
                }
                if (button_pressed == 1)
                    attack_damage[0] = 0;
            }
        }
        if (attack_damage[i]) {
            enemy_cm = (unsigned char *)cmaps[(short)sprite_gfx[i]];
            enemy_cm += get_image_number(i) * 0x3a;
            enemy_hit_w = enemy_cm[0x39];
            if (!enemy_hit_w--)
                continue;
            ex = (long)enemy_cm[0x38] % 18;
            ey = (long)enemy_cm[0x38] / 18;
            ex += (&zelda_sprite_x)[i] - pf_x;
            ey += (&zelda_sprite_y)[i] - pf_y;
            if (ey >= zelda_sprite_y && ey < zelda_sprite_y + 14 && ex + enemy_hit_w > zelda_sprite_x && ex < zelda_sprite_x + 17) {
                high_bit = 15;
                if (ex > zelda_sprite_x)
                    high_bit -= ex - zelda_sprite_x - 1;
                low_bit = 0;
                if (ex + enemy_hit_w < zelda_sprite_x + 17)
                    low_bit += zelda_sprite_x + 17 - (ex + enemy_hit_w) - 1;
                mask_end = (mask_addr = (long)zelda_cm + (ey - zelda_sprite_y) * 2) + 0x1c;
                get_collision_result(high_bit, low_bit, (unsigned short *)mask_addr, mask_end, (&zelda_sprite_x)[i] - pf_x < zelda_sprite_x, zelda_dir == 6,
                                     &result, &hit_offset);
                if (result == 2) {
                    attack_damage[i] = 0;
                    init_miss(zelda_sprite_x + 1 + hit_offset, ey);
                    play_sound(11);
                } else if (result == 1) {
                    hero_hit((unsigned char)attack_damage[i]);
                    init_hit(zelda_sprite_x + 1 + hit_offset, ey);
                    attack_damage[i] = 0;
                    if (!rope_out)
                        knockback[0] = knockback[i];
                    knockback[i] = 0;
                    play_sound(10);
                    if (macros[i]->enemy_id == 70 && lantern_pos > 32)
                        lantern_pos = 32;
                }
            }
        }
    }
}
