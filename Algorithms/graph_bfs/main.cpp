//============================================================================
// Name        : main.cpp
// Author      : luyuan
// Version     :
// Copyright   : Your copyright notice
// Description : Breadth First Search in C++, Ansi-style
//============================================================================

/*
	graph_bfs demonstrates Breadth First Search with a graph represented by matrix
    1. It visits all the nodes of a graph starting from a node;
    2. When visiting a graph, it labels the nodes as white(not yet discovered), gray(discovered and not yet completed) and black(completed).
	3. It computes the minimum distance of all the nodes can be reached from the starting node. If not reachable, the distance is initialized as -1.
*/

#include "graph_bfs.h"

int main() {

	//make an example graph of 8 nodes
	//initialize the nodes;
	node_t nodes[NUM_NODES];
	nodes[0]=initialize_node('r');
	nodes[1]=initialize_node('s');
	nodes[2]=initialize_node('t');
	nodes[3]=initialize_node('u');
	nodes[4]=initialize_node('v');
	nodes[5]=initialize_node('w');
	nodes[6]=initialize_node('x');
	nodes[7]=initialize_node('y');
	//initialize the connection matrix
	//1:connected; 0:not connected;
	char graph_mt[NUM_NODES][NUM_NODES]={
			{0,1,0,0,1,0,0,0},
			{1,0,0,0,0,1,0,0},
			{0,0,0,1,0,1,1,0},
			{0,0,1,0,0,0,0,1},
			{1,0,0,0,0,0,0,0},
			{0,1,1,0,0,0,1,0},
			{0,0,1,0,0,1,0,1},
			{0,0,0,1,0,0,1,0},
	};

	cout << "Breadth First Search:" << endl;

	bread_first_search(nodes, graph_mt, NUM_NODES);

	print_nodes(nodes, NUM_NODES);

	return SUCCESS;
}




