#include "duplicates.h"
#include <unordered_map>

// Search for duplicates using hash table

bool has_duplicates_fast(const std::vector<int> &data) {
    std::unordered_map<int, int> m;

    for(auto it = data.begin(); it != data.end(); it++){
        if(m[*it] != 0) return true;
        else m[*it] ++;
    }

    return false;
}


std::vector<int> get_duplicates_fast(const std::vector<int> &data) {
    std::vector<int> result;
    std::unordered_map<int, int> m;

    for(auto it = data.begin(); it != data.end(); it++) m[*it] ++;
    for(auto it = m.begin(); it != m.end(); it++) if(it->second > 1) result.push_back(it->first);

    return result;
}
