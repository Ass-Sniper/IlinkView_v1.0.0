
#include "server.h"
#include "worker_pool.h"
#include "rate_limiter.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static void request_worker_task(void* arg) {
    struct mg_connection *conn = (struct mg_connection*)arg;
    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello from iLinkView v1.0.0\n");
}

static int request_handler(struct mg_connection *conn, void *cbdata) {
    const char *client_ip = mg_get_request_info(conn)->remote_addr;
    if (!rate_limiter_check(client_ip)) {
        mg_printf(conn, "HTTP/1.1 429 Too Many Requests\r\n\r\nRate limit exceeded");
        return 1;
    }
    worker_pool_submit(request_worker_task, conn);
    return 1;
}

int server_start(http_server_t *server, const char **options) {
    server->ctx = mg_start(NULL, NULL, options);
    if (!server->ctx) return -1;
    mg_set_request_handler(server->ctx, "/", request_handler, NULL);
    return 0;
}

int server_stop(http_server_t *server) {
    if (!server->ctx) return -1;
    mg_stop(server->ctx);
    return 0;
}
