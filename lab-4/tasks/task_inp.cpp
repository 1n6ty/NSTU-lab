#include <iostream>
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

int main(){

    std::vector<std::string> input = gain_input();

    std::cout << "Gain strings:\n";
    for(unsigned int i = 0; i < input.size(); i++){
        std::cout << i << ". " << *(input.begin() + i) << '\n';
    }

    return 0;
}