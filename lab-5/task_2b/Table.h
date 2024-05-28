#include <stdlib.h>

#ifndef TABLE_H
#define TABLE_H

typedef struct Node{
    float f_data;
    size_t u_data;
    char *s;

    Node *next;

    Node(float f_data, size_t u_data, char *s, Node *next);
    ~Node();
} Node;

typedef struct Table{
    Node *head;
    size_t size, capacity;

    Table(size_t capacity);
    ~Table();
    char push(float f_data, size_t u_data, char *s);
    char del(size_t u_data);
    void sort(int &checks, int &changes);
} Table;

void print_table(Table *table);

#endif
