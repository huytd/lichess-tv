#ifndef DEBUG_H
#define DEBUG_H

#include <curses.h>

#define debug_print(...) mvprintw(15, 0, __VA_ARGS__)

#endif