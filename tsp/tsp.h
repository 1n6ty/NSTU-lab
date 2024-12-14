#include "graph.h"

#include <vector>

/// Solve Travelling Salesman Problem (TSP) for the graph:
/// Find the shortest (with a minimal total weight) tour and return it as an array of vertices.

std::vector<int> tsp(const Graph &graph);
std::vector<int> tsp_brute(const Graph &graph);
std::vector<int> tsp_greedy(const Graph &graph);

std::vector<int> tsp_2opt(const Graph &graph);
std::vector<int> opt_improve(const Graph &graph, std::vector<int> path);
std::vector<int> tsp_genetic(const Graph &graph, int P, int N, int MaxIt, float Pm);

double compute_path_length(const Graph &graph, const std::vector<int> &path);

std::vector<int> get_min_path(const Graph &graph, const std::vector<int> &path1, const std::vector<int> &path2);

double get_lower_bound(const Graph &graph, const std::vector<int> &path);

std::vector<int> generate_random_path(const Graph &graph);