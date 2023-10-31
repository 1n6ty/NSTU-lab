#include <iostream>
#include <string>
#include <assert.h>

char check_symmetry(std::string& str){
    const int half_len = (str.length() / 2);
    for(int i = 0; i < half_len; i++){
        if(*(str.begin() + i) != *(str.rbegin() + i)) return 0;
    }
    return 1;
}

int main(){

    std::string unit_1, unit_2, unit_3, unit_4, unit_5, unit_6;

    // unit 1
    unit_1 = "abba";
    assert(check_symmetry(unit_1) == 1 && "unit_1: even length");

    // unit 2
    unit_2 = "aba";
    assert(check_symmetry(unit_2) == 1 && "unit_2: odd length");

    // unit 3
    unit_3 = "";
    assert(check_symmetry(unit_3) == 1 && "unit_3: empty string");

    // unit 4
    unit_4 = "a";
    assert(check_symmetry(unit_4) == 1 && "unit_4: single character");

    // unit 5
    unit_5 = "Hello world";
    assert(check_symmetry(unit_5) == 0 && "unit_5: assymmetric string");

    // unit 6
    unit_6 = "!!..!!";
    assert(check_symmetry(unit_6) == 1 && "unit_6: specific characters");

    std::cout << "All tests passed!" << '\n';

    return 0;
}