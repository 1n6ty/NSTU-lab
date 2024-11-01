#include "min_spanning_tree.h"
#include <algorithm>
#include <iostream>

using namespace std;

vector<pair<int, int>> min_spanning_tree(const Graph &graph) {
	// Return minimal spanning tree (MST) for the graph as array of edges.
	// Each edge is represented by its two vertices.
	// Order of edges in result and vertices in edges doesn't matter.
	// Return empty array if MST doesn't exist.

	// Kruskal algorithm

	vector<pair<int, int>> result;

	Graph graph_cpy(graph);
	vector<tuple<int, int, double>> edges;
	map<int, int> components;
	for(int i: graph_cpy.get_vertices()){
		for(pair<int, double> edge_i: graph_cpy.get_adjacent_edges(i)){
			edges.push_back(std::make_tuple(i, edge_i.first, edge_i.second));
			graph_cpy.remove_edge(i, edge_i.first);
		}
		components[i] = i;
	}
	std::sort(edges.begin(), edges.end(), [&](const tuple<int, int, double> &a, const tuple<int, int, double> &b){return std::get<2>(a) < std::get<2>(b);});

	int component2change, component_that_change, vertex_1, vertex_2;
	for(vector<tuple<int, int, double>>::const_iterator edge = edges.begin(); edge != edges.end(); edge ++){
		vertex_1 = std::get<0>(*edge);
		vertex_2 = std::get<1>(*edge);
		if(components[vertex_1] != components[vertex_2]){
			result.push_back(std::make_pair(vertex_1, vertex_2));
			component2change = components[vertex_2];
			component_that_change = components[vertex_1];
			for(int i: graph.get_vertices()){
				if(components[i] == component_that_change) components[i] = component2change;
			}
		}
	}

	return result;
}
