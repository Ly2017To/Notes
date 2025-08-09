/*
 * graph_dfs.h
 *
 *  Created on: Sep 20, 2023
 *      Author: luyuan
 */

#ifndef GRAPH_DFS_H_
#define GRAPH_DFS_H_

#define SUCCESS 0
#define FAILURE 1

#define NUM_NODES 6

enum color{white,gray,black};
enum edge{Tree,Backward,Forward,Cross};

typedef struct node{
	char name;
	char color;
	int discovery_time;
	int endprocessing_time;
}node_t;

void print_nodes(node_t [], int);
void print_edges(char [][NUM_NODES], int);
node_t initialize_node(char);
void label_backward_edges(node_t [], char [][NUM_NODES], int, char [][NUM_NODES]);
void label_forward_edges(node_t [], char [][NUM_NODES], int, char [][NUM_NODES]);
void label_cross_edges(char [][NUM_NODES], int, char [][NUM_NODES]);
void depth_first_search(node_t [], char [][NUM_NODES], int, char [][NUM_NODES]);
void depth_first_search_node(node_t [], char [][NUM_NODES], int, int, int *, char [][NUM_NODES]);


#endif /* GRAPH_DFS_H_ */
