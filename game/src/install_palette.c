#include "game.h"

void install_palette(long plane, char *pal, char *table)
{
    long i;

    for (i = 0; i < 64; ++i) {
        ((long *)table)[i + 1] = ((i & 0x3f) << 24) + 0x80000000
            | (pal[i * 3] & 0xff) << 16
            | (pal[i * 3 + 1] & 0xff) << 8
            | pal[i * 3 + 2] & 0xff;
        ((long *)table)[i + 0x42] = ((i & 0x3f) << 24) + 0x80000000
            | (pal[i * 3 + 0xc0] & 0xff) << 16
            | (pal[i * 3 + 0xc1] & 0xff) << 8
            | pal[i * 3 + 0xc2] & 0xff;
    }
    dc_wrfct(video_path, plane, 13, 64, &((long *)table)[1]);
    dc_wrfct(video_path, plane, 0x4e, 64, &((long *)table)[0x42]);
}
