#include "tsp.h"
#include <limits.h>
#include <algorithm>
#include <vector>

using namespace std;

vector<int> BnB_greedy(const Graph &graph, const vector<int> &visited){
    if(graph.get_vertices().size() == visited.size()) return visited;
    
    vector<int> visited_cpy(visited);

    int min_v = -1;
    double min_len = __DBL_MAX__, buff_len;
    for(int v: graph.get_adjacent_vertices(visited.at(visited.size() - 1))){
        if(std::find(visited.begin(), visited.end(), v) == visited.end()){
            buff_len = graph.edge_weight(*visited.rbegin(), v);
            if(buff_len < min_len){
                min_v = v;
                min_len = buff_len;
            }
        }
    }
    visited_cpy.push_back(min_v);
    return BnB_greedy(graph, visited_cpy);
}

vector<int> tsp_greedy(const Graph &graph) {
    // Return the shortest (with a minimal total weight) tour as an array of vertices.
    // Return empty array if there is no tour.

    vector<int> visited, verteces;

    verteces = graph.get_vertices();
    if(verteces.size() > 1) visited.push_back(verteces.at(0));
    else return std::vector<int>();

    return BnB_greedy(graph, visited);
}