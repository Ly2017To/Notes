/*
 * kruskal.h
 *
 *  Created on: Sep 24, 2023
 *      Author: luyuan
 */

#ifndef KRUSKAL_H_
#define KRUSKAL_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

typedef struct edge{
	int weight;
	int source;
	int destination;
}edge_t;

void print_matrix(vector<vector<int>>);
vector<int> get_int_from_string(string);
vector<string> read_nodes_name(string);
list<edge_t> read_edges(string);
bool compare_edges(edge_t, edge_t);
void sort_edges(list<edge_t> &);
void print_edges(list<edge_t>, vector<string>);
int find_set(int [], int);
int mst_kruskal(list<edge_t>, list<edge_t> &, size_t);



#endif /* KRUSKAL_H_ */
