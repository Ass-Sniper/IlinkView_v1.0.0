#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int rate_limiter_init(int max_requests_per_sec);
int rate_limiter_check(const char* client_ip);
void rate_limiter_cleanup();

#ifdef __cplusplus
}
#endif
