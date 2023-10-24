#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <math.h>

void sdvig(int *arr, int index, size_t N){
    for(int i = index - 1; i < N; i++){
        arr[i] = arr[i + 1];
    }
}

//попробовать словарь(map)

int main(){
    _setmode(_fileno(stdout), 0x00020000);

    int N;
    int counter;
    int al;
    
    std::wcout << L"Введите натурально число N\n";
    std::cin >> N;
    std::wcout << L"введите N чисел\n";

    float *arr = new float[N];
    for (int i = 0; i < N ; i++){
        std::cin>>arr[i];
    } 

    for (int j = 0; j < N; j++){
        counter = 0;
        for (int k = 0; k < N; k++){
            if(arr[j] == arr[k]){
                counter++;
            }
        }
        if(counter < 3){
            int rep = arr[j];
            for(int r = 0; r < N; r++){
                if(rep == arr[r]){
                    delete &arr[r];
                }
            }
            al = sizeof(arr) / sizeof(arr[0]);
        }
    }

    for(int d = 0; d < al; d++){
        std::cin >> arr[d];
    }
}