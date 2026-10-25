#include "game.h"

long nvr_delete(register char *name)
{
    if (nvr_check_name(name) && unlink(name) != -1)
        return 0;
    errno = 0x3006;
    return -1;
}
