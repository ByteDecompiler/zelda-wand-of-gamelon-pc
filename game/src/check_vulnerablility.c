#include "game.h"

extern unsigned char vulnerability[];
extern long enemy_phase;

long check_vulnerablility(long sprite_idx, long weapon)
{
    if (((char *)(enemy_data + (sprite_idx - 1) * 0xb0 + 0x50))[0] == 7)
        return weapon == 0x68 || weapon == 0x19;
    switch (weapon) {
    case 0x6c:
        return vulnerability[macros[sprite_idx]->enemy_id - 1] & 8;
    case 0x74:
        return vulnerability[macros[sprite_idx]->enemy_id - 1] & 0x10;
    case 0x68:
        return vulnerability[macros[sprite_idx]->enemy_id - 1] & 4;
    case 0:
        return vulnerability[macros[sprite_idx]->enemy_id - 1] & 1;
    case 1:
        return 1;
    case 0x19:
        return vulnerability[macros[sprite_idx]->enemy_id - 1] & 2;
    case 5:
        return vulnerability[macros[sprite_idx]->enemy_id - 1] & 0x20;
    case 7:
        return vulnerability[macros[sprite_idx]->enemy_id - 1] & 0x40;
    }
    return 0;
}
