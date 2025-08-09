/*
 * bellman_ford.h
 *
 *  Created on: Sep 24, 2023
 *      Author: luyuan
 */

#ifndef BELLMAN_FORD_H_
#define BELLMAN_FORD_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <limits.h>

using namespace std;

typedef struct node{
	string name;
	int distance;
}node_t;

void print_matrix(vector<vector<int>>);
vector<int> get_int_from_string(string);
vector<string> read_nodes_name(string);
vector<vector<int>> read_weights(string);
void initialize_nodes(vector<node_t> &, vector<string>);
void print_nodes(vector<node_t>);
size_t min_index(vector<node_t>);
void set_source_node(int, vector<node_t> &);
bool bellman_ford_sssp(vector<vector<int>>, int, vector<node_t> &);


#endif /* BELLMAN_FORD_H_ */
