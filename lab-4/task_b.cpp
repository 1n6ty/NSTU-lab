#include <iostream>
#include <string>
#include <map>
#include <assert.h>

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

int main(){

    std::string unit_1, unit_2, unit_3, unit_4;

    // unit 1
    unit_1 = "arcane";
    assert(is_first_letter_doubled(unit_1) == 1 && "Just a true word");

    // unit 2
    unit_2 = "attribute";
    assert(is_first_letter_doubled(unit_2) == 0 && "No first-letter doublicates");

    // unit 3
    unit_3 = "";
    assert(is_first_letter_doubled(unit_3) == 0 && "Empty string");

    // unit 4
    unit_4 = "a";
    assert(is_first_letter_doubled(unit_4) == 0 && "Single character");

    std::cout << "All tests passed!" << '\n';

    return 0;
}