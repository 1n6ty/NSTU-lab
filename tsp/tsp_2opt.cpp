#include "tsp.h"

void transform(std::vector<int> &path, int A, int C){
    std::vector<int> new_path;

    new_path.push_back(path[A]);
    for(int i = C; i > A; i--) new_path.push_back(path[i]);
    for(int i = C + 1; i < path.size(); i++) new_path.push_back(path[i]);
    for(int i = 0; i < A; i++) new_path.push_back(path[i]);

    path = new_path;
}

std::vector<int> opt_improve(const Graph &graph, std::vector<int> path){
    if(path.size() < 4) return path;

    double old_weights, new_weights;
    
    for(int i = 0; i < path.size() - 3; i++){
        for(int j = i + 2; j < path.size() - 1; j++){
            old_weights = graph.edge_weight(path[i], path[i + 1]) + graph.edge_weight(path[j], path[j + 1]);
            new_weights = graph.edge_weight(path[i], path[j]) + graph.edge_weight(path[i + 1], path[j + 1]);
            if(new_weights < old_weights) transform(path, i, j);
        }
    }

    return path;
}

std::vector<int> tsp_2opt(const Graph &graph){
    std::vector<int> current_path = generate_random_path(graph), improved_path;

    improved_path = opt_improve(graph, current_path);
    while(compute_path_length(graph, improved_path) < compute_path_length(graph, current_path)){
        current_path = improved_path;
        improved_path = opt_improve(graph, current_path);
    }
    return current_path;
}