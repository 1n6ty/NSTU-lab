#include <iostream>
#include <io.h>
#include <map>
#include <math.h>
#include <assert.h>

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
    
    /*for (int i = 0; i < str.size(); i++){
        if(count(str.begin(), str.end(), str[i]) = 1){
            c++;
        }
    }*/
     
}

int main(){
    std::string un1, un2, un3, un4;

    un1 = "asdf";
    assert(not_double(un1) == 1);

    un2 = "asyfhalasy";
    assert(not_double(un2) == 0);

    un3 = "djaishajb";
    assert(not_double(un3) == 1);

    un4 = "fhjsl";
    assert(not_double(un4) == 0);
    
    //std::cout << "All passed!" << '\n';
}