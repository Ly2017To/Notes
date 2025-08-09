/*
 * hash_table.c
 *
 *  Created on: Sep 8, 2023
 *      Author: luyuan
 */

#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"

int hash_function(int bulk_size, int key)
{
	return (key%bulk_size);
}

void print_elements(element_t * elements, int bulk_size)
{
	int i;

	for(i=0;i<bulk_size;i++){
		printf("index=%d occupied=%d key=%d value=%d\n",i,elements[i].occupied,elements[i].key,elements[i].value);
	}

	return;
}

void print_ht_linear_probing(ht_linear_probing_t * ht)
{
	printf("a hash table with linear probing to avoid collision\n");
	printf("bulk_size=%d\n",ht->bulk_size);
	printf("size=%d\n",ht->size);
	printf("load_factor=%f\n",ht->load_factor);
	printf("elements:\n");
	print_elements(ht->elements, ht->bulk_size);

	return;
}

ht_linear_probing_t * initialize_ht_linear_probing(int bulk_size)
{
	int i;
	ht_linear_probing_t * ht;

	ht=(ht_linear_probing_t *)malloc(sizeof(ht_linear_probing_t));
	if(ht==NULL){
		printf("malloc error\n");
		exit(FAILURE);
	}

	ht->bulk_size=bulk_size;
	ht->size=0;
	ht->load_factor=0.0;

	ht->elements=(element_t *)malloc(bulk_size * sizeof(element_t));
	if(ht->elements==NULL){
		printf("malloc error\n");
		exit(FAILURE);
	}

	for(i=0;i<bulk_size;i++){
		ht->elements[i].occupied=0;
		ht->elements[i].key=0;
		ht->elements[i].value=0;
	}

	return ht;
}

int linear_probing(ht_linear_probing_t * ht, int index, int key, int value)
{
	int i=index+1;

	while(i<ht->bulk_size){
		if(ht->elements[i].occupied==0){
			ht->elements[i].occupied=1;
			ht->elements[i].key=key;
			ht->elements[i].value=value;
			ht->size++;
			ht->load_factor=(float)ht->size/(float)ht->bulk_size;
			return SUCCESS;
		}
		i++;
	}

	return FAILURE;
}

void insert_element(ht_linear_probing_t * ht, int key, int value, int bulk_size_rehashing)
{
	int index, insert_success;

	index = hash_function(ht->bulk_size,key);

	if(ht->elements[index].occupied==0){
		ht->elements[index].occupied=1;
		ht->elements[index].key=key;
		ht->elements[index].value=value;
		ht->size++;
		ht->load_factor=(float)ht->size/(float)ht->bulk_size;
		insert_success=SUCCESS;
	}else{
		insert_success=linear_probing(ht, index, key, value);
		if(insert_success==FAILURE){
			rehash_probing(ht, bulk_size_rehashing);
			insert_element(ht, key, value, bulk_size_rehashing);
		}
	}

	return;
}

int search_key_probing(ht_linear_probing_t * ht, int key)
{
	return ht->elements[hash_function(ht->bulk_size,key)].occupied;
}

void delete_element(ht_linear_probing_t * ht, int key)
{
	int index;

	index=hash_function(ht->bulk_size,key);

	if(ht->elements[index].occupied){
		ht->elements[index].occupied=0;
		ht->elements[index].key=0;
		ht->elements[index].value=0;
		ht->size--;
		printf("key is in hash table, delete successfully\n");
	}else{
		printf("key is not in hash table, no need to delete\n");
	}

	return;
}

void rehash_probing(ht_linear_probing_t * ht, int bulk_size_rehash)
{
	int i, bulk_size_old, index;

	//copy the elements before rehash
	bulk_size_old = ht->bulk_size;
	element_t * elements_cp = (element_t *)malloc(bulk_size_old*sizeof(element_t));
	if(elements_cp==NULL){
		printf("malloc error\n");
		exit(FAILURE);
	}

	memcpy(elements_cp,ht->elements,bulk_size_old*sizeof(element_t));

	//free the elements before rehashing
	free(ht->elements);
	ht->elements=NULL;

	ht->elements=(element_t *)malloc(bulk_size_rehash*sizeof(element_t));

	if(ht->elements==NULL){
		printf("malloc error\n");
		exit(FAILURE);
	}

	for(i=0;i<bulk_size_old;i++){
		if(elements_cp[i].occupied==1){
			index = hash_function(bulk_size_rehash,elements_cp[i].key);
			ht->elements[index].occupied=1;
			ht->elements[index].key=elements_cp[i].key;
			ht->elements[index].value=elements_cp[i].value;
		}
	}

	free(elements_cp);

	ht->bulk_size=bulk_size_rehash;
	ht->load_factor=(float)ht->size/(float)bulk_size_rehash;

	return;
}

void free_ht_linear_probing(ht_linear_probing_t * ht)
{
	free(ht->elements);
	free(ht);
	return;
}

node_t * new_node(int key, int value)
{
	node_t * p = (node_t *)malloc(sizeof(node_t));

	if(p==NULL){
		printf("malloc error\n");
		exit(FAILURE);
	}

	p->value=value;
	p->key=key;
	p->next=NULL;

	return p;
}

// push a node into the head of the list;
node_t * push(node_t * pHead, node_t * p)
{
	if(pHead==NULL){
		pHead=p;
	}else{
		p->next=pHead;
		pHead=p;
	}

	return pHead;
}

