#include <sys/types.h>
#include <pthread.h>

struct thr_queue_s;
typedef struct thr_queue_s * thr_queue_t; 

int queue_create(thr_queue_t * q, size_t max_size);
int queue_destroy(thr_queue_t * q);
size_t queue_cur_size(thr_queue_t * q);
size_t queue_max_size(thr_queue_t * q);
int queue_push(thr_queue_t * q, void * data);
void* queue_pop(thr_queue_t * q);
