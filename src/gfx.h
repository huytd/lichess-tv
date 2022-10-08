#ifndef GFX_H
#define GFX_H

#include <curses.h>
#include "chunk.h"

void
gfx_init();

void
gfx_destroy();

void
gfx_draw_board(char* board);

void
gfx_draw_player_info(player_t* players);

#endif
