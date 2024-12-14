#include "tsp.h"
#include <math.h>
#include <algorithm>

std::vector<std::vector<int>> SUS(const std::vector<std::vector<int>> &population, const std::vector<double> &weights, int N){
    std::vector<std::vector<int>> chosen;
    
    double F = 0, Dist, Point, Start, buff_sum_weights;
    for(double i: weights) F += i;

    Dist = F / N;
    Start = (double) rand() * Dist / RAND_MAX;

    int k = 0;
    buff_sum_weights = weights[k];
    for(int i = 0; i < N; i++){
        Point = Start + i * Dist;
        while(buff_sum_weights < Point){
            k ++;
            buff_sum_weights += weights[k];
        }
        chosen.push_back(population[k]);
    }

    return chosen;
}

int sum_row(const std::vector<int> &v){
    int s = 0;
    for(int i: v) s += i;
    return s;
}

std::vector<int> CrossoverER(const std::vector<int> &p1, const std::vector<int> &p2){
    int p_size = p1.size();

    std::vector<std::vector<int>> edge_map(p_size, std::vector<int>(p_size, 0));
    
    for(int i = 0; i < p_size - 1; i++){
        edge_map[p1[i]][p1[i + 1]] = 1;
        edge_map[p2[i]][p2[i + 1]] = 1;

        edge_map[p1[i + 1]][p1[i]] = 1;
        edge_map[p2[i + 1]][p2[i]] = 1;
    }
    edge_map[*(p1.end() - 1)][p1[0]] = 1;
    edge_map[*(p2.end() - 1)][p2[0]] = 1;

    edge_map[p1[0]][*(p1.end() - 1)] = 1;
    edge_map[p2[0]][*(p2.end() - 1)] = 1;

    int cur_v = p1[rand() * (p_size - 1) / RAND_MAX];
    
    std::vector<int> offSpring, Candidates;

    offSpring.push_back(cur_v);
    int min_v_buff, buff_sum;
    while(offSpring.size() < p_size){
        Candidates.clear();
        for(int i = 0; i < p_size; i++) if(edge_map[cur_v][i]) Candidates.push_back(i);

        edge_map[cur_v] = std::vector<int>(p_size, 0);
        for(int i = 0; i < p_size; i++){
            edge_map[i][cur_v] = 0;
        }

        min_v_buff = __INT_MAX__;
        for(int i: Candidates){
            buff_sum = sum_row(edge_map[i]);
            if(buff_sum < min_v_buff){
                min_v_buff = buff_sum;
                cur_v = i;
            }
        }

        if(Candidates.size() == 0){
            for(int i = 0; i < p_size; i++){
                if(sum_row(edge_map[i]) != 0){
                    cur_v = i;
                    break;
                }
            }
        }

        offSpring.push_back(cur_v);
    }

    return offSpring;
}

std::vector<int> tsp_genetic(const Graph &graph, int P, int N, int MaxIt, float Pm){
    if(graph.get_vertices().size() < 2) return std::vector<int>();

    std::vector<std::vector<int>> population, parents;
    std::vector<double> weights(P, 0.0); 
    for(int i = 0; i < P; i++){
        population.push_back(generate_random_path(graph));
    }

    for(int it = 0; it < MaxIt; it++){
        for(int i = 0; i < P; i++){
            weights[i] = compute_path_length(graph, population[i]);
        }

        parents = SUS(population, weights, N);
        population.clear();
        while(population.size() < P){
            population.push_back(
                CrossoverER(
                    parents[rand() * (N - 1) / RAND_MAX],
                    parents[rand() * (N - 1) / RAND_MAX]
                )
            );
        }

        for(int i = 0; i < P; i++){
            if(rand() >= int(Pm * RAND_MAX)){
                population[i] = opt_improve(graph, population[i]);
            }
        }
    }

    double min_weight = __DBL_MAX__;
    int min_index_c;
    for(int i = 0; i < P; i++){
        weights[i] == compute_path_length(graph, population[i]);
        if(weights[i] < min_weight){
            min_weight = weights[i];
            min_index_c = i;
        }
    }

    return population[min_index_c];
}