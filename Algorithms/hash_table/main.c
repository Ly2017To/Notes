/*
 * main.c
 *
 *  Created on: Sep 8, 2023
 *      Author: luyuan
 */

/*
    Hash Table shows two ways of dealing with collision, chaining and linear probing.
    A simple hash function is selected to show this example.
    When load factor is greater or equal to 0.5, re-hashing is performed.

    There are also many other ways to avoid collisions and designing hash functions to make it efficient.
    In real world applications, re-hashing is performed based on load factor, strategies to avoid collision, hash function and other factors.
 */

#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"

int main()
{
	//bulk_size of hash table
	//note: the bulk_size_enlarged is the bulk_size after re-hashing
	//these two numbers need to be calculated carefully based on applications
	//bulk_size_rehash could be a function of bulk_size
	int bulk_size = 7; //7 is a prime number , just to make an example
	int bulk_size_rehash = 11; // 11 is a prime number, just to make an example

	//a hash table uses linear probing to avoid collision
	ht_linear_probing_t * ht_lp;
	ht_lp=initialize_ht_linear_probing(bulk_size);

	insert_element(ht_lp, 0, 100, bulk_size_rehash);
	insert_element(ht_lp, 3, 103, bulk_size_rehash);
	insert_element(ht_lp, 5, 105, bulk_size_rehash);
	insert_element(ht_lp, 12, 112, bulk_size_rehash);
	//print_ht_linear_probing(ht_lp);
	insert_element(ht_lp, 19, 119, bulk_size_rehash);
	print_ht_linear_probing(ht_lp);
	delete_element(ht_lp, 3);
	print_ht_linear_probing(ht_lp);

	free_ht_linear_probing(ht_lp);
	ht_lp=NULL;

	/*
	 * if an elements are inserted through a while loop, then check the load factor after inserting each element.
	 * if the load factor is above a certain threshold, then rehash.
	 */

	printf("\n");

	//a hash table uses chaining to avoid collision
	ht_chaining_t * ht_c;
	ht_c=initialize_ht_chaining(bulk_size);

	insert_node(ht_c, 0, 100);
	insert_node(ht_c, 3, 103);
	insert_node(ht_c, 5, 105);
	insert_node(ht_c, 12, 112);
	//print_ht_chaining(ht_c);

	if(ht_c->load_factor > 0.5){
		rehash_chaining(ht_c, bulk_size_rehash);
	}

	insert_node(ht_c, 19, 119);
	print_ht_chaining(ht_c);

	delete_node(ht_c, 3);
	print_ht_chaining(ht_c);

	chaining_search_t found = search_key_chaining(ht_c, 5);

	if(found.flag){
		printf("key=%d value=%d in hash table\n", found.key, found.value);
	}else{
		printf("Not in hash table\n");
	}

	free_ht_chaining(ht_c);

	return SUCCESS;
}
