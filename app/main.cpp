#include <iostream>
#include "server.h"
#include "snmp_manager.h"

int main() {
    http_server_t server;
    const char *options[] = {"listening_ports", "8080", NULL};
    if (server_start(&server, options) != 0) {
        std::cerr << "Failed to start server\n";
        return -1;
    }
    std::cout << "Server running on port 8080\n";

    snmp_session_t *snmp = snmp_init("user1", "authpass", "privpass");

    // 初始化 worker pool
    // 初始化 rate limiter
    // 主循环
    while (true) {
        sleep(1);
    }

    snmp_close(snmp);
    server_stop(&server);
    return 0;
}
