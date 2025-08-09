/*
 * queue.c
 *
 *  Created on: Sep 8, 2023
 *      Author: luyuan
 */

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

queue_t * initialize_queue(int size)
{
	queue_t * p = (queue_t *)malloc(sizeof(queue_t));

	if(p==NULL){
		printf("malloc error\n");
		exit(FAILURE);
	}

	p->buff = (int *)calloc(size,sizeof(int));
	if(p->buff==NULL){
		printf("malloc error\n");
		exit(FAILURE);
	}

	p->size=size;
	p->head=size-1;
	p->tail=0;

	return p;
}

void print_queue(queue_t *queue)
{
	printf("size of queue = %d\n",queue->size);
	printf("head index = %d\n",queue->head);
	printf("tail index = %d\n", queue->tail);

	return;
}

void enqueue(queue_t * queue, int value)
{
	if(!isFull(queue)){
		queue->buff[queue->tail%queue->size]=value;
		queue->tail++;
		queue->tail%=queue->size;
	}

	return;
}

int dequeue(queue_t * queue)
{
	if(!isEmpty(queue)){
		queue->head++;
		queue->head%=queue->size;
		return queue->buff[queue->head%queue->size];
	}
	return -1;
}

int isFull(queue_t * queue)
{
	if(((queue->tail)%queue->size)!=(queue->head%queue->size)){
		return 0;
	}
	return 1;
}

int isEmpty(queue_t * queue)
{
	if(((queue->head+1)%queue->size)!=(queue->tail%queue->size)){
		return 0;
	}
	return 1;
}


void free_queue(queue_t * queue)
{
	free(queue->buff);
	free(queue);

	return;
}
