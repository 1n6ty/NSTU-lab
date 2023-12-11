#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <math.h>
#include <cstring>

int main(){

    std::string str;
    char d;
    int length = 0;

    std::cout << "Enter a sequence of no more than 40 lowercase Latin letters followed by spaces. \n";

    while (length < 40 && std::cin.get(d) && d != '\n') {
        if (d >= 'a' && d <= 'z') {
            str += d;
        } else {
            std::cout << "Incorrect characters have been entered. \n";
            return 0;
        }
        length++;
    }

    if(str.find("abc") != std::string::npos){
        str.replace(str.find("abc"), 3, "def");
    }
    
    std::cout << str;

    return 0;
}
