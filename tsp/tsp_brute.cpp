#include "tsp.h"
#include <limits.h>
#include <algorithm>

using namespace std;

double compute_path_length(const Graph &graph, const vector<int> &path){
    if(path.size() < 2) return __DBL_MAX__;

    double len_sum = 0;
    for(vector<int>::const_iterator v_it = path.cbegin(); v_it != path.cend() - 1; v_it++){
        len_sum += graph.edge_weight(*v_it, *(v_it + 1));
    }
    return len_sum;
}

vector<int> get_min_path(const Graph &graph, const vector<int> &path1, const vector<int> &path2){
    return (compute_path_length(graph, path1) <= compute_path_length(graph, path2)) ? path1: path2;
}

vector<int> BnB(const Graph &graph, const vector<int> &visited, vector<int> &best_path){
    if(visited.size() == graph.get_vertices().size()) return get_min_path(graph, best_path, visited);
    
    vector<int> visited_cpy, path;

    for(int v: graph.get_vertices()){
        if(std::find(visited.begin(), visited.end(), v) == visited.end()){
            visited_cpy = visited;
            visited_cpy.push_back(v);
            
            path = BnB(graph, visited_cpy, best_path);
            best_path = get_min_path(graph, best_path, path);
        }
    }
    return best_path;
}

vector<int> tsp(const Graph &graph) {
    // Return the shortest (with a minimal total weight) tour as an array of vertices.
    // Return empty array if there is no tour.

    vector<int> best_path = {};
    vector<int> visited = {};

    return BnB(graph, visited, best_path);
}