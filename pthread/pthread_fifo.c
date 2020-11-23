#include <stdlib.h>
#include "pthread_fifo.h"

struct thr_queue_s {
	//FIFO QUEUE
	void ** _queue;
	size_t _cur_size;
	size_t _max_size;
	int _head;
	int _tail;

	//pthread context
	pthread_mutex_t _mutex;
	pthread_cond_t _cond_avail;
	pthread_cond_t _cond_have;
};

int queue_create(thr_queue_t * q, size_t max_size)
{
	//malloc之后要转换成对应数据类型的指针
	struct thr_queue_s * queue = (struct thr_queue_s*)malloc(sizeof(struct thr_queue_s));	
	if(queue == NULL) return -1;

	queue->_queue = (void**)malloc(max_size * sizeof(void*));
	if(queue->_queue == NULL)
	 {
		 free(queue);
		 queue = NULL;
		 return -1;
	}

	queue->_max_size = max_size;
	queue->_cur_size = 0;
	queue->_head = 0;
	queue->_tail = 0;

	pthread_mutex_init(&queue->_mutex, NULL);
	pthread_cond_init(&queue->_cond_avail, NULL);
	pthread_cond_init(&queue->_cond_have, NULL);

	*q = queue;

	return 0;
}

int queue_destroy(thr_queue_t * q)
{
	if(q == NULL|| *q == NULL) return -1;

	struct thr_queue_s * queue;
	queue = *q;

	pthread_mutex_destroy(&queue->_mutex);
	pthread_cond_destroy(&queue->_cond_avail);
	pthread_cond_destroy(&queue->_cond_have);
	free(queue->_queue);
	free(queue);

	return 0;
}

size_t queue_cur_size(thr_queue_t * q)
{

	if(q == NULL|| *q == NULL) return (size_t)0;
	
	return (*q)->_cur_size;
}

size_t queue_max_size(thr_queue_t * q)
{
	if(q == NULL|| *q == NULL) return (size_t)0;

	return (*q)->_max_size;
}

int queue_push(thr_queue_t * q, void * data)
{
	if(q == NULL|| *q == NULL) return -1;	

	struct thr_queue_s * queue;
	queue = *q;

	pthread_mutex_lock(&queue->_mutex);
	if(queue->_cur_size == queue->_max_size)
	{
		pthread_cond_wait(&queue->_cond_avail, &queue->_mutex);
	}
	
	queue->_queue[queue->_tail++] = data;
	if(queue->_tail == queue->_max_size) queue->_tail = 0;

	if(queue->_cur_size++ == 0)
	{
		pthread_cond_signal(&queue->_cond_have);
	}

	pthread_mutex_unlock(&queue->_mutex);
	return 0;
}

void* queue_pop(thr_queue_t * q)
{
	void * old_data = NULL;
	if(q == NULL||*q == NULL) return NULL;	

	struct thr_queue_s * queue;
	queue = *q;

	pthread_mutex_lock(&queue->_mutex);
	if(queue->_cur_size == 0)
	{
		pthread_cond_wait(&queue->_cond_have, &queue->_mutex);
	}
	
	old_data = queue->_queue[queue->_head++];
	if(queue->_head == queue->_max_size) queue->_head = 0;

	if(queue->_cur_size-- == queue->_max_size)
	{
		pthread_cond_signal(&queue->_cond_avail);
	}

	pthread_mutex_unlock(&queue->_mutex);
	return old_data;
}
