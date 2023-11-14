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

    std::cout << "Type 1, if you want to find symmetric words. \n"
                << "Type 2, if you want to find words where the first letter occurs one more time. \n"
                << "Type 3, if you want to find words where no letter repeats. \n"
                << "Type 4, if you want to find words where each letter occurs not less than twice. \n";
    std::cin >> v;

    std::wcout << "Input sequence: \n";
    std::vector<std::string> input = gain_input();

    if(v == 1){        
        for(std::string& word : input){
            if(check_symmetry(word)) std::cout << word << " ";
        }
        
    }else if (v == 2){
        
    }else if (v == 3){
        
    }else if (v == 4){

    }
    
    
    
    return 0;
      
}