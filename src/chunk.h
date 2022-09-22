#ifndef CHUNK_H
#define CHUNK_H

#include <string.h>
#include "lib/json.h"

typedef struct
{
    int is_black;
    const char* name;
    const char* rating;
} player_t;

void
chunk_parse(char* chunk, size_t len);

int
chunk_is_move_description();

void
chunk_destroy();

const char*
chunk_get_fen();

player_t**
chunk_get_players();

#endif
