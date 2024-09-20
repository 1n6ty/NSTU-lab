#include <iostream>
#include <unordered_set>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <numeric>
#include <random>
#include <chrono>

using namespace std;

// Returns shuffled sequence of unique numbers of specified size, with values from start to start + size - 1.
vector<int> shuffled_sequence(int size, int start = 0) {
    vector<int> result(size);
    iota(result.begin(), result.end(), start);
    random_shuffle(result.begin(), result.end());
    return result;
}

// Returns sequence of random numbers of specified size, with values from 0 to max.
vector<int> random_sequence(int size, int max) {
    default_random_engine generator;
    uniform_int_distribution<int> distribution(0, max);
    vector<int> result;
    for (int i = 0; i < size; i++) {
        result.push_back(distribution(generator));
    }
    return result;
}

int main(int argc, char **argv) {
    const int size = (argc > 1 ? stoi(argv[1]) : 10);
    
    // Container to use.
    unordered_set<int> container;
    
    // Insert elements into container.
    const auto elems_to_add = shuffled_sequence(size);

    auto t1 = std::chrono::steady_clock::now();
    for (const auto &elem: elems_to_add) {
        container.insert(elem);
    }
    auto t2 = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double>(t2 - t1).count() << " - insertion time\n";
    // Perform search into container.
    int hits = 0;
    const auto elems_to_search = random_sequence(1000, 2 * size);

    t1 = std::chrono::steady_clock::now();
    for (const auto &elem: elems_to_search) {
        auto it = find(container.begin(), container.end(), elem);
        if (it != container.end()) {
            hits++;
        }        
    }
    t2 = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double>(t2 - t1).count() << " - search time\n";
    cout << "Found " << hits << " elements" << endl;

    return EXIT_SUCCESS;
}
