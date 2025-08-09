/*
 * main.cpp
 *
 *  Created on: Sep 24, 2023
 *      Author: luyuan
 */

/*
    This folder implements Dijkstra algorithm
    1. compute single source shortest path of a graph;
    2. if there exists negative weight edges but no negative edges within any cycle, then optimal solution is not guaranteed;
    3. if there exists negative weight edges within any cycle, then solution is meaningless;
 */


#include "dijkstra.h"

int main() {

	//make an example graph of 5 nodes
	//read it from a file the connection matrix with weights
	string nodes_path = "./nodes_name.txt";
	string weights_path = "./graph_weights.txt";

	vector<string> nodes_name;
	nodes_name = read_nodes_name(nodes_path);
	//cout << nodes_name.size() << endl;

	vector<vector<int>> graph_mt;
	graph_mt = read_weights(weights_path);

	cout << "graph matrix:" << endl;
	print_matrix(graph_mt);

	cout << "Dijkstra Algorithm:" << endl;
	//initialize nodes
	//cout << graph_mt.size() << endl;
	vector<node_t> nodes;
	initialize_nodes(nodes,nodes_name);
	//print_nodes(nodes);

	//dijkstra algorithm for single source shortest path
	//parameters: matrix graph; source node index; all the nodes of the graph initialized;
	dijkstra_sssp(graph_mt, 0, nodes);
	print_nodes(nodes);


	return EXIT_SUCCESS;
}
