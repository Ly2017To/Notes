/*
 * bellman_ford.cpp
 *
 *  Created on: Sep 24, 2023
 *      Author: luyuan
 */

#include "bellman_ford.h"

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
		nodes.push_back(node);
	}

	return;
}

void print_nodes(vector<node_t> nodes)
{
	for(size_t i=0; i<nodes.size(); i++){
		cout << i << " " << nodes[i].name << " " << nodes[i].distance << " " << endl;
	}

	return;
}

void set_source_node(int source_index, vector<node_t> &nodes)
{
	nodes[source_index].distance=0;

	return;
}

void relax_node(int u, vector<vector<int>> graph_mt, vector<node_t> &nodes)
{
	for(size_t i=0; i<nodes.size(); i++){
		if(graph_mt[u][i]!=0){
			if(nodes[i].distance > nodes[u].distance+graph_mt[u][i]){
				nodes[i].distance = nodes[u].distance+graph_mt[u][i];
			}
		}
	}

	return;
}

bool bellman_ford_sssp(vector<vector<int>> graph_mt, int source_index, vector<node_t> &nodes)
{
	//set source node
	set_source_node(source_index,nodes);

	//relax all the nodes for nodes.size()-1 times
	for(size_t count = 0; count<nodes.size()-1; count++){
		for(size_t i=0; i<nodes.size(); i++){
			relax_node(i,graph_mt,nodes);
		}
	}

	//check
	for(size_t i=0; i<nodes.size(); i++){
		for(size_t j=0; j<nodes.size();j++){
			if(graph_mt[i][j]!=0){
				if(nodes[j].distance>nodes[i].distance+graph_mt[i][j]){
					return false;
				}
			}
		}
	}

	return true;
}



