#include <iostream>

namespace S {
    typedef struct Node {
        int data;
        Node *next;
    } Node;

    typedef struct stack {
        int max_e, n;
        Node *top;
    } stack;

    stack* create(int max_e){ // Create stack and fill it with initial values
        stack *q = new stack;

        q->max_e = max_e;
        q->n = 0;
        
        q->top = NULL;

        return q;
    };

    char free(stack *q){
        Node *buff;
        while(q->n > 0){ // Consistenly free all nodes
            buff = q->top;

            q->top = q->top->next;
            q->n --;

            delete buff;
        }

        return 0;
    };

    char is_empty(stack *q){
        return q->n == 0;
    }

    char insert(stack *q, int data){ // Insert new node with data at the top of the stack
        if(q->n + 1 > q->max_e) return 1; // Check whether the stack is full

        Node *new_top = new Node;
        new_top->next = q->top;
        new_top->data = data;

        q->top = new_top;
        q->n ++;

        return 0;
    }

    char get_top(stack *q, Node &out){ // Return top element and remove it from stack
        if(is_empty(q)) return 2; // Check whether the stack is empty

        out = *(q->top);

        Node *buff = q->top;
        q->top = q->top->next;
        q->n --;

        delete buff;

        return 0;
    }

    void print_stack(stack *q){ // Consistent print of the stack
        Node *buff = q->top;

        std::cout << "\n top: ";
        for(int i = 0; i < q->n; i++){
            std::cout << buff->data << " -> ";
            buff = q->top->next;
        }
        std::cout << "NULL";
    }
};

int main(){

    S::stack* stack = S::create(5);

    int mode, val;
    S::Node buff;

    // Gain user input -> mode and execute that mode
    std::cout << "Enter the mode(1 - insert, 2 - get top, 3 - exit): ";
    while(scanf("%d", &mode) > 0 && mode != 3){
        if(mode == 1){
            std::cout << "Enter the data of a node: ";
            scanf("%d", &val);
            if(S::insert(stack, val) == 1) std::cout << "Overflow!!!\n";
        } else if(mode == 2){
            if(S::get_top(stack, buff) == 2){
                std::cout << "Stack is empty!\n";
            } else {
                std::cout << "top value: " << buff.data << '\n';
            }
        }
        S::print_stack(stack);
        std::cout << "\nEnter the mode(1 - insert, 2 - get top, 3 - exit): ";
    }

    S::free(stack);
    return 0;
}