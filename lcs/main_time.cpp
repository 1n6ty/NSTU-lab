#include "lcs.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <cmath>

using namespace std::chrono;

std::string gen_random_str(int size) {
	std::string result = "";
	for (int i = 0; i < size; i++) {
		result += std::string(1, char(rand()));
	}
	return result;
}

void measure(int size) {	
	std::string data1 = gen_random_str(10);
	std::string data2 = gen_random_str(size);
	std::string tmp;

	auto t1 = steady_clock::now();
	tmp = lcs(data1, data2);
	auto t2 = steady_clock::now();
    std::cout << "N: " << size << ", time (lcs): " << duration<double>(t2 - t1).count() << std::endl;

    t1 = steady_clock::now();
	tmp = lcs_brute(data1, data2);
	t2 = steady_clock::now();
    std::cout << "N: " << size << ", time (lcs_brute): " << duration<double>(t2 - t1).count() << std::endl;
}

int main(int argc, char **argv) {
    int max_pow = argc > 1 ? std::stoi(argv[1]) : 6;
    
	for (int i = 1; i <= max_pow; i++) {
		measure(i);
	}

	return 0;
}