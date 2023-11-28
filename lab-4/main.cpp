#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <math.h>
#include <cstring>

const int MAX_LENGTH = 40;

int main(){
    _setmode(_fileno(stdout), 0x00020000);

    std::string str;
    char d;
    int length = 0;

    std::wcout << L"Введите последовательность не более чем из 40 строчных латинских букв, за которой следуют пробелы. \n";

    while (length < MAX_LENGTH && std::cin.get(d) && d != '\n') {
        if (d >= 'a' && d <= 'z') {
            str += d;
        } else {
            std::wcout << L"Введены некорректные символы. \n";
            return 0;
        }
    }

    if(str.find("abc") != std::string::npos){
        str.replace(str.find("abc"), 3, "def");
    }
    
    std::cin >> str;

    return 0;
}
