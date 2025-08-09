/*
 * main.c
 *
 *  Created on: Sep 24, 2023
 *      Author: luyuan
 */


#include "kruskal.h"

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

	list<edge_t> edges;
	edges = read_edges(weights_path);
	sort_edges(edges);

	cout << "edges of graph:" << endl;
	print_edges(edges, nodes_name);

	cout << "Kruskal Algorithm:" << endl;
	list<edge_t> mst;
	int sum_weights = mst_kruskal(edges, mst, nodes_name.size()-1);
	cout << "sum weights of edges " << sum_weights << endl;
	print_edges(mst, nodes_name);


	return EXIT_SUCCESS;
}
