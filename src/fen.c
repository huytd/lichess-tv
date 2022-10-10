#include "fen.h"
#include <ctype.h>
#include <stdlib.h>
#include "lib/debug.h"

void
fen_to_board(char* fen, char* board, turn_e* turn)
{
    memset(board, '.', sizeof(char) * BOARD_SIZE);
    char* placement = strtok(fen, " ");
    size_t p_pos    = 0;
    size_t b_pos    = 0;
    for (p_pos = 0; placement[p_pos] != '\0'; p_pos++) {
        if (placement[p_pos] != '/') {
            if (isdigit(placement[p_pos])) {
                b_pos += placement[p_pos] - '0';
            } else {
                board[b_pos++] = placement[p_pos];
            }
        }
    }
    board[b_pos]   = '\0';

    char* cur_turn = strtok(NULL, " ");
    if (cur_turn != NULL) {
        if (strcmp(cur_turn, "w") == 0) {
            *turn = WHITE_TURN;
        } else {
            *turn = BLACK_TURN;
        }
    }
}
