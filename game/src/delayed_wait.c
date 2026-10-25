#include "game.h"

long delayed_wait(long frames)
{
    long i;

    for (i = 0; i < frames; ++i) {
        wait_for_line_event();
        if (get_input())
            return 1;
    }
    return 0;
}
