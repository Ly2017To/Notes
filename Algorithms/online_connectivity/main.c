/*
 * main.c
 *
 *  Created on: Sep 3, 2023
 *      Author: luyuan
 */


/*
    online_connectivity contains the following algorithm
    1. quick union;
    2. quick find;
 */

#include <stdio.h>
#include <stdlib.h>
#include "connectivity.h"

int main()
{
	int arr_test1[10] = {0,1,2,3,4,5,6,7,8,9};
	int arr_test2[10] = {0,1,2,3,4,5,6,7,8,9};
	int size = 10;

	printf("demonstrate quick find\n");
	quick_find(arr_test1,size,3,4);
	quick_find(arr_test1,size,4,9);
	quick_find(arr_test1,size,8,0);
	quick_find(arr_test1,size,2,3);
	quick_find(arr_test1,size,5,6);
	quick_find(arr_test1,size,2,9);
	quick_find(arr_test1,size,5,9);
	quick_find(arr_test1,size,7,3);
	quick_find(arr_test1,size,4,8);
	quick_find(arr_test1,size,5,6);

	printf("demonstrate quick union\n");
	quick_union(arr_test2,size,3,4);
	quick_union(arr_test2,size,4,9);
	quick_union(arr_test2,size,8,0);
	quick_union(arr_test2,size,2,3);
	quick_union(arr_test2,size,5,6);
	quick_union(arr_test2,size,2,9);
	quick_union(arr_test2,size,5,9);
	quick_union(arr_test2,size,7,3);
	quick_union(arr_test2,size,4,8);
	quick_union(arr_test2,size,5,6);

	return SUCCESS;
}
