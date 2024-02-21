#include <iostream>

namespace List{
    struct Node{
        bool fst;
        float *sec;
        Node *next;
    };

    char insert_node(List::Node *head, int ind, bool fst, float *sec){ // Insert node to ind
        List::Node *buff_node = head;
        for(int i = 0; i < ind - 1; i++){ // Iterate list to Node before ind
            if(buff_node->next != NULL){
                buff_node = buff_node->next;
            } else{
                return 0;
            }
        }

        List::Node *new_node = new List::Node(); // Create a new node and fill it with data
        new_node->fst = fst;
        new_node->sec = sec;

        new_node->next = buff_node->next; // Change pointers
        buff_node->next = new_node;
        
        return 1;
    }

    char del_node(List::Node *head, int ind){ // Delete ind
        List::Node *buff_node = head;
        for(int i = 0; i < ind - 1; i++){ // Iterate list to Node before ind
            if(buff_node->next != NULL){
                buff_node = buff_node->next;
            } else{
                return 0;
            }
        }

        List::Node *deleted = buff_node->next;
        if(buff_node->next != NULL){
            buff_node->next = buff_node->next->next; // Change pointers
        } else{
            return 0;
        }

        delete deleted; // Free deleted node
        return 1;
    }

    char free_all(List::Node *head){ // Free all nodes
        List::Node *buff_node = head, *prev_buff_node = NULL;
        while(buff_node != NULL){
            prev_buff_node = buff_node;
            buff_node = buff_node->next;

            delete prev_buff_node;
        }
        return 1;
    }

    void print_list(List::Node *head){ // Print the whole list
        while(head != NULL){
            std::cout << "bool: " << head->fst << "; float: " << *(head->sec) << "; -> ";
            head = head->next;
        }
        std::cout << '\n';
    }
}

int main(){

    int v;
    float num1 = 2.7, num2 = 3.3;

    List::Node *p = new List::Node();
    p->fst = false;
    p->sec = &num1;
    p->next = NULL;
    List::insert_node(p, 1, true, &num2);

    std::cout << "If you want to delete an element, enter 1. If you want to enter a new element, enter 2\n";
    std::cin >> v;

    if(v == 1){
        int ind;
        std::cout << "Enter the items you want to remove\n";
        std::cin >> ind;
        List::del_node(p, ind);
        List::print_list(p);
    }else if (v == 2){
        int ind_ins;
        bool fst_ins;
        float sec_ins;
        std::cout << "Enter the index, boolean and fractional value for the element you want to insert separated by space\n";
        std::cin >> ind_ins >> fst_ins >> sec_ins;
        List::insert_node(p, ind_ins, fst_ins, &sec_ins);
        List::print_list(p);
    }else std::cout << "incorrect number entered";
    
    List::free_all(p);

    return 0;
}