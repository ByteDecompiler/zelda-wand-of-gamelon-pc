#include "game.h"

void (*compiled_sprites[24])(long x, long w, long y, long *lines);

void load_tool_imagery(char *data)
{
    register struct imagery *hdr;
    register long base = (long)data;
    register unsigned char i;

    hdr = (struct imagery *)base;
    base += 0x9c;
    for (i = 0; i < 24; ++i)
        compiled_sprites[i] = (void (*)())(base + hdr->off[i]);
}
