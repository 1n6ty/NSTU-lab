#include <iostream>
#include <string>
#include <vector>
#include <list>

struct Element {
    std::string key;
    double value;
};

void bubbleSort(Element arr[], int n, int& comparisons, int& swaps) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            comparisons++;
            if (arr[j].key > arr[j + 1].key) {
                Element temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swaps++;
                swapped = true;
            }
        }
        if (!swapped) {
            break;
        }
    }
}

bool hasKey(const Element arr[], int n, const std::string& key) {
    for (int i = 0; i < n; ++i) {
        if (arr[i].key == key) {
            return true;
        }
    }
    return false;
}

int main() {
    int n;
    std::cout << "Enter the number of elements in the table: ";
    std::cin >> n;

    Element table[n];
    std::vector<std::list<Element>> hashTable(n); 

    for (int i = 0; i < n; ++i) {
        std::cout << "Enter element key (no more than 15 characters): ";
        std::cin >> table[i].key;
        
        if (table[i].key.size() > 15) {
            std::cout << "Invalid key length. Please enter a key with at most 15 characters." << std::endl;
            return 0;
        }

        std::cout << "Enter element value: ";
        std::cin >> table[i].value;
    }

    int comparisons = 0;
    int moves = 0;

    for (int i = 0; i < n; i++) {
            int hash = std::hash<std::string>{}(table[i].key) % n; 
            hashTable[hash].push_back(table[i]); 
        }


    bubbleSort(table, n, comparisons, moves);

    std::cout << "\nSorted table:\n";
    for (int i = 0; i < n; ++i) {
        std::cout << table[i].key << " " << table[i].value << std::endl;
    }

    std::cout << "Number of comparisons: " << comparisons << std::endl;
    std::cout << "Number of moves: " << moves << std::endl;

    return 0;
}
