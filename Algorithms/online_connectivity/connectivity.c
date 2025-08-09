/*
 * connectivity.c
 *
 *  Created on: Sep 3, 2023
 *      Author: luyuan
 */

void print_arr(int arr[], int size)
{
	int i;

	printf("arr: ");
	for(i=0;i<size;i++){
		printf("%d ",arr[i]);
	}
	printf("\n");

	return;
}

void quick_find(int arr[], int size, int p, int q)
{
	int i,p_val;

	if(arr[p]==arr[q]){
		printf("node %d and node %d are connected\n",p,q);
		return;
	}

	p_val=arr[p];
	for(i=0;i<size;i++){
		if(arr[i]==p_val){
			arr[i]=arr[q];
		}
	}
	printf("connect node %d and node %d\n",p,q);
	print_arr(arr, size);

	return;
}

void quick_union(int arr[], int size, int p, int q)
{
	int p_node, q_node;
	p_node=p;
	q_node=q;

	while(arr[p]!=p){
		p=arr[p];
	}
	while(arr[q]!=q){
		q=arr[q];
	}

	if(arr[p]==arr[q]){
		printf("node %d and node %d are connected\n",p_node,q_node);
		return;
	}

	arr[p]=q;
	printf("connect node %d and node %d\n",p_node,q_node);
	print_arr(arr, size);



	return;
}
