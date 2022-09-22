#include <stdio.h>
#include "gfx.h"
#include "feed.h"
#include "chunk.h"
#include "fen.h"
#include "lib/debug.h"

void
on_data(char* chunk, size_t len)
{
    chunk_parse(chunk, len);
    if (!chunk_is_move_description()) {
        clear();
        player_t** players = chunk_get_players();
        gfx_draw_player_info(players);
    }
    char* fen   = (char*)chunk_get_fen();
    char* board = fen_to_board(fen);
    gfx_draw_board(board);
    refresh();
    chunk_destroy();
}

int
main()
{
    gfx_init();
    feed_init(on_data);
    gfx_destroy();
    return 0;
}
