#ifndef FEED_H
#define FEED_H

#include <curl/curl.h>

#define LICHESS_TV_URL       "https://lichess.org/api/tv/feed"
#define fetch_callback_t(fn) void (*fn)(char*, size_t)

void feed_init(fetch_callback_t(cb_ptr));

#endif
