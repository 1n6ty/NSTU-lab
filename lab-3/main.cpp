#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <math.h>

int main(){
    _setmode(_fileno(stdout), 0x00020000);

    int n;

    std::wcout<< L"Введите натуральное чсило n\n";
    std::cin>> n;
    std::wcout<< L"Введите n действительных чисел\n";

    float *arr = new float[n];
    for (int i = 0; i < n ; i++){
        std::cin>>arr[i];
    }

    for (int j = 0; j < n - 1; j++){
        if(arr[j] < arr[j+1]){
            std::wcout<< L"Последовательность не упорядочена по убыванию";
            return 0;
        }
    }
    
    std::wcout<< L"Последовательность упорядочена по убыванию";
    return 0;
}