#ifndef FEN_H
#define FEN_H

#include <string.h>

#define BOARD_SIZE 64

char*
fen_to_board(char* fen, char* board);

#endif