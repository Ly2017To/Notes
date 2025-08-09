/*
 * hash_table.h
 *
 *  Created on: Sep 8, 2023
 *      Author: luyuan
 */

#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#define SUCCESS 0
#define FAILURE 1

typedef struct element{
	int key;
	int value;
	int occupied;
}element_t;

typedef struct ht_linear_probing{
	int bulk_size;
	int size;
	float load_factor;
	element_t * elements;
}ht_linear_probing_t;

typedef struct node{
	int key;
	int value;
	struct node * next;
}node_t;

typedef struct node_key_value{
	int key;
	int value;
}node_key_value_t;

typedef struct ht_chaining{
	int bulk_size;
	int size;
	float load_factor;
	node_t ** nodes;
}ht_chaining_t;

typedef struct chaining_search{
	int flag;
	int key;
	int value;
}chaining_search_t;

//an example hash function
int hash_function(int, int);

//function prototypes of linear probing
void print_elements(element_t *, int);
void print_ht_linear_probing(ht_linear_probing_t *);
ht_linear_probing_t * initialize_ht_linear_probing(int);
int linear_probing(ht_linear_probing_t *, int, int, int);
void insert_element(ht_linear_probing_t *, int, int, int);
int search_key_probing(ht_linear_probing_t *, int);
void delete_element(ht_linear_probing_t *, int);
void rehash_probing(ht_linear_probing_t *, int);
void free_ht_linear_probing(ht_linear_probing_t *);

//function prototypes of chaining
node_t * pop(node_t *);
node_t * push(node_t *, node_t *);
int check_key_list(node_t *, int);
node_t * delete_node_in_list(node_t *, int);
void delete_node(ht_chaining_t *, int);
void print_list(node_t *);
void free_list(node_t *);
void print_nodes(node_t **, int);
void print_ht_chaining(ht_chaining_t *);
ht_chaining_t * initialize_ht_chaining(int);
void insert_node(ht_chaining_t *, int, int);
void initialize_chaining_search(chaining_search_t *);
chaining_search_t search_key_chaining(ht_chaining_t *, int);
void rehash_chaining(ht_chaining_t *, int);
void free_ht_chaining(ht_chaining_t *);

#endif /* HASH_TABLE_H_ */
