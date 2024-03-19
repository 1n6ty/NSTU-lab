#include <iostream>  
  
struct Queue {  
    int* data; // Указатель на массив элементов очереди  
    int N, size; // Размер массива  
    int beg, end; // Индексы начала и конца очереди  
  
    Queue() : data(nullptr), N(0), beg(-1), end(-1) {}  
  
    bool isEmpty() {  
        return beg == -1;  
    }  
  
    bool isFull() {  
        return (beg == 0 && end == N - 1) || (beg == end + 1);  
    }  
  
    void enqueue(int value) {  
        if (isFull()) {  
            std::cout << "The queue is full!\n";//Очередь переполнена!  
            return;  
        }  
  
        if (beg == -1) {  
            beg = 0;  
            end = 0;  
        }  
        else {  
            if (end == N - 1) {  
                end = 0;  
            }  
            else {  
                end++;  
            }  
        }  
        data[end] = value;  
    }  
  
    int dequeue() {  
        if (isEmpty()) {  
            std::cout << "The queue is empty!\n";//Очередь пуста!  
            return -1;  
        }  
  
        int value = data[beg];  
  
        if (beg == end) {  
            beg = end = -1;  
        }  
        else {  
            if (beg == N - 1) {  
                beg = 0;  
            }  
            else {  
                beg++;  
            }  
        }  
        return value;  
    }  
    void printQueue() { 
        std::cout << "Items in the queue:";//Элементы в очереди:  
            if (end < beg)  
                for (int i = 0; i < N; i++) {  
                    std::cout << " " << data[i];  
                    if (i == N - 1) break;  
                }  
            else for (int i = beg; i < N; i++) {  
                std::cout << " " << data[i];  
                if (i == end) break;  
            }  
            std::cout << std::endl; 
    }
    void emptyQueue() {  
        if (isEmpty()) {  
            std::cout << "The queue is empty!\n";//Очередь пуста!  
        }  
        else {  
            std::cout << "The queue is not empty.";//Очередь не пуста!
            printQueue();
        }  
    }  
};  
  
int main() {  
    Queue queue;  
    int size, choice, value;  
  
    std::cout << "Enter the queue size:";//Введите размер очереди:   
    std::cin >> size;  
  
    queue.data = new int[size];  
    queue.N = size;  
  
    std::cout << "Enter the queue items (enter -1 to end):\n";//Введите элементы очереди (для окончания введите -1):  
    do {  
        std::cin >> value;  
        if (value != -1) {  
            queue.enqueue(value);  
        }  
    } while (value != -1);  
  
    do {  
        std::cout << "Select an operation:" << std::endl;//Выберите операцию:  
        std::cout << "1. Create an empty queue" << std::endl;//1. Создать пустую очередь  
        std::cout << "2. Check if the queue is empty" << std::endl;//2. Проверить, является ли очередь пустой  
        std::cout << "3. Add an item to the queue" << std::endl;//3. Добавить элемент в очередь  
        std::cout << "4. Take an item from the queue" << std::endl;//4. Взять элемент из очереди  
        std::cout << "0. Exit" << std::endl;//0. Выйти  
        std::cin >> choice;  
  
        switch (choice) {  
        case 1:  
            delete[] queue.data;  
            queue.data = nullptr;  
            queue.N = 0;  
            queue.beg = -1;  
            queue.end = -1;  
            std::cout << "The queue has been successfully cleared\n";//Очередь успешно очищена  
            break;  
        case 2:  
            queue.emptyQueue();  
            break;  
        case 3:  
            std::cout << "Enter the value to add:";//Введите значение для добавления:    
            std::cin >> value;  
            queue.enqueue(value);  
            queue.printQueue();
            break;  
        case 4:  
            std::cout << "The item was taken from the queue:" << queue.dequeue() << std::endl;//Взят элемент из очереди:  
            queue.printQueue(); 
            break;  
        case 0:  
            std::cout << "Exiting the program\n";//Выход из программы  
            break;  
        default:  
            std::cout << "Wrong choice\n";//Неверный выбор  
        }  
    } while (choice != 0);  
  
    return 0;  
}
