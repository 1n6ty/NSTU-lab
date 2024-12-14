#include "tsp.h"
#include <limits.h>
#include <algorithm>
#include <vector>

using namespace std;

vector<int> BnB_brute(const Graph &graph, const vector<int> &visited, vector<int> &best_path){
    if(visited.size() == graph.get_vertices().size()) return get_min_path(graph, best_path, visited);
    
    vector<int> visited_cpy, path;

    for(int v: graph.get_vertices()){
        if(std::find(visited.begin(), visited.end(), v) == visited.end()){
            visited_cpy = visited;
            visited_cpy.push_back(v);
            
            path = BnB_brute(graph, visited_cpy, best_path);
            best_path = get_min_path(graph, best_path, path);
        }
    }
    return best_path;
}

vector<int> tsp_brute(const Graph &graph) {
    // Return the shortest (with a minimal total weight) tour as an array of vertices.
    // Return empty array if there is no tour.

    vector<int> best_path = {};
    vector<int> visited = {};

    return BnB_brute(graph, visited, best_path);
}