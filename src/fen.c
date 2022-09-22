#include "fen.h"
#include <ctype.h>
#include <stdlib.h>
#include "lib/debug.h"

#define BOARD_SIZE 64

char*
fen_to_board(char* fen)
{
    char* board = (char*)malloc(sizeof(char) * (BOARD_SIZE + 1));
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
    board[b_pos] = '\0';
    return board;
}