#include "game.h"

void clean_disc(void)
{
    abort_rtrs();
    plane_B_over_A();
    reinit_lct_A();
    plane_a_buffer = 1;
    flip_A();
    clear_416x240(plane_a_buf1);
    display_clean_disc_sign(0x18620);
    flip_A();
    fade_up_plane_A();
    plane_A_over_B();
    start_voice_over(0x1a);
    while (!play_done && !voice_done)
        port_idle();
    attenuation(0xff);
}
