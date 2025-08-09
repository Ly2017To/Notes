//============================================================================
// Name        : graph_BFS.cpp
// Author      : luyuan
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#ifndef GRAPH_DFS_CPP_
#define GRAPH_DFS_CPP_

#include "graph_bfs.h"

void print_nodes(node_t nodes[], int num_nodes)
{
	for(int i=0;i<num_nodes;i++){
		cout << "node:" << i << " " << nodes[i].name << " "
				<< "color:" << nodes[i].color << " "
				<< "distance:" << nodes[i].distance << endl;
	}

	return;
}

node_t initialize_node(char name)
{
	node_t node;
	node.name=name;
	node.color=white;
	node.distance=-1;

	return node;
}

// Print the queue
void print_queue(queue<int> nodes_queue)
{
	queue<int> q = nodes_queue;
	while (!q.empty()) {
		cout << '\t' << q.front();
		q.pop();
	}
	cout << '\n';
}

void bread_first_search_node(node_t nodes[], char graph_mt[][NUM_NODES], int num_nodes, int index, queue<int> & nodes_queue)
{
	if(nodes[index].color==black){
		return;
	}

	//starting node
	if(nodes[index].color==white){
		nodes_queue.push(index); //push the node index to discovered nodes FIFO queue
		//print_queue(nodes_queue);
		nodes[index].color=gray;
		nodes[index].distance=0;
	}

	//breadth first search of all the nodes reachable from nodes[index]
	for(int i=0; i<num_nodes; i++){
		if(graph_mt[index][i]==1&&nodes[index].color==gray&&nodes[i].color==white){
			nodes_queue.push(i); //push this node index to discovered nodes FIFO queue
			nodes[i].color=gray;
			nodes[i].distance=nodes[index].distance+1; //compute distance to the starting node
		}
	}

	//when finishes, pop the node index from the discovered nodes FIFO queue
	nodes[index].color=black;
	nodes_queue.pop();

	//while the discovered nodes FIFO queue is not empty
	while(!nodes_queue.empty()){
		//print_queue(nodes_queue);
		int node_index = nodes_queue.front(); //extract the first node index in the queue
		bread_first_search_node(nodes,graph_mt,num_nodes, node_index, nodes_queue);
	}

	return;
}

void bread_first_search(node_t nodes[], char graph_mt[][NUM_NODES], int num_nodes)
{
	queue<int> discovered_nodes_queue; //a FIFO queue to store the nodes discovered by index
	int start_index;
	//breadth first search from node 1 called 's'
	start_index = 1;
	bread_first_search_node(nodes,graph_mt,num_nodes, start_index, discovered_nodes_queue);

	return;
}



#endif
