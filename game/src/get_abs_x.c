#include "game.h"

long get_abs_x(long which, long x, long y, long dir)
{
    switch (which) {
    case 0:
        return x;
    case 1:
        return y + 9 + x;
    case 2:
        return y + 9 + (dir == 2 ? x : -x);
    case 3:
        return y + 9 + (dir == 6 ? x : -x);
    }
}
