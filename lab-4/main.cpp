#include <iostream>

int main(){

    char *str = (char*) calloc(40, sizeof(char));
    char sym;

    for(unsigned char i = 0; EOF != (sym = fgetc(stdin)) && sym != '\n' && sym != ' '; i++){
        str[i] = sym;
    }

    for(unsigned char i = 0; i < 37; i++){
        if(str[i] == 'a' && str[i + 1] == 'b' && str[i + 2] == 'c'){
            str[i] = 'd', str[i + 1] = 'e', str[i + 2] = 'f';
            i += 2;
        }
    }

    printf("%s", str);

    return EXIT_SUCCESS;
}