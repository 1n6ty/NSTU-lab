#include <iostream>
#include <io.h>
#include <assert.h>
#include <iostream>

void RemoveX(int A[], int& N, int X) {
    int i = 0;
    int j = 0;
    
    while (i < N) {
        if (A[i] != X) {
            A[j] = A[i];
            j++;
        }
        i++;
    }
    
    N = j;
}

void Inp(int arr[], int l){
    for (int i = 0; i < l; i++){
        std::cin >> arr[i];
    }
}

void Outp(int arr[], int l, int X){
    if(l != 0){
        for (int i = 0; i < l; i++) {
        std::wcout << arr[i] << " ";
        }
    }else std::wcout << L"Массив пуст";

    
}

int main() {
    _setmode(_fileno(stdout), 0x00020000);

    int Na;
    int Nb;
    int Nc;
    int Xa;
    int Xb;
    int Xc;

    std::wcout << L"Введите размеры массивов A, В и С через пробел: ";
    std::cin >> Na >> Nb >> Nc;
    int A[Na];
    int B[Nb];
    int C[Nc];
    
    std::wcout << L"Введите элементы массива A:\n";
    Inp(A, Na);
    std::wcout << L"Введите элементы массива В:\n";
    Inp(B, Nb);
    std::wcout << L"Введите элементы массива C:\n";
    Inp(C, Nc);
    
    std::wcout << L"Введите число, которое необходимо удалить из массива A: ";
    std::cin >> Xa;
    std::wcout << L"Введите число, которое необходимо удалить из массива B: ";
    std::cin >> Xb;
    std::wcout << L"Введите число, которое необходимо удалить из массива C: ";
    std::cin >> Xc;
    
    RemoveX(A, Na, Xa);
    RemoveX(B, Nb, Xb);
    RemoveX(C, Nc, Xc);
    
    std::wcout << L"\nМассив A после удаления элементов со значением " << Xa << ": ";
    Outp(A, Na, Xa);
    std::wcout << L"\nРазмер массива A после удаления элементов со значением " << Xa << ": " << Na;

    std::wcout << L"\nМассив B после удаления элементов со значением " << Xb << ": ";
    Outp(B, Nb, Xb);
    std::wcout << L"\nРазмер массива A после удаления элементов со значением " << Xb << ": " << Nb;

    std::wcout << L"\nМассив C после удаления элементов со значением " << Xc << ": ";
    Outp(C, Nc, Xc);
    std::wcout << L"\nРазмер массива A после удаления элементов со значением " << Xc << ": " << Nc;
    
    return 0;
}
