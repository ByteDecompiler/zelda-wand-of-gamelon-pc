#include "game.h"

void check_for_animations(long area_death_count)
{
    long i;
    register struct enemy *enemy_rec;

    for (i = 1; i < 21; ++i)
        if ((unsigned char)sprite_killed[i] > 0 || sprite_gone[i]) {
            enemy_rec = (struct enemy *)(enemy_data + (i - 1) * 0xb0);
            if (enemy_rec->item > 0)
                leave_item(enemy_rec->item, sprite_draw_xs[i], sprite_draw_ys[i]);
            if (enemy_rec->kind == 1 || enemy_rec->kind == 2)
                mark_experience(enemy_rec->experience);
            if (!sprite_gone[i]) {
                start_anim(room_number, i, 99);
                if (game_won >= 0) {
                    stop_music();
                    play_animation(game_won);
                    game_won = -1;
                    if (area_death_count < 2)
                        start_music(music_track);
                    if (heart_bonus)
                        ++max_hearts;
                    heart_bonus = 0;
                    anim_restores_life = 0;
                    anim_flag_239c = 0;
                }
            }
        }
}
