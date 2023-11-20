#include <iostream>
#include <string>
#include <map>
#include <assert.h>

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

    
    std::string unit_1, unit_2, unit_3, unit_4, unit_5;

    // unit 1
    unit_1 = "abba";
    assert(is_letter_occured_me2(unit_1) == 1 && "unit_1: even length");

    // unit 2
    unit_2 = "aba";
    assert(is_letter_occured_me2(unit_2) == 0 && "unit_2: odd length");

    // unit 3
    unit_3 = "";
    assert(is_letter_occured_me2(unit_3) == 0 && "unit_3: empty string");

    // unit 4
    unit_4 = "a";
    assert(is_letter_occured_me2(unit_4) == 0 && "unit_4: single character");

    // unit 5
    unit_5 = "!!..!!";
    assert(is_letter_occured_me2(unit_5) == 1 && "unit_5: specific characters");

    std::cout << "All tests passed!" << '\n';

    return 0;
}