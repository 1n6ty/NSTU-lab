#include <iostream>

struct Node { //структура ячейки очереди
    int data;
    Node* next;
};

struct Queue { //структура очереди
    Node* front;
    Node* rear;
    int size;
    int capacity;
};

void initQueue(Queue& q, int capacity) { //создание очереди нулевой длины/очистка очереди
    q.front = nullptr;
    q.rear = nullptr;
    q.size = 0;
    q.capacity = capacity;
}

bool isEmpty(Queue q) { //проверка очереди на пустоту
    return q.front == nullptr;
}

bool isFull(Queue q) { //проверка очереди на переполненость 
    return q.size == q.capacity;
}

void printQueue(Queue q) { //вывод очереди
    if (isEmpty(q)) {
        std::cout << "Queue is empty.\n";
        return;
    }

    std::cout << "Queue: ";
    Node* current = q.front;
    while (current != nullptr) {
        std::cout << current->data << " ";
        current = current->next;
    }
    std::cout << "\n";
}


int enqueue(Queue& q, int value) { //добавление элемента в очередь
    if (isFull(q)) {
        return 1; // Очередь переполнена
    }

    Node* newNode = new Node;
    newNode->data = value;
    newNode->next = nullptr;

    if (isEmpty(q)) {
        q.front = newNode;
    } else {
        q.rear->next = newNode;
    }

    q.rear = newNode;
    q.size++;

    printQueue(q);

    return 0; 
}

int dequeue(Queue& q, int& value) { //удаление элемента из очереди
    if (isEmpty(q)) {
        return 2; // Очередь пуста
    }

    Node* temp = q.front;
    value = temp->data;

    q.front = q.front->next;
    delete temp;
    q.size--;

    if (isEmpty(q)) {
        q.rear = nullptr;
    }

    printQueue(q);

    return 0; 
}

int main() {
    Queue q;
    int capacity, choice, element, result, exit;
    bool end = true;

    std::cout << "Enter queue size: ";
    std::cin >> capacity;

    initQueue(q, capacity);

    while (end){

        std::cout << "To clear the queue, enter 1.\n" <<
                     "To check whether the queue is empty, press 2.\n" <<
                     "To add an item to the queue, press 3.\n" << 
                     "To remove an item from the queue, enter 4. \n";
        std::cin >> choice;

        if(choice == 1){
            initQueue(q, capacity);
            std::cout << "Queue is empty!\n";
        }else if (choice == 2){
            if (isEmpty(q)) {
                std::cout << "Queue is empty!\n";
            } else {
                std::cout << "Queue is not empty.\n";
            }
        }else if (choice == 3){
            std::cout << "Enter element for add: ";
            std::cin >> element;
            result = enqueue(q, element);
            if (result == 1) {
                std::cout << "Queue is full!\n";
            } else {
                std::cout << "Element in queue.\n";
            }
        }else if (choice == 4){
            int extractedElement;
            result = dequeue(q, extractedElement);
            if (result == 2) {
                std::cout << "Queue is empty!\n";
            } else {
                std::cout << "Takes element: " << extractedElement << "\n";
            }
        }else{
            std::cout << "Incorect value.\n";
            end = false;
        }

        std::cout << "Enter 1 if you want to continue working with the queue and 0 if you don't want to. \n";
        std::cin >> exit;
        if (exit == 0) end = false;
    }

    return 0;
}
