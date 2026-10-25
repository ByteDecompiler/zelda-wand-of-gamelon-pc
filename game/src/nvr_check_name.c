#include "game.h"

long nvr_check_name(char *name)
{
    register char *ext;
    long len;

    len = strlen(name);
    ext = &name[len - 4];
    if (*ext++ == 'n' && *ext++ == 'v' && *ext++ == 'r' && *ext == 'i')
        return 1;
    errno = 0x3000;
    return 0;
}
