#include <iostream>
#include <map>
#include <cassert>

void shift_back(int *arr, int pos, size_t s){
    if(pos == 0) return;
    for(int i = pos - 1; i < s; i++){
        *(arr + i) = *(arr + i + 1);
    }
}

size_t clear_duplicates_rev(int *arr, size_t s){
    std::map<int, int> dupl;
    for(int i = s - 1; i >= 0; i--){
        if(dupl.count(*(arr + i))){
            shift_back(arr, i + 1, s);
            s --;
        } else{
            dupl[*(arr + i)] = 1;
        }
    }

    return s;
}

char compare(int *arr_1, int *arr_2, size_t s){
    for(int i = 0; i < s; i++){
        if(*(arr_1 + i) != *(arr_2 + i)) return 0;
    }
    return 1;
}

int main(){

    size_t s;
    


    std::cout << "unit-test 1:\n";

    int arr_1[] = {0, 0, 0, 0}, answ_1[] = {0};
    s = 4;

    for(int i = 0; i < s; i++) std::cout << arr_1[i] << ' ';
    std::cout << "<- initial\n";

    s = clear_duplicates_rev(arr_1, s);

    for(int i = 0; i < s; i++) std::cout << arr_1[i] << ' ';
    std::cout << "<- after clearing duplicates\n";

    assert(s == 1 && compare(arr_1, answ_1, s) && "{0, 0, 0, 0} -> {0}");



    std::cout << "unit-test 2:\n";

    int arr_2[] = {-1, 8, 4, 4, -1, 8, 9, 9}, answ_2[] = {4, -1, 8, 9};
    s = 8;

    for(int i = 0; i < s; i++) std::cout << arr_2[i] << ' ';
    std::cout << "<- initial\n";

    s = clear_duplicates_rev(arr_2, s);

    for(int i = 0; i < s; i++) std::cout << arr_2[i] << ' ';
    std::cout << "<- after clearing duplicates\n";

    assert(s == 4 && compare(arr_2, answ_2, s) && "{-1, 8, 4, 4, -1, 8, 9, 9} -> {4, -1, 8, 9}");

    return 0;
}