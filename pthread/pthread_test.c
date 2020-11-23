#include <stdio.h>
#include <stdlib.h>
#include "pthread_fifo.h"

//FIFO test
thr_queue_t fifo_queue;
#define Q_SIZE 30
void *producer(void *arg)
{
	for(int i = 0; i < 300; i++)
	{
		int data = random();
		queue_push(&fifo_queue, (void*)data);	
		printf("thr #%d, queue_push, data=%d, cur_size=%ld\n", (int)arg, data, queue_cur_size(&fifo_queue));
	}
	
	return (void*) 0;
}

void *consumer(void *arg)
{
	for(int i = 0; i < 600; i++)
	{
		int data = (int)queue_pop(&fifo_queue);	
		printf("thr #%d queue_pop, data=%d, cur_size=%ld\n", (int)arg, data, queue_cur_size(&fifo_queue));
	}

	return (void*) 0;
}

int main(int argc, char * argv[])
{
	pthread_t pid1, pid2, pid3;
	if(queue_create(&fifo_queue, Q_SIZE) != 0) return 0;
	
	pthread_create(&pid1, NULL, producer, (void*) 1);
	pthread_create(&pid2, NULL, producer, (void*) 3);
	pthread_create(&pid3, NULL, consumer, (void*) 5);

	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);
	pthread_join(pid3, NULL);

	queue_destroy(&fifo_queue);
}

