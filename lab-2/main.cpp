#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <math.h>

float sin_calc(long int n, float x){
    float buff = x;
    for(int i = 0; i < n; i++){
        buff = std::sin(buff);
    }
    return buff;
}

int main(){
    _setmode(_fileno(stdout), 0x00020000);

    int v;
    int n;
    float x;

    std::wcout << L"Для вычисления sin(x)+sin**2(x)+...+sin**n(x) введите 1." << '\n';
    std::wcout << L"Для вычисления sin(x)+sin(x**2)+...+sin(x**n) введите 2." << '\n';
    std::wcout << L"Для вычисления sin(x)+sin(sin(x))+...+sin(sin(...(sin(x)))) введите 3." << '\n';
    std::cin >> v;

    std::wcout << L"Введите натуральное число n и действительное число x" << '\n';
    std::cin >> n >> x;

    if(n>0){
        float counter = 0;
        if(v == 1){    
            for (int i = 1; i <= n; i++){
                counter += (float) std::pow((float) std::sin(x), i);
            }
        }else if (v == 2){
            for (int i = 1; i <= n; i++){
                counter += (float) std::sin(std::pow(x, i));
            }
        }else if (v == 3){
            for(int i = 1; i <= n; i++){
                counter += (float) sin_calc(i, x);
            }
        }
        std::wcout << counter << std::endl; 
    }else std::wcout << L"Введённое число n не является натуральным" << std::endl;
    
    return 0;
}