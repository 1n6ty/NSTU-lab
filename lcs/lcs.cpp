#include "lcs.h"
#include <vector>
#include <algorithm>

using namespace std;

string lcs_compute(const string &first, const string &second, int i, int j, vector<vector<string>> &cache){
    if(i == -1 || j == -1) return "";
    
    if(cache[i][j] != "\0") return cache[i][j];

    if(first[i] == second[j]){
        cache[i][j] = lcs_compute(first, second, i - 1, j - 1, cache) + first[i];
        return cache[i][j];
    } else {
        string buff_i = lcs_compute(first, second, i - 1, j, cache), buff_j = lcs_compute(first, second, i, j - 1, cache);
        size_t buff_i_len = buff_i.length(), buff_j_len = buff_j.length();
        if(buff_i_len < buff_j_len || ((buff_i_len == buff_j_len) && buff_i < buff_j)){
            cache[i][j] = buff_j;
            return buff_j;
        } else {
            cache[i][j] = buff_i;
            return buff_i;
        }
    }
}

string lcs(const string &first, const string &second) {
    vector<vector<string>> cache(first.length(), vector<string>(second.length(), "\0"));
    
    return lcs_compute(first, second, first.length() - 1, second.length() - 1, cache);
}

void fill_set_with_subsequences(const string str, vector<string> &vec, string buff = ""){
    if(str.length() == 0){
        vec.push_back(buff);
    } else {
        for(auto it = str.cbegin(); it != str.cend(); it++){
            vec.push_back(buff + *it);
            fill_set_with_subsequences(str.substr(it - str.cbegin() + 1, str.cend() - it - 1), vec, buff + *it);
        }
        fill_set_with_subsequences("", vec, buff);
    }
}

string lcs_brute(const string &first, const string &second) {
    vector<string> fst_seq, sec_seq;
    string result = "";
    fill_set_with_subsequences(first, fst_seq);
    fill_set_with_subsequences(second, sec_seq);

    vector<string>::const_iterator buff_it;
    for(auto it = fst_seq.cbegin(); it != fst_seq.cend(); it++){
        buff_it = std::find(sec_seq.begin(), sec_seq.end(), *it);
        if(buff_it != sec_seq.cend() && (result.length() < (*buff_it).length() || (result.length() == (*buff_it).length() && result < *buff_it))){
            result = *buff_it;
        }
    }

    return result;
}
