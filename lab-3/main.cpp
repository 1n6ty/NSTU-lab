#include <iostream>
#include <algorithm>
#include <io.h>
#include <fcntl.h>
#include <math.h>

int main(){
    _setmode(_fileno(stdout), 0x00020000);

    int N;
    
    std::wcout << L"Введите размер массива N: ";
    std::cin >> N;

    int* array = new int[N];
    std::wcout << L"Введите элементы массива:\n";
    for (int i = 0; i < N; i++) {
        std::cin >> array[i];
    }

    for (int i = 0; i < N; i++) {
        int count = 0;
        for (int j = 0; j < N; j++) {
            if (array[j] == array[i]) {
                count++;
            }
        }

        if (count < 3) {
            for (int j = i; j < N - 1; j++) {
                array[j] = array[j + 1];
            }
            N--;
            i--;
        }
    }

    std::wcout << L"Размер полученного массива: " << N << std::endl;
    std::wcout << L"Содержимое полученного массива:";
    for (int i = 0; i < N; i++) {
        std::wcout << array[i] << " ";
    }
    return 0;
}