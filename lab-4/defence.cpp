#include <iostream>
#include <math.h>
#include <vector>

const unsigned int N = 1000, cap = 10000;
const float A = (std::sqrt(5) - 1) / 2;

typedef struct TableItem {
    unsigned int key;
    float f_data;
} TableItem;

typedef struct HashTable {
    TableItem **items;
    unsigned int size;

    static HashTable *create(unsigned int size);
    unsigned int HashFunc(unsigned int key, unsigned int i);
    char insert(unsigned int key, float f_data);
    TableItem *get(unsigned int key);
    std::vector<TableItem *> searchByFVal(float f_data);
    char del(unsigned int key);
    char freeTable();
} HashTable;

HashTable *HashTable::create(unsigned int size){
    HashTable *table = new HashTable();
    table->size = size;
    table->items = (TableItem **) malloc(sizeof(TableItem*) * size);
    for(unsigned int i = 0; i < size; i++) table->items[i] = NULL;

    return table;
}

unsigned int HashTable::HashFunc(unsigned int key, unsigned int i = 0){
    if(i){
        return (N * (unsigned int) (key * A) + i) % N;
    }
    return N * (unsigned int) (key * A);
}

char HashTable::insert(unsigned int key, float f_data){
    unsigned int hash = this->HashFunc(key);

    TableItem* item = new TableItem();
    item->key = key;
    item->f_data = f_data;

    unsigned int i = 1;
    while(this->items[hash] != NULL){
        hash = this->HashFunc(key, i);
        i ++;
    }
    this->items[hash] = item;

    return 0;
}

TableItem *HashTable::get(unsigned int key){
    unsigned int hash = this->HashFunc(key);

    return this->items[hash];
}

char HashTable::del(unsigned int key){
    unsigned int hash = this->HashFunc(key);

    if(this->items[hash] != NULL){
        free(this->items[hash]);
        this->items[hash] = NULL;

        return 0;
    }

    return 1;
}

std::vector<TableItem *> HashTable::searchByFVal(float f_data){
    std::vector<TableItem *> response;
    for(unsigned int i = 0; i < this->size; i++){
        if(this->items[i] != NULL && this->items[i]->f_data == f_data) response.push_back(this->items[i]); 
    }

    return response;
}

char HashTable::freeTable(){
    for(unsigned int i = 0; i < this->size; i++){
        free(this->items[i]);
    }
    free(this->items);
    free(this);

    return 0;
}

int main(){

    HashTable *table = HashTable::create(cap);

    char inp_buff;
    float f_buff;
    unsigned int uint_buff;

    TableItem *item;

    // Start reading file
    FILE *table_txt = fopen("table.txt", "r");
    if(!table_txt){
        std::cout << "File error\n";
    } else {
        while(fscanf(table_txt, "%u %f", &uint_buff, &f_buff) != EOF){
            table->insert(uint_buff, f_buff);
        }
    }
    fclose(table_txt);

    while(1){
        std::cout << "Enter 1 - to insert, 2 - to get, 3 - to delete, 4 - to search for element by fvalue, 5 - to exit\n";
        std::cin >> inp_buff;
        if(inp_buff == '1'){
            std::cout << "Enter (key and fvalue) in format \"\%d \%f:\n";
            if(scanf("%u %f", &uint_buff, &f_buff)){
                table->insert(uint_buff, f_buff);
            } else {
                std::cout << "Wrong format\n";
            }
        } else if(inp_buff == '2'){
            std::cout << "Enter key:\n";
            std::cin >> uint_buff;

            item = table->get(uint_buff);
            if(item != NULL) printf("%u: %f\n", item->key, item->f_data);
            else printf("%s\n", "NULL");
            
        } else if(inp_buff == '3'){
            std::cout << "Enter key:\n";
            std::cin >> uint_buff;

            table->del(uint_buff);
        } else if(inp_buff == '4'){
            std::cout << "Enter fvalue:\n";
            std::cin >> f_buff;

            for(TableItem *i: table->searchByFVal(f_buff)){
                std::cout << i->key << ": " << i->f_data << " | ";
            }
            std::cout << '\n';
        }else if(inp_buff == '5'){
            break;
        }
    }

    table->freeTable();

    return 0;
}