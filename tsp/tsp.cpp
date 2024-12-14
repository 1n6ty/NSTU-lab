#include "tsp.h"
#include <limits.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

std::vector<int> generate_random_path(const Graph &graph){
    std::vector<int> vertices = graph.get_vertices(), new_path;
    if(vertices.size() < 2) return std::vector<int>();

    Graph graph_cpy = Graph(graph);

    int start;
    while(true){
        start = vertices[rand() * (vertices.size() - 1) / RAND_MAX];

        new_path.push_back(start);

        vertices = graph_cpy.get_adjacent_vertices(start);
        if(vertices.size() == 0) break;

        graph_cpy.remove_vertex(start);
    }
    
    return new_path;
}

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

double get_lower_bound(const Graph &graph, const vector<int> &path){
    double len_num = 0, buff_len, min_edge_1, min_edge_2;

    vector<int>::const_iterator buff_iter;
    for(int v: graph.get_vertices()){
        buff_iter = std::find(path.begin(), path.end(), v);
        if(buff_iter == path.end()){
            min_edge_1 = __DBL_MAX__;
            min_edge_2 = __DBL_MAX__;
            for(int adj_v: graph.get_adjacent_vertices(v)){
                buff_len = graph.edge_weight(v, adj_v);
                if(buff_len < min_edge_1){
                    min_edge_2 = min_edge_1;
                    min_edge_1 = buff_len;
                }
            }
            if(min_edge_1 != __DBL_MAX__ && min_edge_2 != __DBL_MAX__){
                len_num += (min_edge_1 + min_edge_2) / 2;
            }
        } else if(buff_iter != path.begin() && buff_iter != path.end() - 1){
            len_num += (graph.edge_weight(v, *(buff_iter - 1)) + graph.edge_weight(v, *(buff_iter + 1))) / 2;
        } else if(buff_iter == path.begin()){
            min_edge_1 = __DBL_MAX__;
            for(int adj_v: graph.get_adjacent_vertices(v)){
                buff_len = graph.edge_weight(v, adj_v);
                if(buff_len < min_edge_1 && buff_iter + 1 != path.end() && adj_v != *(buff_iter + 1)){
                    min_edge_1 = buff_len;
                }
            }
            if(min_edge_1 != __DBL_MAX__ && buff_iter + 1 != path.end()){
                len_num += (min_edge_1 + graph.edge_weight(v, *(buff_iter + 1))) / 2;
            }
        } else {
            min_edge_1 = __DBL_MAX__;
            for(int adj_v: graph.get_adjacent_vertices(v)){
                buff_len = graph.edge_weight(v, adj_v);
                if(buff_len < min_edge_1 && adj_v != *(buff_iter - 1)){
                    min_edge_1 = buff_len;
                }
            }
            if(min_edge_1 != __DBL_MAX__){
                len_num += (min_edge_1 + graph.edge_weight(v, *(buff_iter - 1))) / 2;
            }
        }
    }

    return len_num;
}

vector<int> BnB(const Graph &graph, const vector<int> &visited, vector<int> &best_path){
    if(visited.size() == graph.get_vertices().size()) return get_min_path(graph, best_path, visited);
    
    vector<int> visited_cpy, path;

    for(int v: graph.get_vertices()){
        if(std::find(visited.begin(), visited.end(), v) == visited.end()){
            visited_cpy = visited;
            visited_cpy.push_back(v);
            if(get_lower_bound(graph, visited_cpy) < compute_path_length(graph, best_path)){
                path = BnB(graph, visited_cpy, best_path);
                best_path = get_min_path(graph, best_path, path);
            }
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
