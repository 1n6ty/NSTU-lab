#include <iostream>
#include <algorithm>
#include <io.h>

int main(){

    _setmode(_fileno(stdout), 0x00020000);
    long double a, b, c, d;

    std::wcout << L"Введите 4 числа через пробел" << '\n';
    std::cin >> a >> b >> c >> d;

    if(a <= b && b <= c && c <= d){
        a = d;
        b = d;
        c = d;
    }
    else if(a > b && b > c && c > d){}
    else {
        a *= a;
        b *= b;
        c *= c;
        d *= d;
    }

    std::wcout << a << ' ' << b << ' ' << c << ' ' << d << std::endl;

    return 0;
}