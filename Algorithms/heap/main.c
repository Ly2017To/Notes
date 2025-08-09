/*
 * main.c
 *
 *  Created on: Sep 8, 2023
 *      Author: luyuan
 */


/*
    heap(almost complete binary tree; key(parent)>key(child)) contains the following operations
    1. insert;
    2. heapify;
    3. build heap;
    4. heap sort;

 */

#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

int main()
{
	//an array to demonstrate heap operation
	int size = 20;
	int *arr = ini_arr(size);

	//insert
	int num_elements = 13;
	int arr_heap[13]={20,15,10,12,11,5,4,9,8,6,7,-1,0};
	memcpy(arr,arr_heap,num_elements*sizeof(int));
	//print_arr(arr,size);
	int key = 17;
	insert(arr,key,&num_elements);
	printf("example insert:\n");
	print_arr(arr,num_elements);

	//heapify
	num_elements = 10;
	int arr_heapify[10]={16,4,10,14,7,9,3,2,8,1};
	memset(arr,0,size*sizeof(int));
	memcpy(arr,arr_heapify,num_elements*sizeof(int));
	//print_arr(arr,size);
	int heapify_index = 1;
	heapify(arr,heapify_index,num_elements);
	printf("example heapify:\n");
	print_arr(arr,num_elements);

	//build heap
	num_elements = 10;
	int arr_build_heap[10]={4,1,3,2,16,9,10,14,8,7};
	memset(arr,0,size*sizeof(int));
	memcpy(arr,arr_build_heap,num_elements*sizeof(int));
	printf("before building heap:\n");
	print_arr(arr,num_elements);
	build_heap(arr,num_elements);
	printf("after building heap:\n");
	print_arr(arr,num_elements);

	//heap sort
	num_elements = 8;
	int arr_heap_sort[8]={5,3,2,6,4,1,9,7};
	memset(arr,0,size*sizeof(int));
	memcpy(arr,arr_heap_sort,num_elements*sizeof(int));
	printf("before heap sort:\n");
	print_arr(arr,num_elements);
	heap_sort(arr,num_elements);
	printf("after heap sort:\n");
	print_arr(arr,num_elements);

	//free allocated memory
	free_arr(arr);

	return SUCCESS;
}
