#include <iostream>
#include <cstring>

const unsigned int cap = 5000, str_cap = 1000;

typedef struct TableItem {
    unsigned int key;
    float f_data;
    char str_data[str_cap];

    TableItem *next;
} TableItem;

typedef struct HashTable {
    TableItem **items;
    unsigned int size;

    static HashTable *create(unsigned int size);
    unsigned int HashFunc(unsigned int key);
    char insert(unsigned int key, float f_data, char *str_data);
    TableItem *get(unsigned int key);
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

unsigned int HashTable::HashFunc(unsigned int key){
    return key % this->size;
}

char HashTable::insert(unsigned int key, float f_data, char *str_data){
    unsigned int hash = this->HashFunc(key);

    TableItem* item = new TableItem();
    item->key = key;
    item->f_data = f_data;
    item->next = NULL;
    strcpy(item->str_data, str_data);

    if(this->items[hash] != NULL){ // Collision
        TableItem *buff = this->items[hash];
        while(buff->next != NULL) buff = buff->next;

        buff->next = item;
    } else { 
        this->items[hash] = item;
    }

    return 0;
}

TableItem *HashTable::get(unsigned int key){
    unsigned int hash = this->HashFunc(key);

    return (this->items[hash] != NULL) ? this->items[hash]: NULL;
}

char HashTable::del(unsigned int key){
    unsigned int hash = this->HashFunc(key);

    if(this->items[hash] != NULL){
        TableItem *buff = this->items[hash];
        while(buff != NULL){
            free(buff);
            buff = buff->next;
        }
        this->items[hash] = NULL;

        return 0;
    }

    return 1;
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

    char inp_buff, str_buff[str_cap];
    float f_buff;
    unsigned int uint_buff;

    TableItem *item;

    while(1){
        std::cout << "Enter 1 - to insert, 2 - to get, 3 - to delete, 4 - to exit\n";
        std::cin >> inp_buff;
        if(inp_buff == '1'){
            std::cout << "Enter (key, float value and string) in format \"\%d \%f \%10s\":\n";
            if(scanf("%u %f %s", &uint_buff, &f_buff, str_buff) == 3 && strlen(str_buff) <= 10){
                table->insert(uint_buff, f_buff, str_buff);
            } else {
                std::cout << "Wrong format\n";
            }
        } else if(inp_buff == '2'){
            std::cout << "Enter key:\n";
            std::cin >> uint_buff;

            item = table->get(uint_buff);
            while (item != NULL){
                printf("%u: %f | %s -> ", uint_buff, item->f_data, item->str_data);
                item = item->next;
            }
            std::cout << "NULL\n";
            
        } else if(inp_buff == '3'){
            std::cout << "Enter key:\n";
            std::cin >> uint_buff;

            table->del(uint_buff);
        } else if(inp_buff == '4'){
            break;
        }
    }

    table->freeTable();

    return 0;
}