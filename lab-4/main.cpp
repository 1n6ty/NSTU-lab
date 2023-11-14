#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <math.h>
#include <vector>
#include <string>
#include <map>
#include <assert.h>

std::vector<std::string> gain_input(){
    std::vector<std::string> out;

    out.push_back(std::string());

    char sym;
    while(EOF != (sym = fgetc(stdin)) && sym != '.'){
        if(sym == ' '){
            if(out.rbegin()->length() < 8){
                out.push_back(std::string());
            } else {
                std::cout << "Word's length should be less than 8!\n";
                return {};
            }
        } else if(sym != '\n'){
            *out.rbegin() += sym;
        }
    }
    if(out.rbegin()->length() >= 8){
        std::cout << "Word's length should be less than 8!\n";
        return {};
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

char is_first_letter_doubled(std::string& str){
    if(str.length() == 0) return 0;
    std::map<char, unsigned int> count;

    std::size_t count_of_char;
    for(std::string::const_iterator it = str.begin() + 1; it != str.end(); it++){
        count_of_char = count.count(*it);
        if(count_of_char >= 2 && *it == *str.begin()) return 0;

        if(count_of_char != 0){
            count[*it] = 1;
        } else{
            count[*it] ++;
        }
    }

    if(count.count(*str.begin()) == 1) return 1;
    return 0;
}

char not_double(std::string& str){
    std::map<char, int> count;    

    for(char e: str){
        if(count.count(e) ){
            return 0;
        }else{
            count[e] = 1;
        }
    }
    return 1;
}

char is_letter_occured_me2(std::string& str){
    if(str.length() == 0) return 0;
    std::map<char, unsigned int> count;

    for(std::string::const_iterator it = str.begin(); it != str.end(); it++){
        if(count.count(*it) > 0){
            count[*it] ++;
        } else{
            count[*it] = 1;
        }
    }

    for(std::map<char, unsigned int>::const_iterator it = count.begin(); it != count.end(); it++){
        if(it->second < 2) return 0;
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

    if(input.size() == 0) return EXIT_FAILURE;

    if(v == 1){        
        for(std::string& word : input){
            if(check_symmetry(word)) std::cout << word <<" ";
        }
        
    }else if (v == 2){
        for(std::string& word : input){
            if(is_first_letter_doubled(word)) std::cout << word <<" ";
        }
        
    }else if (v == 3){
        for(std::string& word : input){
        if(not_double(word)) std::cout << word <<" ";
        }
        
    }else if (v == 4){
        for(std::string& word : input){
        if(is_letter_occured_me2(word)) std::cout << word <<" ";
        }

    }
    
    return 0;
      
}