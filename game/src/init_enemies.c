#include "game.h"

void init_enemies(void)
{
    long base;
    long id;
    register struct enemy *enemy_rec;
    register long j;
    register long k;
    long i;
    struct enemy_gfx *gfx_hdr;
    unsigned short color_word;

    enemy_rec = (struct enemy *)enemy_data;
    for (i = 1; i < 21; ++i) {
        if (enemy_rec->present) {
            if ((enemy_rec->kind == 1 || enemy_rec->kind == 2) && check_experience(enemy_rec->experience)) {
                sprite_drawn[i] = 0;
                sprite_ptrs[i] = 0;
                enemy_rec = (struct enemy *)((char *)enemy_rec + 0xb0L);
                continue;
            }
            macros[i]->kind = enemy_rec->type;
            id = enemy_rec->id;
            macros[i]->enemy_id = id;
            macros[i]->layer = enemy_rec->layer;
            macros[i]->step_delay = enemy_rec->step_delay;
            if ((unsigned char)macros[i]->step_delay < 1)
                macros[i]->step_delay = 1;
            if ((unsigned char)macros[i]->step_delay > 8)
                macros[i]->step_delay = 8;
            macros[i]->alt_step_delay = 1;
            macros[i]->sleep_timer = 0;
            macros[i]->wake_pending = 0;
            macros[i]->lives = enemy_rec->lives;
            macros[i]->respawned = 0;
            macros[i]->in_wake_box = 0;
            if (!enemy_rec->first_wake_mode && enemy_rec->first_sleep > 0 || enemy_rec->first_wake_mode == 2) {
                macros[i]->sleep_timer = enemy_rec->first_sleep;
                sprite_asleep[i] = 1;
            } else if (enemy_rec->first_wake_mode == 1)
                sprite_asleep[i] = 1;
            else {
                sprite_ptrs[i] = (char *)1;
                sprite_drawn[i] = 0;
            }
            for (k = 0; k < 4; ++k)
                if (enemy_types[k].id == id)
                    break;
            sprite_gfx[i] = k + 1;
            gfx_hdr = enemy_types[k].gfx;
            base = (long)gfx_hdr + 0x9c;
            for (j = 0; j < 24; ++j) {
                sprite_frames[i * 24 + j] = base + gfx_hdr->frames[j];
                color_word = gfx_hdr->colors[j];
                ((char *)frame_color_hi)[i * 24 + j] = color_word >> 8;
                ((char *)frame_color_lo)[i * 24 + j] = color_word & 0xff;
            }
            main_init(i, (struct macro_def *)enemy_rec);
            sprite_drawn_frame[i] = sprite_frame_no[i];
            (&saved_zelda_x)[i] = (&zelda_sprite_x)[i] - pf_x;
            (&saved_zelda_y)[i] = (&zelda_sprite_y)[i] - pf_y;
        }
        enemy_rec = (struct enemy *)((char *)enemy_rec + 0xb0L);
    }
    if (music_paused && room_number == 17) {
        sprite_drawn[5] = 0;
        sprite_asleep[5] = 0;
        sprite_ptrs[5] = 0;
        sprite_drawn[10] = 0;
        sprite_asleep[10] = 0;
        sprite_ptrs[10] = 0;
        ((char *)(enemy_data + 0x520))[0] = 1;
    }
    if (room_number == 45 && check_experience(16) && check_experience(17) && check_experience(15)) {
        sprite_asleep[4] = 0;
        sprite_ptrs[4] = (char *)1;
        sprite_asleep[5] = 0;
        sprite_ptrs[5] = (char *)1;
    }
}
