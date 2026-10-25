#include "game.h"

struct tool tools[20];

void check_tool_collision(void)
{
    unsigned char *cmap;
    long i;
    long x;
    long y;
    long width;

    if (!attack_damage[0])
        return;
    cmap = (unsigned char *)cmaps[0];
    cmap += get_image_number(0) * 0x3a;
    width = cmap[0x39];
    if (!width--)
        return;
    x = (long)cmap[0x38] % 18;
    y = (long)cmap[0x38] / 18;
    if (zelda_dir == 6)
        x = 17 - (x + width);
    x += zelda_sprite_x + pf_x;
    y += zelda_sprite_y + pf_y;
    for (i = 0; i < 20; ++i)
        if (tools[i].state != 0xff && (room_number == 18 || tools[i].kind != 6 && tools[i].kind != 18)
                && y >= tools[i].y - 2 && y < tools[i].y + 6
                && x + width >= tools[i].x && x < tools[i].x + 4
                && tools[i].kind != 25 && tools[i].kind != 26 && tools[i].kind != 27) {
            sprite_hits_tool(i, 0);
            play_sound(3);
            if (button_pressed == 1)
                attack_damage[0] = 0;
            return;
        }
}
