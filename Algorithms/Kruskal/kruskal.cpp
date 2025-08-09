/*
 * kruskal.cpp
 *
 *  Created on: Sep 24, 2023
 *      Author: luyuan
 */

#ifndef KRUSKAL_CPP_
#define KRUSKAL_CPP_

#include "kruskal.h"

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


list<edge_t> read_edges(string file_path)
{
	fstream file;
	file.open (file_path);

	string line;
	list<edge_t> edges;

	if (file.is_open()){
		int i = 0;
		while (getline(file,line)){
			//cout << line << '\n';
			vector<int> v;
			v = get_int_from_string(line);
			if(!v.empty()){
				for(size_t j=0; j<v.size(); j++){
					if(v[j]>0){
						edge_t edge;
						edge.source=i;
						edge.destination=(int)j;
						edge.weight=v[j];
						edges.push_back(edge);
					}
				}
			}
			i++;
		}
	}

	file.close();

	return edges;
}

bool compare_edges(edge_t e1, edge_t e2)
{
	return (e1.weight<e2.weight);
}

void sort_edges(list<edge_t> & edges)
{
	edges.sort(compare_edges);

	return;
}

void print_edges(list<edge_t> edges, vector<string> nodes_name)
{
	for(auto edge: edges){
		cout << edge.weight << " " << nodes_name [edge.source] << " " << nodes_name [edge.destination] << " "<<  endl;
	}

	return;
}

int find_set(int arr[], int node_index)
{
	int parent_index = arr[node_index];

	if(parent_index==node_index){
		return node_index;
	}

	return find_set(arr,parent_index);
}

int mst_kruskal(list<edge_t> edges, list<edge_t> & mst, size_t num_edges)
{
	//based on the idea of quick union
	//an array to store the representation of each set
	int arr[num_edges+1];
	//initially, each set contains only one node, itself
	for(int i=0; i<num_edges+1; i++){
		arr[i]=i;
	}

	while(!edges.empty()){

		edge_t edge = edges.front();

		//extract the index of the two points of this edge
		int source = edge.source;
		int destination = edge.destination;

		//find the index of the representation of the set source node belongs
		int source_set = find_set(arr, source);
		//find the index of the representation of the set destination node belongs
		int destination_set = find_set(arr, destination);

		//if the two points of this edge does not form a cycle
		if(source_set!=destination_set){
			arr[source_set]=arr[destination_set];
			mst.push_back(edge);
		}

		edges.pop_front();

		if(mst.size()==num_edges){
			break;
		}

	}

	int sum_weights = 0;

	for(auto edge: mst){
		sum_weights+= edge.weight;
	}

	return sum_weights;
}



#endif

