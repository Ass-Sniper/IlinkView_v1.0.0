#pragma once
#include "civetweb.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    struct mg_context *ctx;
} http_server_t;

int server_start(http_server_t *server, const char **options);
int server_stop(http_server_t *server);

#ifdef __cplusplus
}
#endif
