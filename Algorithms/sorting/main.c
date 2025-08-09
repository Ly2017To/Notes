/*
 * main.c
 *
 *  Created on: Aug 31, 2023
 *      Author: luyuan
 */


/*
    Sorting contains the following methods to sort array in an ascending order
    1. insertion sort;
    2. bubble sort;
    3. selection sort;
    4. counting sort;
    5. merge sort;
    6. quick sort;
*/

#include <stdio.h>
#include <stdlib.h>
#include "methods.h"

int main()
{
  //iteractive sorting
  //sorted array: 0 1 3 4 4 5 6 7 8 8 9 12 21
  //int arr[] = {6,3,5,1,7,21,8,4,12,0,9,8,4};
  //int size = 13;

  //insertion sort
  //insertion_sort(arr,size);

  //bubble sort
  //bubble_sort(arr,size);

  //selection sort
  //selection_sort(arr,size);

  //counting sort: based on computation, applicable with restrictions on data
  //use another array to demonstrate the idea
  //sorted array: 0 0 2 2 3 3 3 5
  //int arr_cs[] = {2,5,3,0,2,3,0,3};
  //int size_cs = 8;
  //counting_sort(arr_cs, size_cs);

  //recursive sorting: based on divide and conquer
  int arr_dc[] = {12,6,4,5,9,2,3,1};
  int size_dc = 8;

  //merge sort
  //merge_sort(arr_dc,0,size_dc);
  //print_arr(arr_dc,size_dc);

  //quick sort
  quick_sort(arr_dc,0,size_dc);
  print_arr(arr_dc,size_dc);

  return SUCCESS;
}
