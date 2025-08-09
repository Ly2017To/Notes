/*
 * heap.c
 *
 *  Created on: Sep 8, 2023
 *      Author: luyuan
 */

#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

void print_arr(int * arr, int size)
{
	int i;
	for(i=0;i<size;i++){
		printf("%d ",arr[i]);
	}
	printf("\n");

	return;
}

int * ini_arr(int size)
{
	int * p = (int *)calloc(size,sizeof(int));
	if(p==NULL){
		printf("malloc error\n");
		exit(FAILURE);
	}

	return p;
}

void free_arr(int * arr)
{
	free(arr);
	return;
}

int parent(int index)
{
	return (index-1)/2;
}

int left_child(int index)
{
	return 2*index+1;
}

int right_child(int index)
{
	return 2*index+2;
}

void swap(int * arr, int a, int b)
{
	if(a==b){
		return;
	}

	int tmp = arr[a];
	arr[a]=arr[b];
	arr[b]=tmp;

	return;
}

void insert(int * arr, int key, int *num_elements)
{
	int parent_index, child_index;

	child_index=*num_elements;
	arr[child_index]=key;
	*num_elements=*num_elements+1;
	parent_index = parent(*num_elements);

	while(parent_index>0){
		swap(arr,parent_index,child_index);
		child_index=parent_index;
		parent_index=parent(child_index);
	}

	return;
}

int max_node_index(int * arr, int a, int b, int c)
{
	if(b==-1){
		if(arr[a]>arr[c]){
			return a;
		}
		return c;
	}

	if(c==-1){
		if(arr[a]>arr[b]){
			return a;
		}
		return b;
	}

	if(arr[a]>arr[b]&&arr[a]>arr[c]){
		return a;
	}

	if(arr[b]>arr[a]&&arr[b]>arr[c]){
		return b;
	}

	return c;
}

void heapify(int * arr, int index, int num_elements)
{
	int left_child_index, right_child_index, max_index;

	left_child_index=left_child(index);
	right_child_index=right_child(index);

	if((left_child_index>num_elements-1)&&(right_child_index>num_elements-1)){
		return;
	}

	if(left_child_index>num_elements-1){
		max_index=max_node_index(arr,index,-1,right_child_index);
	}else if(right_child_index>num_elements-1){
		max_index=max_node_index(arr,index,left_child_index,-1);
	}else{
		max_index=max_node_index(arr,index,left_child_index,right_child_index);
	}

	swap(arr,index,max_index);

	if(max_index==left_child_index){
		heapify(arr, left_child_index, num_elements);
	}

	if(max_index==right_child_index){
		heapify(arr, right_child_index, num_elements);
	}

	return;
}

void build_heap(int * arr, int num_elements)
{
	int parent_index = parent(num_elements);

	while(parent_index>=0){
		heapify(arr, parent_index, num_elements);
		parent_index--;
	}

	return;
}

void heap_sort(int * arr, int num_elements)
{
	build_heap(arr, num_elements);
	//print_arr(arr,num_elements);
	swap(arr,0,num_elements-1);
	//print_arr(arr,num_elements);
	num_elements--;
	while(num_elements>0){
		heapify(arr,0,num_elements);
		//print_arr(arr,num_elements);
		swap(arr,0,num_elements-1);
		//print_arr(arr,num_elements);
		num_elements--;
	}

	return;
}


