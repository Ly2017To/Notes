/*
 * graph_dfs.c
 *
 *  Created on: Sep 20, 2023
 *      Author: luyuan
 */

#ifndef GRAPH_DFS_C_
#define GRAPH_DFS_C_

#include <stdio.h>
#include <stdlib.h>
#include "graph_dfs.h"

void print_nodes(node_t nodes[], int num_nodes)
{
	int i;

	for(i=0;i<num_nodes;i++){
		printf("node %d: name=%c; color=%d; discovery_time=%d; endprocessing_time=%d\n",i,nodes[i].name,nodes[i].color,nodes[i].discovery_time,nodes[i].endprocessing_time);
	}

	return;
}

void print_edges(char edges_mt[][NUM_NODES], int num_nodes)
{
	int i, j;

	for(i=0;i<num_nodes;i++){
		for(j=0;j<num_nodes;j++){
			printf("%d ", edges_mt[i][j]);
		}
		printf("\n");
	}

	return;
}

node_t initialize_node(char name)
{
	node_t node;
	node.name=name;
	node.color=white;
	node.discovery_time=-1;
	node.endprocessing_time=-1;

	return node;
}

void label_backward_edges(node_t nodes[], char graph_mt[][NUM_NODES], int num_nodes, char edges_mt[][NUM_NODES])
{
	int i,j;

	for(i=0;i<num_nodes;i++){
		for(j=0;j<num_nodes;j++){
			if(graph_mt[i][j]==1&&edges_mt[i][j]!=Tree){
				if(nodes[i].discovery_time>nodes[j].discovery_time&&nodes[i].endprocessing_time<nodes[j].endprocessing_time){
					edges_mt[i][j]=Backward;
				}
			}
		}
	}

	return;
}

void label_forward_edges(node_t nodes[], char graph_mt[][NUM_NODES], int num_nodes, char edges_mt[][NUM_NODES])
{

	int i,j;

	for(i=0;i<num_nodes;i++){
		for(j=0;j<num_nodes;j++){
			if(graph_mt[i][j]==1&&edges_mt[i][j]!=Tree){
				if(nodes[i].discovery_time<nodes[j].discovery_time&&nodes[i].endprocessing_time>nodes[j].endprocessing_time){
					edges_mt[i][j]=Forward;
				}
			}
		}
	}

	return;
}

void label_cross_edges(char graph_mt[][NUM_NODES], int num_nodes, char edges_mt[][NUM_NODES])
{

	int i,j;

	for(i=0;i<num_nodes;i++){
		for(j=0;j<num_nodes;j++){
			if(graph_mt[i][j]==1&&edges_mt[i][j]!=Tree&&edges_mt[i][j]!=Backward&&edges_mt[i][j]!=Forward){
				edges_mt[i][j]=Cross;
			}
		}
	}

	return;
}


void depth_first_search(node_t nodes[], char graph_mt[][NUM_NODES], int num_nodes, char edges_mt[][NUM_NODES])
{
	int i, time;

	time=-1;

	for(i=0;i<num_nodes;i++){
		depth_first_search_node(nodes, graph_mt, num_nodes, i, &time, edges_mt);
	}

	label_backward_edges(nodes, graph_mt, num_nodes, edges_mt);
	label_forward_edges(nodes, graph_mt, num_nodes, edges_mt);
	label_cross_edges(graph_mt, num_nodes, edges_mt);

	return;
}

void depth_first_search_node(node_t nodes[], char graph_mt[][NUM_NODES], int num_nodes, int index, int *time, char edges_mt[][NUM_NODES])
{
	int i;

	//this node is completed, return
	if(nodes[index].color==black){
		return;
	}

	//this node is not yet discovered
	if(nodes[index].color==white){
		nodes[index].color=gray; //discovered but not complete
		nodes[index].discovery_time= ++(*time); //discovery time
	}

	//connected nodes of this node
	for(i=0;i<num_nodes;i++){
		if(graph_mt[index][i]==1&&nodes[i].color==white){
			nodes[i].color=gray; //discovered but not complete
			nodes[i].discovery_time= ++(*time); //discovery time
			edges_mt[index][i]= Tree; //a tree edge
			depth_first_search_node(nodes, graph_mt, num_nodes, i, time, edges_mt);
		}
	}

	//this nodes is discovered but not yet completed
	if(nodes[index].color==gray){
		nodes[index].color=black; //completed
		nodes[index].endprocessing_time= ++(*time); //endprocessing time
	}

	return;
}

#endif /* GRAPH_DFS_C_ */
