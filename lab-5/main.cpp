#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <math.h>

int sym(int x[], int y[]){
    int summa = 0;
    for (int k = 0; k < 20; k++){
        summa += x[k]*y[k];
    }
    
    if(summa > 0) return 1;
    else return 0;
}

int main(){
    _setmode(_fileno(stdout), 0x00020000);

    int x[20];
    int y[20];
    int s1 = 0;


    std::wcout << L"Введите 20целых чисел для массива х. \n";
    for (int i = 0; i < 20; i++){
        std::cin >> x[i];
    }

    std::wcout << L"Введите 20 целых чисел для массива y. \n";
    for (int j = 0; j < 20; j++){
        std::cin >> y[j];
    }

    if(sym(x, y)){
        for (int r = 0; r < 20; r++){
            s1 += x[r] * x[r];
        }
    }else{
        for (int t = 0; t < 20; t++){
            s1 += y[t] * y[t];
        }
    }

    std::wcout << s1;
    
    return 0;
}