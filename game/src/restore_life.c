#include "game.h"

void restore_life(void)
{
    register long j;
    register long i;
    register long x;
    register long where;
    long half_hearts;
    long y;

    y = 0x20;
    where = 0x18620;
    x = 0;
    damage_taken = 0;
    half_hearts = max_hearts * 2 - damage_taken;
    for (i = 0; i < half_hearts - 1; i += 2) {
        play_sound(3);
        draw_heart(0, x, y);
        draw_heart(0, x++, where);
        for (j = 0; j < 20; ++j)
            wait_for_line_event();
    }
    if (i == half_hearts - 1 && i < max_hearts * 2 - 1) {
        i += 2;
        play_sound(3);
        draw_heart(1, x, y);
        draw_heart(1, x, where);
    }
}
