#include "rate_limiter.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

typedef struct {
    char ip[64];
    int count;
    time_t last_time;
} ip_record_t;

#define MAX_IPS 1024
static ip_record_t ip_records[MAX_IPS];
static int max_requests = 10;

int rate_limiter_init(int max_requests_per_sec) {
    max_requests = max_requests_per_sec;
    memset(ip_records, 0, sizeof(ip_records));
    return 0;
}

int rate_limiter_check(const char* client_ip) {
    time_t now = time(NULL);
    for (int i=0;i<MAX_IPS;i++) {
        if (strcmp(ip_records[i].ip, client_ip) == 0) {
            if (difftime(now, ip_records[i].last_time) >= 1.0) {
                ip_records[i].count = 1;
                ip_records[i].last_time = now;
                return 1;
            } else {
                if (ip_records[i].count >= max_requests) return 0;
                ip_records[i].count++;
                return 1;
            }
        }
    }
    // 新 IP
    for (int i=0;i<MAX_IPS;i++) {
        if (ip_records[i].ip[0] == 0) {
            strncpy(ip_records[i].ip, client_ip, 63);
            ip_records[i].count = 1;
            ip_records[i].last_time = now;
            return 1;
        }
    }
    return 0; // 达到最大记录
}

void rate_limiter_cleanup() {
    memset(ip_records, 0, sizeof(ip_records));
}
