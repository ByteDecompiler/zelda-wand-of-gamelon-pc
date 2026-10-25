#include "game.h"

struct sleeper {
    char pad0[0x2e];
    char wake;
    char pad1[3];
    char wake2;
    char pad2[2];
    unsigned char size;
    unsigned char size2;
    char pad3;
    short box[4];
    short box2[4];
} BIG_ENDIAN_DATA;
long check_combat_box(short *box, long box_dir, long sprite_no, long facing);
void wake_on_chance(long sprite_no, struct enemy_def *sleeper_rec);

void check_sleepers(void)
{
    long i;
    struct sleeper *sleeper_rec;
    unsigned char size;
    short *box;

    for (i = 0; i < 21; ++i) {
        if (!i)
            continue;
        if ((sprite_asleep[i] || sprite_ptrs[i]) && macros[i]->in_wake_box) {
            sleeper_rec = (struct sleeper *)(enemy_data + (i - 1) * 0xb0);
            if (!macros[i]->respawned) {
                size = sleeper_rec->size;
                box = (short *)((char *)sleeper_rec + 0x38);
            } else {
                size = sleeper_rec->size2;
                box = (short *)((char *)sleeper_rec + 0x40);
            }
            if (check_combat_box(box, size, i, 0))
                continue;
            macros[i]->in_wake_box = 0;
        }
        if (!sprite_asleep[i])
            continue;
        sleeper_rec = (struct sleeper *)(enemy_data + (i - 1) * 0xb0);
        if (!macros[i]->respawned) {
            if (!sleeper_rec->wake) {
                if (--macros[i]->sleep_timer == 0)
                    wake_on_chance(i, (struct enemy_def *)sleeper_rec);
            } else if (sleeper_rec->wake == 1) {
                if (check_combat_box((short *)((char *)sleeper_rec + 0x38), sleeper_rec->size, i, 0)) {
                    wake_on_chance(i, (struct enemy_def *)sleeper_rec);
                    macros[i]->in_wake_box = 1;
                }
            } else if (macros[i]->wake_pending) {
                if (macros[i]->sleep_timer)
                    --macros[i]->sleep_timer;
                if (!macros[i]->sleep_timer) {
                    wake_on_chance(i, (struct enemy_def *)sleeper_rec);
                    macros[i]->wake_pending = 0;
                    macros[i]->in_wake_box = 1;
                }
            } else if (check_combat_box((short *)((char *)sleeper_rec + 0x38), sleeper_rec->size, i, 0))
                macros[i]->wake_pending = 1;
        } else {
            if (!sleeper_rec->wake2) {
                if (--macros[i]->sleep_timer == 0)
                    wake_on_chance(i, (struct enemy_def *)sleeper_rec);
            } else if (sleeper_rec->wake2 == 1) {
                if (check_combat_box((short *)((char *)sleeper_rec + 0x40), sleeper_rec->size2, i, 0)) {
                    wake_on_chance(i, (struct enemy_def *)sleeper_rec);
                    macros[i]->in_wake_box = 1;
                }
            } else if (macros[i]->wake_pending) {
                if (macros[i]->sleep_timer)
                    --macros[i]->sleep_timer;
                if (!macros[i]->sleep_timer) {
                    wake_on_chance(i, (struct enemy_def *)sleeper_rec);
                    macros[i]->wake_pending = 0;
                    macros[i]->in_wake_box = 1;
                }
            } else if (check_combat_box((short *)((char *)sleeper_rec + 0x40), sleeper_rec->size2, i, 0))
                macros[i]->wake_pending = 1;
        }
    }
}
