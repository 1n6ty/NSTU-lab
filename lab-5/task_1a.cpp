#include <iostream>
#include <string>

struct Element {
    std::string key;
    double value;
};

void insertionSort(Element arr[], int n, int& comparisons, int& moves) {
    for (int i = 1; i < n; ++i) {
        Element temp = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j].key > temp.key) {
            comparisons++;
            arr[j + 1] = arr[j];
            moves++;
            j--;
        }

        comparisons++;
        arr[j + 1] = temp;
        moves++;
    }
}

int main() {
    int n;
    std::cout << "Enter the number of elements in the table: ";
    std::cin >> n;

    Element table[n];

    std::cout << "Enter elements (key followed by value):\n";
    for (int i = 0; i < n; ++i) {
        std::cin >> table[i].key >> table[i].value;
    }

    int comparisons = 0;
    int moves = 0;
    insertionSort(table, n, comparisons, moves);

    std::cout << "Sorted table:\n";
    for (int i = 0; i < n; ++i) {
        std::cout << table[i].key << " " << table[i].value << std::endl;
    }

    return 0;
}
