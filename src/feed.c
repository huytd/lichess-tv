#include "feed.h"

static fetch_callback_t(callback_fn);

size_t
write_callback(char* ptr, size_t size, size_t nmemb, void* userdata)
{
    size_t realsize = size * nmemb;
    callback_fn(ptr, realsize);
    return realsize;
}

void
feed_init(fetch_callback_t(cb_ptr))
{
    callback_fn  = cb_ptr;
    CURL* handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);
    curl_easy_setopt(handle, CURLOPT_URL, LICHESS_TV_URL);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(handle, CURLOPT_PIPEWAIT, 1L);

    curl_easy_perform(handle);

    curl_easy_cleanup(handle);
    curl_global_cleanup();
}
