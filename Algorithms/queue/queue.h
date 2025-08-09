/*
 * queue.h
 *
 *  Created on: Sep 8, 2023
 *      Author: luyuan
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#define SUCCESS 0
#define FAILURE 1

typedef struct queue{
    int size;
    int head; //output
    int tail; //input
	int *buff;
}queue_t;

queue_t * initialize_queue(int);
void print_queue(queue_t *);
void enqueue(queue_t *, int);
int dequeue(queue_t *);
int isFull(queue_t *);
int isEmpty(queue_t *);
void free_queue(queue_t *);


#endif /* QUEUE_H_ */
