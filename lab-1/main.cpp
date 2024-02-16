#include <iostream>

namespace List{
    struct Node{
        bool fst;
        float *sec;
        Node *next;
    };

    char insert_node(List::Node *head, int ind, bool fst, float *sec){
        List::Node *buff_node = head;
        for(int i = 0; i < ind - 1; i++){
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
        for(int i = 0; i < ind - 1; i++){
            if(buff_node->next != NULL){
                buff_node = buff_node->next;
            } else{
                return 0;
            }
        }

        List::Node *deleted = buff_node->next;
        if(buff_node->next != NULL){
            buff_node->next = buff_node->next->next;
        } else{
            return 0;
        }

        delete deleted;
        return 1;
    }

    char free_all(List::Node *head){
        List::Node *buff_node = head, *prev_buff_node = NULL;
        while(buff_node != NULL){
            prev_buff_node = buff_node;
            buff_node = buff_node->next;

            delete prev_buff_node;
        }
        return 1;
    }

    void print_list(List::Node *head){
        while(head != NULL){
            std::cout << "bool: " << head->fst << "; float: " << *(head->sec) << "; -> ";
            head = head->next;
        }
        std::cout << '\n';
    }
}

int main(){

    float num1 = 2.7, num2 = 3.3;
    
    List::Node *p = new List::Node();
    p->fst = false;
    p->sec = &num1;
    p->next = NULL;

    List::insert_node(p, 1, true, &num1);
    List::insert_node(p, 2, false, &num2);

    List::print_list(p);

    List::del_node(p, 1);

    List::print_list(p);

    List::free_all(p);

    return 0;
}