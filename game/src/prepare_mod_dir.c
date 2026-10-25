#include "game.h"

long prepare_mod_dir(void)
{
    char *module_a;
    char *module_b;

    if ((module_a = modcload(mod_dir_a, 0, 0x80)) == (char *)-1)
        return errno;
    if ((module_b = modcload(mod_dir_b, 0, 0x80)) == (char *)-1)
        return errno;
    if (munlink(module_a) == -1)
        return errno;
    if (munlink(module_b) == -1)
        return errno;
}
