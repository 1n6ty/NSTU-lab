#include "sort.h"

using namespace std;

void sort(vector<int> &data) {
    size_t d_size = data.size();

    if(d_size == 2 && data[0] > data[1]){
        data[0] += data[1];
        data[1] = data[0] - data[1];
        data[0] -= data[1];
    }
    if(d_size <= 2) return;

    int mid_el = data.at(0);
    vector<int> before_mid, after_mid;
    for(auto it = data.begin() + 1; it != data.end(); it++){
        if(*it <= mid_el) before_mid.push_back(*it);
        else after_mid.push_back(*it);
    }

    sort(before_mid);
    sort(after_mid);

    before_mid.push_back(mid_el);
    before_mid.insert(before_mid.end(), after_mid.begin(), after_mid.end());

    data = before_mid;
}
