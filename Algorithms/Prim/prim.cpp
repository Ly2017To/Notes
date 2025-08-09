/*
 * prim.cpp
 *
 *  Created on: Sep 24, 2023
 *      Author: luyuan
 */

#ifndef PRIM_CPP_
#define PRIM_CPP_

#include "prim.h"

void print_matrix(vector<vector<int>> mt)
{
	for(size_t i=0; i<mt.size(); i++){
		for(size_t j=0; j<mt[i].size(); j++){
			cout << mt[i][j] << " ";
		}
		cout << endl;
	}

	return;
}

vector<int> get_int_from_string(string str) {

	vector<int> v;
	stringstream ss(str);

	while (!ss.eof()) {
		string substr;
		getline(ss, substr, ',');
		try {
			v.push_back(stoi(substr,nullptr,10));
		}
		catch (const std::invalid_argument& ia) {
			//std::cerr << "Invalid argument: " << ia.what() << '\n';
			break;
		}
	}
	/*
	for (size_t i = 0; i < v.size(); i++){
		cout << v[i] << " ";
	}
	cout << endl;
	 */

	return v;
}

vector<string> read_nodes_name(string file_path)
{
	fstream file;
	file.open (file_path);

	string line;
	vector<string> names;

	if (file.is_open()){
		while (getline(file,line)){
			if(!line.empty()){
				names.push_back(line);
			}
		}
	}

	file.close();

	return names;
}


vector<vector<int>> read_weights(string file_path)
{
	fstream file;
	file.open (file_path);

	string line;
	vector<vector<int>> mt;

	if (file.is_open()){
		while (getline(file,line)){
			//cout << line << '\n';
			vector<int> v;
			v = get_int_from_string(line);
			if(!v.empty()){
				mt.push_back(v);
			}
		}
	}

	file.close();

	return mt;
}

edge_t select_safe_edge(int arr[], vector<vector<int>> weights, int num_nodes)
{
	int min_weight = INT_MAX;

	edge_t edge;

	//select a safe edge crossing the cut
	for(int i=0; i<num_nodes; i++){
		if(arr[i]==1){
			for(int j=0; j<num_nodes; j++){
				if(arr[j]==0&&weights[i][j]>0&&weights[i][j]<min_weight){
					min_weight=weights[i][j];
					edge.weight=min_weight;
					edge.source=i;
					edge.destination=j;
				}
			}
		}
	}

	return edge;
}

int mst_prim(vector<vector<int>> graph_mt, int num_nodes, list<edge_t> & mst)
{
	//an array to indicate whether a node is visited or not
	int arr[num_nodes]={0};
	//the first node to visit
	int i=0;
	arr[i]=1;

	while(mst.size()<num_nodes-1){
		edge_t edge = select_safe_edge(arr,graph_mt,num_nodes);
		arr[edge.source]=1;
		arr[edge.destination]=1;
		mst.push_back(edge);
	}

	int sum_weights = 0;
	for(auto edge: mst){
		sum_weights+= edge.weight;
	}

	return sum_weights;
}

void print_edges(list<edge_t> edges, vector<string> nodes_name)
{
	for(auto edge: edges){
		cout << edge.weight << " " << nodes_name [edge.source] << " " << nodes_name [edge.destination] << " "<<  endl;
	}

	return;
}

#endif


