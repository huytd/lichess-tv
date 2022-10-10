#include <stdio.h>
#include "gfx.h"
#include "feed.h"
#include "chunk.h"
#include "fen.h"
#include "lib/debug.h"
#include <time.h>
#include <pthread.h>

typedef struct
{
    int ready;
    char board[BOARD_SIZE + 1];
    player_t players[2];
    int clock[2];
    turn_e current_turn;
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
    chunk_sync_clock(app_state.clock);
    char* fen = (char*)chunk_get_fen();
    fen_to_board(fen, app_state.board, &app_state.current_turn);
    chunk_destroy();
}

void*
start_data_feed()
{
    feed_init(on_data);
    return NULL;
}

void
render()
{
    gfx_draw_player_info(app_state.players);
    gfx_draw_board(app_state.board);
    gfx_draw_clock(app_state.current_turn, app_state.clock);
    refresh();
}

void
update(unsigned int* last_sync)
{
    if (app_state.clock[0] > 0 && app_state.clock[1] > 0) {
        unsigned int now      = clock();
        unsigned int duration = (double)(now - *last_sync) / CLOCKS_PER_SEC;
        if (duration >= 1) {
            if (app_state.current_turn == WHITE_TURN) {
                app_state.clock[0] -= 1;
            } else {
                app_state.clock[1] -= 1;
            }
            *last_sync = now;
        }
    }
}

int
main()
{
    pthread_t feed_th;
    pthread_create(&feed_th, NULL, start_data_feed, NULL);

    unsigned int last_sync = clock();

    gfx_init();
    while (true) {
        if (app_state.ready) {
            update(&last_sync);
            render();
        }
    }
    gfx_destroy();

    return 0;
}
