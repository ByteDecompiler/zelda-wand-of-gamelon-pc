#include "game.h"

void leave_item(long kind, long x, long y)
{
    long id;
    long item;
    long temporary;

    id = -1;
    switch (kind) {
    case 1:
        item = 2;
        temporary = 1;
        break;
    case 2:
        item = 3;
        temporary = 1;
        break;
    case 3:
        item = 4;
        temporary = 1;
        break;
    case 4:
        item = randomize() % 2 ? 2 : 3;
        temporary = 1;
        break;
    case 5:
        item = randomize() % 2 ? 3 : 4;
        temporary = 1;
        break;
    case 6:
        item = randomize() % 5 == 1 ? 4 : (randomize() % 2 ? 2 : 3);
        temporary = 1;
        break;
    case 7:
        id = 54;
        item = 16;
        temporary = 0;
        mark_experience(0xa3);
        break;
    case 8:
        id = 52;
        item = 14;
        temporary = 0;
        mark_experience(0xa2);
        break;
    case 9:
        id = 1;
        item = 5;
        temporary = 0;
        mark_experience(0xa4);
        break;
    case 10:
        id = 5;
        item = 5;
        temporary = 0;
        mark_experience(0xa5);
        break;
    case 11:
        id = 7;
        item = 5;
        temporary = 0;
        mark_experience(0xa6);
        break;
    case 12:
        id = 8;
        item = 5;
        temporary = 0;
        mark_experience(0xa7);
        break;
    case 13:
        id = 9;
        item = 5;
        temporary = 0;
        mark_experience(0xa8);
        break;
    case 14:
        id = 10;
        item = 5;
        temporary = 0;
        mark_experience(0xa9);
        break;
    case 15:
        id = 11;
        item = 5;
        temporary = 0;
        mark_experience(0xaa);
        break;
    case 16:
        id = 12;
        item = 5;
        temporary = 0;
        mark_experience(0xab);
        break;
    case 17:
        id = 13;
        item = 5;
        temporary = 0;
        mark_experience(0xac);
        break;
    case 18:
        id = 14;
        item = 5;
        temporary = 0;
        mark_experience(0xad);
        break;
    case 19:
        id = 15;
        item = 5;
        temporary = 0;
        mark_experience(0xae);
        break;
    case 20:
        id = 16;
        item = 5;
        temporary = 0;
        mark_experience(0xaf);
        break;
    case 21:
        id = 18;
        item = 21;
        temporary = 0;
        mark_experience(0xb1);
        break;
    case 22:
        id = 26;
        item = 21;
        temporary = 0;
        mark_experience(0xb2);
        break;
    case 23:
        id = 17;
        item = 5;
        temporary = 0;
        mark_experience(0xb0);
        break;
    default:
        item = 4;
        temporary = 1;
        break;
    }
    if (!temporary)
        tool_spots[id].room = room_number;
    add_to_playfield_tools(temporary, id == -1 ? item : id, x + 7, y + 7, item, temporary == 0 ? 0 : 0x50);
}
