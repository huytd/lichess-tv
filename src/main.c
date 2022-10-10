#include <stdio.h>
#include "gfx.h"
#include "feed.h"
#include "chunk.h"
#include "fen.h"
#include "lib/debug.h"
#include <pthread.h>

typedef struct
{
    int ready;
    char board[BOARD_SIZE + 1];
    player_t players[2];
} app_state_t;

static app_state_t app_state;

void
on_data(char* chunk, size_t len)
{
    app_state.ready = 1;
    chunk_parse(chunk, len);
    if (!chunk_is_move_description()) {
        clear();
        chunk_parse_players(app_state.players);
    }
    char* fen = (char*)chunk_get_fen();
    fen_to_board(fen, app_state.board);
    chunk_destroy();
}

void*
start_data_feed()
{
    feed_init(on_data);
    return NULL;
}

int
main()
{
    pthread_t feed_th;
    pthread_create(&feed_th, NULL, start_data_feed, NULL);

    gfx_init();
    while (true) {
        if (app_state.ready) {
            gfx_draw_player_info(app_state.players);
            gfx_draw_board(app_state.board);
        }
        refresh();
    }
    gfx_destroy();

    return 0;
}
