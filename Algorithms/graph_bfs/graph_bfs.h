/*
 * graph_BFS.h
 *
 *  Created on: Sep 23, 2023
 *      Author: luyuan
 */

#ifndef GRAPH_BFS_H_
#define GRAPH_BFS_H_

#include <iostream>
#include <queue>
using namespace std;

#define SUCCESS 0
#define FAILURE 1

#define NUM_NODES 8

enum color{white,gray,black};

typedef struct node{
	char name;
	int color;
	int distance;
}node_t;

node_t initialize_node(char);
void print_nodes(node_t [], int);
void print_queue(queue<int> );
void bread_first_search_node(node_t [], char [][NUM_NODES], int, int, queue<int> &);
void bread_first_search(node_t [], char [][NUM_NODES], int);

#endif /* GRAPH_BFS_H_ */
