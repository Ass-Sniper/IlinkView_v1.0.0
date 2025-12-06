#pragma once
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*task_fn)(void*);

typedef struct {
    pthread_t *threads;
    int num_threads;
    // 可扩展：任务队列
} worker_pool_t;

int worker_pool_init(worker_pool_t *pool, int num_threads);
int worker_pool_submit(task_fn fn, void* arg);
int worker_pool_shutdown(worker_pool_t *pool);

#ifdef __cplusplus
}
#endif
