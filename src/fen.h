#ifndef FEN_H
#define FEN_H

#include <string.h>

#define BOARD_SIZE 64

typedef enum
{
    WHITE_TURN,
    BLACK_TURN
} turn_e;

void
fen_to_board(char* fen, char* board, turn_e* turn);

#endif
