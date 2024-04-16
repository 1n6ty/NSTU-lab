#include <iostream>
#include <vector>
#include <string.h>

#define STACK_CAP 100

namespace S {
    typedef struct Node {
        char data;
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

    char insert(stack *q, char data){ // Insert new node with data at the top of the stack
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
            buff = buff->next;
        }
        std::cout << "NULL";
    }
};

void parse_eq(S::stack *stack, char *eq, int fst, int lst, char B){
    if(eq[fst] == '(' && eq[lst] == ')'){
        fst ++;
        lst --;
    }
    if(fst > lst){
        return;
    } else if(fst == lst){
        if(eq[fst] == 'B') S::insert(stack, B);
        else S::insert(stack, eq[fst]);
        return;
    }

    int br_c = 0;
    for(int i = fst; i <= lst; i++){
        if(eq[i] == '('){
            br_c ++;
        } else if(eq[i] == ')'){
            br_c --;
        } else if((eq[i] == '!' || eq[i] == '|' || eq[i] == '&') && br_c == 0){
            S::insert(stack, eq[i]);
            if(i + 1 == lst){
                parse_eq(stack, eq, i + 1, lst, B);
                parse_eq(stack, eq, fst, i - 1, B);
            } else {
                parse_eq(stack, eq, fst, i - 1, B);
                parse_eq(stack, eq, i + 1, lst, B);
            }
            break;
        }
    }
}

char is_op(char op){
    return (op == '|' || op == '&' || op == '!');
}

char operate(std::vector<char> arr, char op, char ch){
    if(op == '|'){
        return ((*(arr.rbegin()) - '0') | (*(arr.rbegin() + 1) - '0')) + '0';
    } else if(op == '&'){
        return ((*(arr.rbegin()) - '0') & (*(arr.rbegin() + 1) - '0')) + '0';
    } else if(op == '!'){
        return (!(*(arr.rbegin()) - '0')) + '0';
    } else {
        return 0;
    }
}

int main(){

    S::stack* stack = S::create(STACK_CAP);
    FILE *inp = fopen("lab-2/inp", "r");

    char eq[] = "((!(B|(B&0)))&B)", B;
    fscanf(inp, "%c", &B);

    parse_eq(stack, eq, 0, strlen(eq) - 1, B);
    S::print_stack(stack);

    char step;
    S::Node buff;
    std::vector<char> arr;
    while(!S::is_empty(stack)){
        if(S::get_top(stack, buff) == 2) break;
        if(is_op(buff.data)){
            step = operate(arr, buff.data, B);
            arr.erase(arr.end() - std::min(2, (int) arr.size()), arr.end());
            arr.push_back(step);
            for(auto it = arr.rbegin(); it != arr.rend(); it++) S::insert(stack, *it);
            S::print_stack(stack);
            arr = std::vector<char>();
        } else {
            arr.push_back(buff.data);
        }
    }

    fclose(inp);
    S::free(stack);

    std::cout << '\n' << buff.data << " - answer" << '\n';

    return 0;
}