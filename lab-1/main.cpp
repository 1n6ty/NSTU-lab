#include <iostream>

namespace List{
    struct Node{
        bool fst;
        float *sec;
        Node *next;
    };

    char insert_node(List::Node *head, int ind, bool fst, float *sec){
        List::Node *buff_node = head;
        for(int i = 0; i <= ind; i++){
            if(buff_node->next != NULL){
                buff_node = buff_node->next;
            } else{
                return 0;
            }
        }

        List::Node *new_node = new List::Node();
        new_node->fst = fst;
        new_node->sec = sec;

        new_node->next = buff_node->next;
        buff_node->next = new_node;
        
        return 1;
    }

    char del_node(List::Node *head, int ind){
        List::Node *buff_node = head;
        for(int i = 0; i < ind; i++){
            if(buff_node->next != NULL){
                buff_node = buff_node->next;
            } else{
                return 0;
            }
        }

        if(buff_node->next != NULL){
            buff_node->next = buff_node->next->next;
        } else{
            return 0;
        }

        return 1;
    }

    char free_all(List::Node *head){

    }
}

int main(){

    float num1 = 2.7;
    
    List::Node *p = new List::Node();
    p->fst = false;
    p->sec = &num1;
    p->next = NULL;

    delete p;

    return 0;
}