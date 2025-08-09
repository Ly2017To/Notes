/*
 * main.c
 *
 *  Created on: Sep 24, 2023
 *      Author: luyuan
 */

#include "prim.h"

/*
    This folder implements Kruskal algortithm
    1. compute minimum spanning tree of a connected and undirected graph;
    2. a tree that acyclic and cover all vertices with minimum sum weights;
*/

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

	cout << "PRIM Algorithm:" << endl;
	list<edge_t> mst;
	//cout << graph_mt.size() << endl;
	int sum_weights = mst_prim(graph_mt, nodes_name.size(), mst);
	cout << "sum weights of edges " << sum_weights << endl;
	print_edges(mst, nodes_name);

	return EXIT_SUCCESS;
}
