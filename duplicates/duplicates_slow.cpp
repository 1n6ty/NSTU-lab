#include "duplicates.h"
#include <algorithm>

// Implements slow duplicates search O(n^2)
// Just by bruteforce

bool has_duplicates_slow(const std::vector<int> &data) {
    for(auto it = data.begin(); it != data.end(); it++){
        for(auto it1 = it + 1; it1 != data.end(); it1++){
            if(*it == *it1) return true;
        }   
    }
    return false;
}


std::vector<int> get_duplicates_slow(const std::vector<int> &data) {
    std::vector<int> result;

    for(auto it = data.begin(); it != data.end(); it++){
        for(auto it1 = it + 1; it1 != data.end(); it1++){
            if(*it == *it1){
                if(std::find(result.begin(), result.end(), *it) == result.end()) result.push_back(*it);
                break;
            }
        }   
    }

    return result;
}