// delete a node with a specific value;
node_t * delete_node_in_list(node_t * pHead, int key)
{
	node_t *current, *previous, *pTmp;

	current=pHead;
	previous=pTmp=NULL;

	if(pHead==NULL){
		return pHead;
	}

	while(current!=NULL){
		if(current->key==key){
			break;
		}
		previous=current;
		current=current->next;
	}

	if(current==pHead){
		pTmp=pHead;
		pHead=pHead->next;
		free(pTmp);
	}else{
		pTmp=current;
		previous->next=current->next;
		free(pTmp);
	}

	return pHead;
}

void delete_node(ht_chaining_t * ht, int key)
{
	int index = hash_function(ht->bulk_size,key);
	ht->nodes[index] = delete_node_in_list(ht->nodes[index], key);

	return;
}

// print the lined list
void print_list(node_t * pHead)
{
	while(pHead!=NULL){
		printf("key=%d;value=%d ",pHead->key,pHead->value);
		pHead=pHead->next;
		if(pHead!=NULL){
			printf("->");
		}
	}
	printf("\n");
	return;
}

// free the heap memory of the list;
void free_list(node_t * pHead)
{
	node_t * pTmp=NULL;

	while(pHead!=NULL){
		pTmp=pHead;
		pHead=pHead->next;
		free(pTmp);
	}

	return;
}

void print_nodes(node_t ** nodes, int bulk_size)
{
	int i;

	for(i=0;i<bulk_size;i++){
		if(nodes[i]!=NULL){
			print_list(nodes[i]);
		}else{
			printf("this bulk is empty\n");
		}
	}

	return;
}

void print_ht_chaining(ht_chaining_t * ht)
{
	printf("a hash table with chaining to avoid collision\n");
	printf("bulk size=%d\n",ht->bulk_size);
	printf("number of nodes=%d\n",ht->size);
	printf("load_factor=%f\n",ht->load_factor);
	printf("nodes:\n");
	print_nodes(ht->nodes, ht->bulk_size);
	return;
}

ht_chaining_t * initialize_ht_chaining(int bulk_size)
{
	int i;
	ht_chaining_t * ht;

	ht=(ht_chaining_t *)malloc(sizeof(ht_chaining_t));
	if(ht==NULL){
		printf("malloc error\n");
		exit(FAILURE);
	}

	ht->bulk_size=bulk_size;
	ht->size=0;
	ht->load_factor=0.0;

	ht->nodes=(node_t **)malloc(bulk_size* sizeof(node_t *));
	if(ht->nodes==NULL){
		printf("malloc error\n");
		exit(FAILURE);
	}

	for(i=0;i<bulk_size;i++){
		ht->nodes[i]=NULL;
	}

	return ht;
}

void insert_node(ht_chaining_t * ht, int key, int value)
{
	int index = hash_function(ht->bulk_size,key);

	ht->nodes[index] = push(ht->nodes[index], new_node(key, value));

	ht->size++;
	ht->load_factor = (float)ht->size/(float)ht->bulk_size;

	return;
}

void initialize_chaining_search(chaining_search_t * found)
{
	found->flag=0;
	found->key=0;
	found->value=0;

	return;
}

chaining_search_t search_key_chaining(ht_chaining_t * ht, int key)
{
	int index = hash_function(ht->bulk_size,key);
	node_t * ptmp = NULL;

	chaining_search_t found;
	initialize_chaining_search(&found);

	ptmp = ht->nodes[index];
	while(ptmp!=NULL){
		if(ptmp->key==key){
			found.flag=1;
			found.key=ptmp->key;
			found.value=ptmp->value;
			break;
		}
		ptmp=ptmp->next;
	}

	return found;
}

void rehash_chaining(ht_chaining_t * ht, int bulk_size_rehash)
{
	int i, j, bulk_size_old, size_old, index;
	node_t * ptmp = NULL;

	//copy the elements before rehash
	bulk_size_old = ht->bulk_size;
	size_old = ht->size;

	node_key_value_t * nodes_cp = (node_key_value_t *)malloc(size_old*sizeof(node_key_value_t));
	if(nodes_cp==NULL){
		printf("malloc error\n");
		exit(FAILURE);
	}

	j=0;
	for(i=0;i<bulk_size_old;i++){
		ptmp = ht->nodes[i];
		while(ptmp!=NULL){
			nodes_cp[j].key=ptmp->key;
			nodes_cp[j].value=ptmp->value;
			j++;
			ptmp=ptmp->next;
		}
	}

	//free the elements before rehashing
	for(i=0;i<bulk_size_old;i++){
		free_list(ht->nodes[i]);
		ht->nodes[i]=NULL;
	}
	free(ht->nodes);
	ht->nodes=NULL;

	ht->nodes=(node_t **)malloc(bulk_size_rehash*sizeof(node_t *));
	if(ht->nodes==NULL){
		printf("malloc error\n");
		exit(FAILURE);
	}

	for(i=0;i<bulk_size_rehash;i++){
		ht->nodes[i]=NULL;
	}

	for(i=0;i<size_old;i++){
		index = hash_function(bulk_size_rehash,nodes_cp[i].key);
		ht->nodes[index] = push(ht->nodes[index], new_node(nodes_cp[i].key, nodes_cp[i].value));
	}

	free(nodes_cp);

	ht->bulk_size=bulk_size_rehash;

	return;
}

void free_ht_chaining(ht_chaining_t * ht)
{
	int i;

	for(i=0;i<ht->bulk_size; i++){
		free_list(ht->nodes[i]);
	}
	free(ht->nodes);
	free(ht);

	return;
}
