#include <iostream>

namespace List{
    struct Node{
        int fst;
        Node *next;
    };

    struct queue{
        Node** queueArray;
        int beg;
        int end;
        int size_queue;
    };

    void init_queue(queue& q, int size){ //создание нулевой очереди
        q.queueArray = new Node*[size];
        q.beg = -1;
        q.end = -1;
        q.size_queue = size;
    }

    
    bool isEmpty(queue& q) {
        return q.beg == -1;
    }

    bool isFull(queue& q) {
        return (q.end + 1) % q.size_queue == q.beg;
    }

    char insert_node(queue& q, int fst){
        if(isFull(q)) return 1;

        Node* newNode = new Node;
        newNode->fst = fst;
        newNode->next = nullptr;

        if(isEmpty(q)) q.beg = 0;

        q.end = (q.end + 1) % q.size_queue;
        q.queueArray[q.end] = newNode;
        
        return 0;
    }
}  


int main(){
    List::queue queue;

    int sizeQ;
    int val;
    std::cin >> sizeQ;
    std::cin >> val;

    List::init_queue(queue, sizeQ);
    List::insert_node(queue, val);



}