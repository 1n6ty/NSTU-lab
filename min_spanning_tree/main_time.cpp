#include "graph.h"
#include "min_spanning_tree.h"
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <cmath>

using namespace std::chrono;

Graph get_random_graph(int size, int edges) {
	Graph graph;
    for(int i = 0; i < size; i++) graph.add_vertex(i);
    for(int i = 0; i < edges; i++){
        graph.add_edge((double)rand() / RAND_MAX * (size - 1), (double)rand() / RAND_MAX * (size - 1), rand());
    }
    return graph;
}

void measure(int size, int edges) {	
	Graph graph = get_random_graph(size, edges);
    std::vector<std::pair<int, int>> tmp;

	auto t1 = steady_clock::now();
    tmp = min_spanning_tree(graph);
	auto t2 = steady_clock::now();
    std::cout << "Vertexes: " << size << ", Edges: " << edges << ", time: " << duration<double>(t2 - t1).count()<< std::endl;
}

int main(int argc, char **argv) {
    int max_v = argc > 1 ? std::stoi(argv[1]) : 1;
    int max_e = argc > 2 ? std::stoi(argv[2]) : 1;

    for(int i = 1; i <= 5; i++){
        for(int j = 1; j <= 5; j++){
            measure(max_v * pow(10, i), max_e * pow(10, j));
        }
    }

	return 0;
}