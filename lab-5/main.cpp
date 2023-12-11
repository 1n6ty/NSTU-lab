#include <iostream>

#define INIT_CAP 16

double* sort_array(double *arr, size_t start_pos, size_t end_pos){ // standart quick-sort algorithm (middle pick)
    if(end_pos == 1) return arr;

    size_t mid = (end_pos - start_pos) / 2;
    double mid_e = arr[mid];

    size_t arr_b_len = 0, arr_a_len = 0, arr_b_cap = INIT_CAP, arr_a_cap = INIT_CAP;
    double *arr_b = (double*) malloc(arr_b_cap * sizeof(double)), *arr_a = (double*) malloc(arr_b_cap * sizeof(double));

    for(size_t i = start_pos; i < end_pos; i++){
        if(arr[i] <= mid_e && i != mid){
            arr_b[arr_b_len] = arr[i];
            arr_b_len ++;
        } else if(arr[i] > mid_e && i != mid){
            arr_a[arr_a_len] = arr[i];
            arr_a_len ++;
        }

        if(arr_a_cap == arr_a_len){
            arr_a_cap *= 2;
            arr_a = (double*) realloc(arr_a, arr_a_cap * sizeof(double));
            
            if(arr_a == NULL) return NULL;
        }
        if(arr_b_cap == arr_b_len){
            arr_b_cap *= 2;
            arr_b = (double*) realloc(arr_b, arr_b_cap * sizeof(double));

            if(arr_b == NULL) return NULL;
        }
    }

    if(arr_a_len != 0) arr_a = sort_array(arr_a, 0, arr_a_len);
    if(arr_a == NULL) return NULL;

    if(arr_b_len != 0) arr_b = sort_array(arr_b, 0, arr_b_len);
    if(arr_b == NULL) return NULL;

    for(size_t i = 0; i < arr_b_len; i++) arr[i] = arr_b[i];
    arr[arr_b_len] = mid_e;
    for(size_t i = 0; i < arr_a_len; i++) arr[arr_b_len + 1 + i] = arr_a[i];

    free(arr_a);
    free(arr_b);

    return arr;
}

int main(){
    size_t N;
    std::cin >> N;

    double *arr = (double*) malloc(N * sizeof(double)), buff;
    for(size_t i = 0; i < N; i++){
        std::cin >> buff;
        arr[i] = buff;
    }

    arr = sort_array(arr, 0, N);
    if(arr == NULL) std::cout << "error\n";

    for(size_t i = 0; i < N; i++) std::cout << arr[i] << ' ';
    std::cout << '\n';

    return EXIT_SUCCESS;
}