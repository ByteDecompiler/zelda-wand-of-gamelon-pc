#include "game.h"

void convert_palette(void)
{
    install_palette(fct_b, palette_out, (char *)palette_in);
}
