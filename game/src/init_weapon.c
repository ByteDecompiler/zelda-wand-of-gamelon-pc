#include "game.h"

void init_weapon(void)
{
    if (button_pressed == 1 && !rope_out && attack_damage[0]) {
        if (sword_level3) {
            straight_shot(0, 1);
            play_sound(4);
        } else if (sword_level2 && !damage_taken) {
            straight_shot(0, 1);
            play_sound(4);
        } else
            play_sound(0);
    } else if (button_pressed == 2) {
        if (selected_item == 99)
            get_first_item(1);
        switch (selected_item) {
        case 8:
            if (!rope_out && !zelda_riding && !jump_rising && !jump_falling && rope_count) {
                init_rope();
                if (rope_out)
                    play_sound(3);
            }
            break;
        case 7:
            if (!bomb_placed && !rope_out && item_counts[7] && init_sword_thrust() && use_item(7)) {
                lob_bomb();
                play_sound(9);
            }
            break;
        case 9:
            if (rubies >= 10) {
                if (init_sword_thrust()) {
                    glove_flash = 5;
                    play_sound(8);
                    use_rubies(10);
                }
            } else if (!climbing_blk48 && !climbing_blk25 && !rope_out)
                use_rubies(10);
            break;
        case 2:
            if (dark_room && lantern_pos < 32 && init_sword_thrust()) {
                light_lantern();
                play_sound(3);
            }
            break;
        case 0:
        case 1:
            if (dark_room && item_counts[22] && lantern_pos < 32 && init_sword_thrust() && use_item(22)) {
                light_lantern();
                play_sound(3);
            }
            break;
        case 11:
            if (damage_taken && init_sword_thrust()) {
                water_used = 0;
                damage_taken = 0;
                selected_item = 12;
                play_sound(3);
            }
            break;
        case 13:
            if (!projectile_on_field(6) && !bread_timer && item_counts[selected_item] && init_sword_thrust() && use_item(selected_item)) {
                lob_shot(6, 0);
                play_sound(9);
            }
            break;
        case 10:
            if (!projectile_on_field(5) && item_counts[selected_item] && init_sword_thrust()) {
                lob_shot(5, 1);
                play_sound(9);
            }
            break;
        case 6:
            if (!projectile_on_field(7) && item_counts[selected_item] && init_sword_thrust()) {
                lob_shot(7, 1);
                play_sound(9);
            }
            break;
        case 5:
            if (!in_combat) {
                if (rubies >= 20) {
                    if (init_sword_thrust()) {
                        in_combat = 1;
                        combat_frames = 0;
                        play_sound(3);
                        use_rubies(20);
                    }
                } else if (!climbing_blk48 && !climbing_blk25 && !rope_out)
                    use_rubies(20);
            }
            break;
        case 4:
            if (rubies >= 5) {
                if (!freeze_timer && init_sword_thrust()) {
                    freeze_timer = 0x50;
                    play_sound(3);
                    use_rubies(5);
                }
            } else if (!climbing_blk48 && !climbing_blk25 && !rope_out)
                use_rubies(5);
            break;
        case 3:
            if (!rope_out && !jump_rising && !jump_falling && !climbing_blk48 && !climbing_blk25) {
                get_sprite_block(zelda_x, zelda_y, &jump_info);
                if (!jump_info.right && !jump_info.left && !jump_info.deadly && !jump_info_ground && use_rubies(5)) {
                    init_big_jump();
                    play_sound(3);
                }
            }
            break;
        }
    }
    attack_damage[0] = 0;
    button_pressed = 0;
}
