#include <iostream>
#include <string>

class BigNumber{
    public:
        std::string num = "";

        template <typename T> BigNumber(T num){
            this->num = std::to_string(num);
        }

        BigNumber operator*=(BigNumber const &add){
            
            if(this->num.length() < add.num.length()){

            }
        }
};

int main(){

    


    return 0;
}