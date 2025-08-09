/*
 * main.c
 *
 *  Created on: Sep 20, 2023
 *      Author: luyuan
 */


/*
    graph_dfs demonstrates Depth First Search with a graph represented by matrix
    1. It visits all the nodes of a graph;
    2. When visiting a graph, it labels the nodes as white(not yet discovered), gray(discovered and not yet completed) and black(completed).
	3. It labels the edges as Tree (DFS Tree),Backward(connect a node and its ancestor),Forward(connect a node and its descendant) and Cross(remaining edges).
*/


#include <stdio.h>
#include <stdlib.h>
#include "graph_dfs.h"

int main(void)
{
	//make an example graph of 6 nodes
	//initialize the nodes;
	node_t nodes[NUM_NODES];
	nodes[0]=initialize_node('r');
	nodes[1]=initialize_node('s');
	nodes[2]=initialize_node('t');
	nodes[3]=initialize_node('v');
	nodes[4]=initialize_node('w');
	nodes[5]=initialize_node('x');
	//initialize the connection matrix
	//1:connected; 0:not connected;
	char graph_mt[NUM_NODES][NUM_NODES]={
		{0,1,0,1,0,0},
		{0,0,0,0,1,0},
		{0,0,0,0,1,1},
		{0,1,0,0,1,0},
		{0,0,0,0,0,0},
		{0,0,0,0,0,0}
	};

	//initialize the matrix of edges to be labeled as -1 (ffff)
	//each element: [Cross][Forward][Backward][Tree]
	char edges_mt[NUM_NODES][NUM_NODES]={
		{-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1}
	};

	//depth first search
	printf("depth first search:\n");
	depth_first_search(nodes, graph_mt, NUM_NODES, edges_mt);

	//print nodes information to see its discovery time and end processing time
	printf("discovery time and end processing time of each node:\n");
	print_nodes(nodes, NUM_NODES);

	//print edges of graph
	printf("label edge:\n");
	print_edges(edges_mt, NUM_NODES);

	return SUCCESS;
}
