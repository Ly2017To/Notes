/*
 * methods.h
 *
 *  Created on: Aug 31, 2023
 *      Author: luyuan
 */


#ifndef METHODS_H_
#define METHODS_H_

#define SUCCESS 0
#define FAILURE 1

void print_arr(int [], int);
void swap(int *, int *);
void ini_arr(int [], int);
void insertion_sort(int [], int);
void bubble_sort(int [], int);
void selection_sort(int [], int);
void counting_sort(int [], int);
void merge_sort(int [], int, int);
void merge_arr(int [], int, int);
void quick_sort(int [], int, int);
int partition(int [], int, int, int);

#endif
