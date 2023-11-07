#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <math.h>
#include <vector>
#include <string>

std::vector<std::string> gain_input(){
    std::vector<std::string> out;

    out.push_back(std::string());

    char sym;
    while(EOF != (sym = fgetc(stdin)) && sym != '.'){
        if(sym == ' '){
            out.push_back(std::string());
        } else {
            *out.rbegin() += sym;
        }
    }

    for(std::vector<std::string>::iterator it = out.begin(); it != out.end() - 1;){
        if(*it == *out.rbegin()) it = out.erase(it);
        else it ++;
    }

    return out;
}

char check_symmetry(std::string& str){
    const int half_len = (str.length() / 2);
    for(int i = 0; i < half_len; i++){
        if(*(str.begin() + i) != *(str.rbegin() + i)) return 0;
    }
    return 1;
}

int main(){
    int v;

    std::cout << "Введите 1, если хотите найти симметричные слова в последовательности. \n"
                << "Введите 2, если хотите найти слова, в котрых первая буква входит в слово еще раз. \n"
                << "Введите 3, если хотите найти слова, в котрых нет повторяющихся букв. \n"
                << "Введите 4, если хотите найти слова, в котрых каждая буква входит в слово не менее двух раз. \n";
    std::cin >> v;

    std::cout << "Введите непустую последовательность слов (не более 50), в каждом слове не более восьми символов. Слова разделены пробелом, за по-следним словом точка. \n";
    std::vector<std::string> input = gain_input();

    if(v == 1){
        for(std::string word : input){
            if(check_symmetry(word)) std::cout << word;
        }
        
    }else if (v == 2){
        
    }else if (v == 3){
        
    }else if (v == 4){

    }
    
    
    
    return 0;
      
}