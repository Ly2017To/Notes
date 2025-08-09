/*
 * dijkstra.cpp
 *
 *  Created on: Sep 24, 2023
 *      Author: luyuan
 */

#ifndef DIJKSTRA_CPP_
#define DIJKSTRA_CPP_

#include "dijkstra.h"

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

void initialize_nodes(vector<node_t> &nodes, vector<string> nodes_name)
{
	for(size_t i=0; i<nodes_name.size();i++){
		node_t node;
		node.name = nodes_name[i];
		node.distance=INT_MAX;
		node.relaxed=false;
		nodes.push_back(node);
	}

	return;
}

void print_nodes(vector<node_t> nodes)
{
	for(size_t i=0; i<nodes.size(); i++){
		cout << i << " " << nodes[i].name << " " << nodes[i].distance << " " << nodes[i].relaxed << " " << endl;
	}

	return;
}

bool nodes_all_relaxed(vector<node_t> &nodes)
{
	bool flag = true;

	for(size_t i=0; i<nodes.size(); i++){
		if(!nodes[i].relaxed){
			flag = false;
			break;
		}
	}

	return flag;
}

size_t min_index(vector<node_t> nodes)
{
	int min_value;
	size_t index;

	min_value = INT_MAX;

	for(size_t i=0; i<nodes.size(); i++){
		if(!nodes[i].relaxed){
			//cout << "i " << i << " " << endl;
			if(nodes[i].distance < min_value){
				min_value = nodes[i].distance;
				index = i;
			}
		}
	}

	return index;

}

void set_source_node(int source_index, vector<node_t> &nodes)
{
	nodes[source_index].distance=0;

	return;
}

void relax_node(int u, vector<vector<int>> graph_mt, vector<node_t> &nodes)
{
	for(size_t i=0; i<nodes.size(); i++){
		if(graph_mt[u][i]>=0){
			if(nodes[i].distance > nodes[u].distance+graph_mt[u][i]){
				nodes[i].distance = nodes[u].distance+graph_mt[u][i];
			}
		}
	}

	nodes[u].relaxed=true;

	return;
}

void dijkstra_sssp(vector<vector<int>> graph_mt, int source_index, vector<node_t> &nodes)
{
	set_source_node(source_index,nodes);

	while(!nodes_all_relaxed(nodes)){
		size_t u = min_index(nodes);
		//cout << "u " << u << " " << endl;
		relax_node(u,graph_mt,nodes);
	}

	return;
}


#endif /* DIJKSTRA_CPP_ */
