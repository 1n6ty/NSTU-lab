#include "shortest_path.h"
#include <map>
#include <queue>
#include <utility>
#include <limits>

using namespace std;

const vector<int> build_path(const std::map<int, int> &parent, int end_vertex){
    vector<int> result;
    int current_vertex = end_vertex;
    while(current_vertex != -1){
        result.push_back(current_vertex);
        current_vertex = parent.at(current_vertex);
    }

    if(result.size() == 1) return vector<int>{};

    int result_size = result.size() / 2;
    for(int i = 0; i < result_size; i++){
        *(result.begin() + i) += *(result.end() - 1 - i);
        *(result.end() - 1 - i) = *(result.begin() + i) - *(result.end() - 1 - i);
        *(result.begin() + i) -= *(result.end() - 1 - i);
    }

    return result;
}

bool cmp_q_elements(const std::pair<int, double> &a, const std::pair<int, double> &b){
    return a.second > b.second;
}

vector<int> shortest_path(const Graph &graph, int start_vertex, int end_vertex) {
    // Return shortest path in the graph from start vertex to end vertex as array of vertices.
    // First item in the result should be start vertex, last - end vertex.
    // Return empty array if there is no path.

    std::map<int, double> dist;
    std::map<int, int> parent;

    std::priority_queue<std::pair<int, double>, std::vector<std::pair<int, double>>, decltype(&cmp_q_elements)> q(cmp_q_elements);
    for(int v: graph.get_vertices()){
        if(v == start_vertex){
            q.push(std::make_pair(v, 0));
            dist[v] = 0;
            parent[v] = -1;
        } else {
            dist[v] = __DBL_MAX__;
        }
    }

    std::pair<int, double> buff_e;
    while(!q.empty()){
        buff_e = q.top();
        q.pop();
        if(buff_e.second == dist[buff_e.first]){
            if(buff_e.first == end_vertex){
                return build_path(parent, end_vertex);
            }
            for(std::pair<int, double> adj: graph.get_adjacent_edges(buff_e.first)){
                if(dist[adj.first] - adj.second > dist[buff_e.first]){
                    dist[adj.first] = dist[buff_e.first] + adj.second;
                    parent[adj.first] = buff_e.first;
                    q.push(std::make_pair(adj.first, dist[adj.first]));
                }
            }
        }
    }
    
    return vector<int>{};
}
