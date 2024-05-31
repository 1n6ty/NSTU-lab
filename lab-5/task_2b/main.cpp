#include "Table.h"
#include <iostream>

int main(){
    const size_t cap = 100;
    Table *table = new Table(cap);

    char inp_buff, *s_buff = (char *) malloc(10);
    float f_buff;
    size_t uint_buff;

    Node *item;
    int changes, checks;        

    while(1){
        std::cout << "Enter 1 - to insert, 2 - to delete, 3 - to exit\n";
        std::cin >> inp_buff;
        if(inp_buff == '1'){
            std::cout << "Enter (key, fvalue, string) in format \"\%d \%f \%10s:\n";
            if(scanf("%lu %f %10s", &uint_buff, &f_buff, s_buff) == 3){
                table->push(f_buff, uint_buff, s_buff);
            } else {
                std::cout << "Wrong format\n";
            }
        } else if(inp_buff == '2'){
            std::cout << "Enter key:\n";
            std::cin >> uint_buff;

            table->del(uint_buff);
        } else if(inp_buff == '3'){
            break;
        }
        table->sort(checks, changes);
        print_table(table);
        printf("%d checks and %d changes\n", checks, changes);
    }

    table->~Table();

    return 0;
}