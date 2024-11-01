#include "graph.h"
#include "shortest_path.h"
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

void measure(int size, int edges, int times) {	
	Graph graph = get_random_graph(size, edges);
    std::vector<int> tmp;

	auto t1 = steady_clock::now();
    for(int i = 0; i < times; i++){
        tmp = shortest_path(graph, (double)rand() / RAND_MAX * (size - 1), (double)rand() / RAND_MAX * (size - 1));
    }
	auto t2 = steady_clock::now();
    std::cout << "Vertexes: " << size << ", Edges: " << edges << ", time (mean): " << duration<double>(t2 - t1).count() / times << std::endl;
}

int main(int argc, char **argv) {
    int max_v = argc > 1 ? std::stoi(argv[1]) : 1;
    int max_e = argc > 2 ? std::stoi(argv[2]) : 1;
    int times = argc > 3 ? std::stoi(argv[3]) : 100;

    for(int i = 1; i <= 5; i++){
        for(int j = 1; j <= 5; j++){
            measure(max_v * pow(10, i), max_e * pow(10, j), times);
        }
    }

	return 0;
}