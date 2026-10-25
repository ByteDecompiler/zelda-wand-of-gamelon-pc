#include "game.h"

void start_anim(room, sprite_no, kind_arg)
long room;
long sprite_no;
long kind_arg;
{
    char kind = kind_arg;
    if (kind != 99
            && ((unsigned char *)(enemy_data + (sprite_no - 1) * 0xb0 + 0x50))[0] < 2)
        return;
    if (!sprite_ptrs[sprite_no]) {
        if (room == 45 && (sprite_no == 1 || sprite_no == 2 || sprite_no == 3)
                && !sprite_ptrs[1] && !sprite_ptrs[2] && !sprite_ptrs[3]) {
            game_won = 29;
            anim_flag_239c = 1;
            return;
        }
        if (sprite_no != 1)
            return;
        if (room == 28)
            game_won = 13;
        else if (room == 39) {
            if (!check_experience(0xa0)) {
                mark_experience(0xa0);
                heart_bonus = 1;
                anim_restores_life = 1;
            }
            game_won = 22;
        } else if (room == 8) {
            game_won = 32;
            mark_experience(32);
        } else if (room == 59) {
            if (!check_experience(0xa1)) {
                mark_experience(0xa1);
                heart_bonus = 1;
                anim_restores_life = 1;
            }
            game_won = 36;
            mark_experience(33);
            mark_experience(34);
        } else if (room == 66) {
            game_won = 40;
            mark_experience(35);
        } else if (room == 74) {
            game_won = 44;
            mark_experience(23);
            mark_experience(33);
            mark_experience(34);
        } else if (room == 73)
            mark_experience(36);
        return;
    }
    if (sprite_no == 1) {
        if (room == 24)
            game_won = 45;
        else if (room == 37)
            game_won = 6;
        else if (room == 4)
            game_won = 31;
        else if (room == 50)
            game_won = 34;
        else if (room == 55)
            game_won = 35;
        else if (room == 64)
            game_won = 39;
        else if (room == 68)
            game_won = 42;
        else if (room == 18)
            game_won = 2;
        else if (room == 60) {
            game_won = 37;
            kill_sprite_noflash(1);
        } else if (room == 57) {
            game_won = 38;
            kill_sprite_noflash(1);
            mark_experience(24);
        } else if (room == 9) {
            game_won = 33;
            kill_sprite_noflash(1);
            have_water = 1;
        } else if (room == 22) {
            if (item_counts[16]) {
                game_won = 8;
                item_counts[18] = 0;
                item_counts[16] = 0;
                item_counts[5] = 1;
                kill_sprite_noflash(1);
            } else if (item_counts[18])
                game_won = 4;
            else {
                item_counts[18] = 1;
                game_won = 7;
            }
        } else if (room == 16) {
            if (item_counts[6] || item_counts[3])
                game_won = 5;
            else {
                item_counts[6] = 1;
                game_won = 11;
            }
        } else if (room == 21) {
            game_won = 26;
            item_counts[13] = 1;
        } else if (room == 32) {
            if (item_counts[19]) {
                item_counts[4] = 1;
                item_counts[19] = 0;
                game_won = 10;
                kill_sprite_noflash(1);
            } else
                game_won = 9;
        } else if (room == 29) {
            if (item_counts[17]) {
                sword_level3 = 1;
                item_counts[17] = 0;
                anim_restores_life = 1;
                game_won = 28;
            } else if (sword_level3)
                game_won = 48;
            else
                game_won = 27;
        } else if (room == 13) {
            game_won = 15;
            kill_sprite_noflash(1);
        } else if (room == 36) {
            if (item_counts[15]) {
                game_won = 17;
                item_counts[15] = 0;
                item_counts[9] = 1;
            } else if (item_counts[9])
                game_won = 47;
            else
                game_won = 16;
        } else if (room == 75)
            game_won = 55;
        else if (room == 38) {
            game_won = 20;
            kill_sprite_noflash(1);
            mark_experience(31);
        } else if (room == 40)
            game_won = 23;
        else if (room == 12) {
            if (check_experience(6))
                game_won = 46;
            else
                game_won = 14;
            get_water_of_life();
            anim_restores_life = 1;
        } else if (room == 63) {
            game_won = 41;
            item_counts[2] = 1;
            kill_sprite_noflash(1);
        } else if (room == 42) {
            if (item_counts[14]) {
                item_counts[3] = 1;
                item_counts[14] = 0;
                item_counts[6] = 0;
                game_won = 19;
                kill_sprite_noflash(1);
            } else
                game_won = 18;
        }
    } else if (room == 45 && (sprite_no == 4 || sprite_no == 5)) {
        if (!check_experience(0x9f)) {
            mark_experience(0x9f);
            heart_bonus = 1;
            get_water_of_life();
            anim_restores_life = 1;
            item_counts[16] = 1;
            game_won = 30;
        } else {
            get_water_of_life();
            anim_restores_life = 1;
            game_won = 49;
        }
    } else if (sprite_no == 2 && room == 40) {
        game_won = 24;
        item_counts[1] = 1;
        kill_sprite_noflash(1);
    }
    if (game_won != -1 && kind == 25)
        anim_delay = 10;
}
