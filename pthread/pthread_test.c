#include <stdio.h>
#include <stdlib.h>
#include "pthread_fifo.h"

//FIFO test
thr_queue_t * fifo_queue = NULL;
#define Q_SIZE 30
void *producer(void *arg)
{
	if(fifo_queue == NULL)
	{
		printf("thr #%d uninitialized fifo queue!", (int) arg);
		return (void*) 0;
	}

	for(int i = 0; i < 300; i++)
	{
		int data = random();
		queue_push(fifo_queue, (void*)data);	
		printf("thr #%d, queue_push, data=%d, cur_size=%ld", (int)arg, data, queue_cur_size(fifo_queue));
	}
}

void *consumer(void *arg)
{
	if(fifo_queue == NULL)
	{
		printf("thr #%d uninitialized fifo queue!", (int) arg);
		return (void*) 0;
	}
	for(int i = 0; i < 300; i++)
	{
		int data = (int)queue_pop(fifo_queue);	
		printf("thr #%d queue_pop, data=%d, cur_size=%ld", (int)arg, data, queue_cur_size(fifo_queue));
	}
}

int main(int argc, char * argv[])
{
	pthread_t pid1, pid2, pid3;
	queue_create(fifo_queue, Q_SIZE);	
	
	pthread_create(&pid1, NULL, producer, (void*) 1);
	pthread_create(&pid2, NULL, producer, (void*) 3);
	pthread_create(&pid3, NULL, consumer, (void*) 5);

	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);
	pthread_join(pid3, NULL);

	queue_destroy(fifo_queue);
}

