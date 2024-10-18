#include "activities.h"
#include <algorithm>

std::vector<Activity> get_max_activities(const std::vector<Activity> &activities) {
    if(activities.size() == 0) return {};

    std::vector<Activity> activities_sorted(activities);
    std::sort(activities_sorted.begin(), activities_sorted.end(), [&](auto act1, auto act2){return act1 < act2;});

    std::vector<Activity> response = {activities_sorted[0]};
    for(auto it = activities_sorted.cbegin() + 1; it != activities_sorted.cend(); it++){
        if((response.cend() - 1)->finish <= it->start) response.push_back(*it);
    }

    return response;
}

void put_max_subsequence(const std::vector<Activity> &activities_sorted, std::vector<Activity> &max_seq, std::vector<Activity> buff = {}){
    if(activities_sorted.size() == 0){
        if(max_seq.size() < buff.size()) max_seq = buff;
    } else {
        std::vector<Activity> buff_cpy;
        for(auto it = activities_sorted.cbegin(); it != activities_sorted.cend(); it++){
            buff_cpy = buff;
            buff_cpy.push_back(*it);
            for(auto it2 = it + 1; ; it2++){
                if(it2 == activities_sorted.cend() || it->finish <= it2->start){
                    put_max_subsequence(std::vector<Activity>(it2, activities_sorted.cend()), max_seq, buff_cpy);
                    break;
                }
            }
        }
    }
}

std::vector<Activity> get_max_activities_brute(const std::vector<Activity> &activities) {
    std::vector<Activity> activities_sorted(activities), max_seq;
    std::sort(activities_sorted.begin(), activities_sorted.end(), [&](auto act1, auto act2){return act1 < act2;});

    put_max_subsequence(activities_sorted, max_seq);

    return max_seq;
}
