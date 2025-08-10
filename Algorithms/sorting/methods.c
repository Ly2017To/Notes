/*
 * methods.c
 *
 *  Created on: Aug 31, 2023
 *      Author: luyuan
 */


#include <stdio.h>
#include <stdlib.h>
#include "methods.h"

void print_arr(int arr[], int size)
{
  int i;

  for(i=0;i<size;i++){
    printf("%d ",arr[i]);
  }
  printf("\n");

  return;
}

void swap(int *a, int *b)
{
  int tmp = *a;
  *a=*b;
  *b=tmp;

  return;
}

void ini_arr(int arr[], int size)
{
  int i;

  for(i=0;i<size;i++){
    arr[i]=0;
  }

  return;
}

void insertion_sort(int arr[], int size)
{

  int i,j,k,value;

  for(i=1;i<size;i++){
    for(j=0;j<i;j++){
      if(arr[j]>arr[i]){
        value=arr[i];
	for(k=i-1;k>=j;k--){
	  arr[k+1]=arr[k];
	}
	arr[j]=value;
	break;
      }
    }
  }

  printf("insertion_sort\n");
  print_arr(arr,size);

  return;
}

void bubble_sort(int arr[], int size)
{
  int i,j,sorted;

  for(i=size-1;i>0;i--){
    sorted = 1;
    for(j=0;j<i;j++){
      if(arr[j]>arr[j+1]){
        swap(&arr[j], &arr[j+1]);
	sorted = 0;
      }   
    }
    if(sorted == 1){
      break;
    } 
  }

  printf("bubble_sort\n");
  print_arr(arr,size);
  return;
}

void selection_sort(int arr[], int size)
{
  int i,j,min_index;

  for(i=0;i<size;i++){
    min_index = i;
    for(j=i+1;j<size;j++){
      if(arr[j]<arr[min_index]){
        min_index=j;
      }
    }
    swap(&arr[i],&arr[min_index]);
   }

   printf("selection_sort\n");
   print_arr(arr,size);
   return;
}

void counting_sort(int arr[], int size)
{
  //analysis of arr
  //count the occurence of values in arr
  int arr_count[6];
  ini_arr(arr_count,6);
  //store the sorted array
  int arr_sorted[8];

  int i;

  //single occurence
  for(i=0;i<size;i++){
    arr_count[arr[i]]++;
  }
  print_arr(arr_count,6);

  //multiple occurence
  for(i=1;i<6;i++){
    arr_count[i]=arr_count[i]+arr_count[i-1];
  }
  print_arr(arr_count,6);

  for(i=0;i<size;i++){
    arr_count[arr[i]]--;
    arr_sorted[arr_count[arr[i]]]=arr[i];
  }

  printf("counting_sort\n");
  print_arr(arr_sorted,size);

  return;
}

void merge_arr(int arr[], int l, int r)
{
  int i,j,k,mid;
	
  int * buff = malloc((r-l)*sizeof(int));
  if(buff==NULL){
	  printf("buff malloc error\n");
	  return;
  }
	
  k=0;
  i=l;
  mid=l+(r-l)/2;
  j=mid;

  while(i<mid&&j<r){
    if(arr[i]<arr[j]){
      buff[k]=arr[i];
      i++;
    }else{
      buff[k]=arr[j];
      j++;
    }
    k++;
  }

  while(i<mid){
    buff[k]=arr[i];
    i++;
    k++;
  }

  while(j<r){
    buff[k]=arr[j];
    j++;
    k++;
  }

  memcpy(&arr[l],buff,(r-l)*sizeof(int));
  free(buff);

  return;
}

void merge_sort(int arr[], int l, int r)
{
  //[l,r)
  if(r-l<2){
    return;
  }
	
  int mid = l+(r-1)/2; //prevent overflow

  merge_sort(arr,l,mid); 
  merge_sort(arr,mid,r);
  merge_arr(arr,l,r);

  return;
}

int partition(int arr[], int l, int r, int pivot)
{
  int i,j;

  i=l;
  j=r-1;

  while(i<j){
    while(arr[i]<pivot) i++;
    while(arr[j]>pivot) j--;
    if(i<j) swap(&arr[i],&arr[j]);
  }

  return j;
}

void quick_sort(int arr[], int l, int r)
{
  [l,r)
  if(r-l<2){
    return;
  }

  int pivot, index_partition;

  pivot= arr[l+(r-l)/2]; //prevent overflow
  //pivot=arr[l+rand()%(r-l)];
  index_partition = partition(arr,l,r,pivot);

  quick_sort(arr,l,index_partition);
  quick_sort(arr,index_partition+1,r);

  return;
}


