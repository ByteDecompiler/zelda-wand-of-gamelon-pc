#include "game.h"

void playfield(long room, long track, long enemies, long unused, long entry_x, long entry_y, long scroll_x, long scroll_y, unsigned char resume, unsigned char last_room)
{
    demo_playfield(room, track, enemies, unused, entry_x, entry_y, scroll_x, scroll_y, resume, last_room);
}
