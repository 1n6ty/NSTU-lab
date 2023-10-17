#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <math.h>

int main(){
    _setmode(_fileno(stdout), 0x00020000);

    int n;
    int result1 = 1;
    int result2 = 1;

    std::wcout << L"Введите натуральное число n\n";
    std::wcin >> n;

    for (int i = 1; i <= n; i++){
        result1 *= i;
    }

    for (int j = 1; j <= result1; j++){
        result2 *= j;
    }

    std::wcout << result2;

}