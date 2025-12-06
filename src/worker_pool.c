#include "worker_pool.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#define MAX_TASKS 1024

typedef struct {
    task_fn tasks[MAX_TASKS];
    void* args[MAX_TASKS];
    int front, rear;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int stop;
} task_queue_t;

static task_queue_t queue;

static void* worker_thread(void* arg) {
    while (1) {
        pthread_mutex_lock(&queue.mutex);
        while (queue.front == queue.rear && !queue.stop)
            pthread_cond_wait(&queue.cond, &queue.mutex);

        if (queue.stop && queue.front == queue.rear) {
            pthread_mutex_unlock(&queue.mutex);
            break;
        }

        task_fn fn = queue.tasks[queue.front];
        void* fn_arg = queue.args[queue.front];
        queue.front = (queue.front + 1) % MAX_TASKS;
        pthread_mutex_unlock(&queue.mutex);

        fn(fn_arg);
    }
    return NULL;
}

int worker_pool_init(worker_pool_t *pool, int num_threads) {
    pool->num_threads = num_threads;
    pool->threads = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);
    if (!pool->threads) return -1;

    queue.front = queue.rear = 0;
    queue.stop = 0;
    pthread_mutex_init(&queue.mutex, NULL);
    pthread_cond_init(&queue.cond, NULL);

    for (int i=0;i<num_threads;i++)
        pthread_create(&pool->threads[i], NULL, worker_thread, NULL);
    return 0;
}

int worker_pool_submit(task_fn fn, void* arg) {
    pthread_mutex_lock(&queue.mutex);
    queue.tasks[queue.rear] = fn;
    queue.args[queue.rear] = arg;
    queue.rear = (queue.rear + 1) % MAX_TASKS;
    pthread_cond_signal(&queue.cond);
    pthread_mutex_unlock(&queue.mutex);
    return 0;
}

int worker_pool_shutdown(worker_pool_t *pool) {
    pthread_mutex_lock(&queue.mutex);
    queue.stop = 1;
    pthread_cond_broadcast(&queue.cond);
    pthread_mutex_unlock(&queue.mutex);
    for (int i=0;i<pool->num_threads;i++)
        pthread_join(pool->threads[i], NULL);
    free(pool->threads);
    return 0;
}
