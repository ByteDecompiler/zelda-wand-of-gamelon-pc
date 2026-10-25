#include "game.h"

void set_fctA_clut7(void)
{
    dc_wrfi(video_path, fct_a, 6, 0x78000400);
    reinit_lct_A();
}
