#include "tsp.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <cmath>

using namespace std::chrono;

void fill_graph(Graph &graph, int size){
    for(int v = 0; v < size; v++){
        graph.add_vertex(v);
    }

    for(int v = 0; v < size; v++){
        for(int adj_v = 0; adj_v < size; adj_v++){
            if(adj_v != v && !graph.has_edge(v, adj_v)){
                graph.add_edge(v, adj_v, rand());
            }
        }
    }
}

void measure(int size, int P, int N, int MaxIt, float Pm){
    Graph graph;
    fill_graph(graph, size);

	std::vector<int> tmp;

	auto t1 = steady_clock::now();
	tmp = tsp(graph);
	auto t2 = steady_clock::now();
    std::cout << "N: " << size << ", time (tsp): " << duration<double>(t2 - t1).count() << " ";
    std::cout << "Length: " << compute_path_length(graph, tmp) << std::endl;

    t1 = steady_clock::now();
	tmp = tsp_brute(graph);
	t2 = steady_clock::now();
    std::cout << "N: " << size << ", time (tsp_brute): " << duration<double>(t2 - t1).count() << " ";
    std::cout << "Length: " << compute_path_length(graph, tmp) << std::endl;

    t1 = steady_clock::now();
	tmp = tsp_greedy(graph);
	t2 = steady_clock::now();
    std::cout << "N: " << size << ", time (tsp_greedy): " << duration<double>(t2 - t1).count() << " ";
    std::cout << "Length: " << compute_path_length(graph, tmp) << std::endl;

    t1 = steady_clock::now();
	tmp = tsp_2opt(graph);
	t2 = steady_clock::now();
    std::cout << "N: " << size << ", time (tsp_2opt): " << duration<double>(t2 - t1).count() << " ";
    std::cout << "Length: " << compute_path_length(graph, tmp) << std::endl;

    t1 = steady_clock::now();
	tmp = tsp_genetic(graph, P, N, MaxIt, Pm);
	t2 = steady_clock::now();
    std::cout << "N: " << size << ", time (tsp_genetic): " << duration<double>(t2 - t1).count() << " ";
    std::cout << "Length: " << compute_path_length(graph, tmp) << std::endl;
}

int main(int argc, char **argv) {
    int max_pow = argc > 1 ? std::stoi(argv[1]) : 6;
    int P = argc > 2 ? std::stoi(argv[2]) : 6;
    int N = argc > 3 ? std::stoi(argv[3]) : 6;
    int MaxIt = argc > 4 ? std::stoi(argv[4]) : 6;
    float Pm = argc > 5 ? std::stod(argv[5]) : 6;
    
	for (int i = 1; i <= max_pow; i++) {
		measure(i, P, N, MaxIt, Pm);
	}

	return 0;
}