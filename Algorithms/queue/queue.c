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
	p->head=0;
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
	}

	return;
}

int dequeue(queue_t * queue)
{
	if(!isEmpty(queue)){
		int value = queue->buff[queue->head];
		queue->head = (queue->head+1)%queue->size;
		return value;
	}
	return -1;
}

int isFull(queue_t * queue)
{
	return ((queue->tail+1)%queue->size)==queue->head;
}

int isEmpty(queue_t * queue)
{
	return queue->tail==queue->head;
}

void free_queue(queue_t * queue)
{
	free(queue->buff);
	free(queue);

	return;
}
