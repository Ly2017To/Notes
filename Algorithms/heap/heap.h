/*
 * heap.h
 *
 *  Created on: Sep 8, 2023
 *      Author: luyuan
 */

#ifndef HEAP_H_
#define HEAP_H_

#define SUCCESS 0
#define FAILURE 1

void print_arr(int *, int);
int * ini_arr(int);
void free_arr(int *);
int parent(int);
int left_child(int);
int right_child(int);
void swap(int *, int, int );
void insert(int *, int, int *);
int max_node_index(int *, int, int, int);
void heapify(int *, int, int);
void build_heap(int *, int);
void heap_sort(int *, int);

#endif /* HEAP_H_ */
