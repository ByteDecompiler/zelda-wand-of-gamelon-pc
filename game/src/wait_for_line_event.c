#include "game.h"

void wait_for_line_event(void)
{
    line_event = 0;
    while (!line_event)
        port_idle();
}
