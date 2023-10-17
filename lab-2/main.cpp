#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <math.h>

int main(){
    _setmode(_fileno(stdout), 0x00020000);

    int n;
    long long result = 1;

    std::wcout << L"Введите натуральное число n\n";
    std::wcin >> n;

    if(n > 0){
        if(n%2==0){
            for (int i = 2; i <= n ; i += 2){
                result *= i;
            }
        }else{
             for (int i = 1; i <= n; i += 2){
                result *= i;
            }           
        }
    }else{
        std::wcout << L"Введённое число n не является натуральным\n";
        return 0;
    }

    std::wcout << result;

    return 0;

}