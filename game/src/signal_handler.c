#include "game.h"

void signal_handler(long sig)
{
    switch (sig) {
    case 0x1f4:
        line_event = 1;
        dc_ssig(video_path, 0x1f4, 0);
        break;
    case 0x1f9:
        frame_flag = 1;
        ++frame_count;
        break;
    case 0x1f8:
        bg_loaded = 1;
        if (bg_pcl->ctrl & 0x80 || pcl_b->ctrl & 0x80 || pcl_c->ctrl & 0x80) {
            play_failed = 1;
            play_done = 1;
        }
        break;
    case 0x1f6:
    case 0x1f7:
        play_failed = 1;
        play_done = 1;
        break;
    case 0x1f5:
        if (pcb->stat & 0x10) {
            attenuation(255);
            voice_done = 1;
        }
        if (pcb->stat & 1)
            play_done = 1;
        if (pcb->stat & 0x8000) {
            play_failed = 1;
            play_done = 1;
        }
        break;
    default:
        ac_signal_process(sig);
    }
}
