#include "game.h"

long play_bumper(void)
{
    register char *mod_hdr;
    register long *params;
    long status;

    if ((mod_hdr = modcload(bumper_dir, 0, 0x80)) == (char *)-1)
        return errno;
    if ((mod_hdr = modlink(bumper_module, 0)) == (char *)-1)
        return errno;
    params = (long *)(mod_hdr + *(long *)(mod_hdr + 0x30));
    params[0] = fct_a;
    params[1] = fct_b;
    params[2] = lct_plane_a1;
    params[3] = lct_plane_b1;
    params[4] = 0x1e0;
    params[5] = is_pal;
    params[6] = (long)pcb;
    params[7] = 0;
    params[8] = plane_a_buf0;
    params[9] = plane_b_buf0;
    strcpy(&params[10], bumper_name);
    if ((status = os9exec((char *)os9fork, bumper_proc, &bumper_proc, bumper_env, 0, 0, 4)) == -1) {
    }
    wait(&status);
    if ((status = munlink(mod_hdr)) == -1) {
    }
    if ((status = munlink(bumper_dir2)) == -1) {
    }
}
