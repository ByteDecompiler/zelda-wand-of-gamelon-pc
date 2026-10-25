#include "game.h"

void power_glove_effect(void)
{
    long i;
    long gray;

    for (i = 0; i < 64; ++i) {
        gray = 0xff - (palette_out[i * 3] + palette_out[i * 3 + 1] + palette_out[i * 3 + 2]) / 3;
        palette_in[i + 1] = ((i & 0x3f) << 24) + 0x80000000 | (gray & 0xff) << 16 | (gray & 0xff) << 8 | gray & 0xff;
    }
    for (i = 0; i < 64; ++i) {
        gray = 0xff - (palette_out[i * 3 + 0xc0] + palette_out[i * 3 + 0xc1] + palette_out[i * 3 + 0xc2]) / 3;
        palette_in[i + 0x42] = ((i & 0x3f) << 24) + 0x80000000 | (gray & 0xff) << 16 | (gray & 0xff) << 8 | gray & 0xff;
    }
    dc_wrfct(video_path, fct_b, 13, 64, &palette_in[1]);
    dc_wrfct(video_path, fct_b, 0x4e, 64, &palette_in[0x42]);
}
