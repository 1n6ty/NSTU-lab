#include "sort.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <cmath>

using namespace std::chrono;

std::vector<int> gen_random_array(int size) {
	std::vector<int> result(size);
	for (int i = 0; i < size; i++) {
		result[i] = rand();		
	}
	return result;
}

void measure(int size) {	
	std::vector<int> data = gen_random_array(size);
    std::vector<int> data_cpy = data;
	
	auto t1 = steady_clock::now();
	sort(data);
	auto t2 = steady_clock::now();
    std::cout << "N: " << size << ", time (quick_sort non-std): " << duration<double>(t2 - t1).count() << std::endl;

    t1 = steady_clock::now();
	std::sort(data.begin(), data.end());
	t2 = steady_clock::now();
    std::cout << "N: " << size << ", time (quick_sort std): " << duration<double>(t2 - t1).count() << std::endl;
}

int main(int argc, char **argv) {
    int max_pow = argc > 1 ? std::stoi(argv[1]) : 6;
    
	for (int i = 1; i <= max_pow; i++) {
		measure(pow(10, i));
	}

	return 0;
}