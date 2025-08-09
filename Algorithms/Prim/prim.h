/*
 * prim.h
 *
 *  Created on: Sep 24, 2023
 *      Author: luyuan
 */

#ifndef PRIM_H_
#define PRIM_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm>
#include <limits.h>
#include <utility>

using namespace std;

typedef struct edge{
	int weight;
	int source;
	int destination;
}edge_t;

void print_matrix(vector<vector<int>>);
vector<int> get_int_from_string(string);
vector<string> read_nodes_name(string);
vector<vector<int>> read_weights(string);
edge_t select_safe_edge(int [], vector<vector<int>>, int);
int mst_prim(vector<vector<int>>, int, list<edge_t> &);
void print_edges(list<edge_t>, vector<string>);

#endif /* PRIM_H_ */
