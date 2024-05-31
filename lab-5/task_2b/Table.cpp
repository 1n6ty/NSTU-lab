#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include "Table.h"

Node::Node(float f_data, size_t u_data, char *s, Node *next){ // Node constructor
    this->f_data = f_data;
    this->u_data = u_data;

    this->next = next;
    this->s = (char *) malloc(strlen(s));
    if(this->s != NULL) strcpy(this->s, s); // if no malloc error
}

Node::~Node(){ // Node destructor
    Node *buff;
    while(this->next != NULL){
        buff = this->next->next;
        free(this->next->s);
        free(this->next);
        this->next = buff;
    }
}

Table::Table(size_t capacity){ // Table constructor
    this->capacity = capacity;
    this->size = 0;

    head = (Node *) malloc(sizeof(Node) * capacity);
}

Table::~Table(){ // Table destructor
    for(size_t i = 0; i < this->size; i++){
        free((this->head + i)->s);
        (this->head + i)->~Node();
    }
    free(this->head);
}

static char reTable(Table *table){ // realloc table content to 2*capacity space
    table->capacity *= 2;
    table->head = (Node *) realloc(table->head, sizeof(Node) * table->capacity);

    return table->head == NULL;
}

char Table::push(float f_data, size_t u_data, char *s){
    this->size ++;
    if(this->size > this->capacity && reTable(this)) return 1; // Check overflow and realloc if it occurs (with) error check

    head[this->size - 1] = Node(f_data, u_data, s, NULL);
    if(head->s == NULL) return 2; // check string copy error

    return EXIT_SUCCESS;
}

static void shift_back(Table *table, size_t i){ // Shift content back starting from i-th index
    for( ; i < table->size; i++) table->head[i - 1] = table->head[i];
    table->size --;
}

char Table::del(size_t u_data){ // delete element from table
    for(size_t i = 0; i < this->size; i++){
        if(this->head[i].u_data == u_data){
            free(this->head[i].s);
            shift_back(this, i + 1);
            this->head[this->size].~Node();
            break;
        }
    }

    return EXIT_SUCCESS;
}

static void swap(Node *fst, Node *sec){
    Node tmp = *fst;
    *fst = *sec;
    *sec = tmp;
}

void Table::sort(int &check, int &changes){ // sorting the table content
    check = 0;
    changes = 0;

    Node *buff;
    for(size_t i = 0; i < this->size - 1; i++){
        for(size_t j = i + 1; j < this->size; j++){
            check ++;
            if(this->head[i].u_data > this->head[j].u_data){
                swap(this->head + i, this->head + j);
                changes ++;
            } else if(this->head[i].u_data == this->head[j].u_data){
                buff = this->head + i;
                while(buff->next != NULL) buff = buff->next;
                buff->next = new Node(this->head[j].f_data, this->head[j].u_data, this->head[j].s, this->head[j].next);
                
                free(this->head[j].s);
                shift_back(this, j + 1);
                this->head[this->size].~Node();
            }
        }
    }
}

void print_table(Table *table){
    Node *buff;
    for(size_t i = 0; i < table->size; i++) {
        printf("{ ");
        buff = table->head + i;
        while (buff != NULL){
            printf("(%lu, %f, %s) ", buff->u_data, buff->f_data, buff->s);
            buff = buff->next;
        }
        printf("} ");
    }
    printf("\n");
}
