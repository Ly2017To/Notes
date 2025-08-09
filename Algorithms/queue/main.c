/*
 * main.c
 *
 *  Created on: Sep 8, 2023
 *      Author: luyuan
 */


/*
    Queue(FIFO) contains the following operations
    1. initialize a circular buffer to represent this queue of finite size;
    2. check whether a queue is empty or not;
    3. check whether a queue is full or not;
    4. enqueue when not full;
    5. dequeue when not empty;
    6. free the heap memory of a queue;
 */

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int main()
{
	int buff_size = 4;
	queue_t * circular_buff = initialize_queue(buff_size);

	int value;

	enqueue(circular_buff,1);
	enqueue(circular_buff,2);
	value = dequeue(circular_buff);
	printf("%d\n",value);
	enqueue(circular_buff,3);
	//value = dequeue(circular_buff);
	//printf("%d\n",value);

	print_queue(circular_buff);

	free_queue(circular_buff);

	return SUCCESS;
}

