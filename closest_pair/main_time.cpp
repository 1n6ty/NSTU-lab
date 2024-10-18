#include "closest_pair.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <cmath>

using namespace std::chrono;

std::vector<Point> gen_random_array(int size) {
	std::vector<Point> result(size);
	for (int i = 0; i < size; i++) {
		result[i] = Point((double) rand() / (double) rand(), (double) rand() / (double) rand());
	}
	return result;
}

void measure(int size) {	
	std::vector<Point> data = gen_random_array(size);
	std::pair<Point, Point> tmp;

	auto t1 = steady_clock::now();
	tmp = closest_pair(data);
	auto t2 = steady_clock::now();
    std::cout << "N: " << size << ", time (closest_point): " << duration<double>(t2 - t1).count() << std::endl;

    t1 = steady_clock::now();
	tmp = closest_pair_brute(data);
	t2 = steady_clock::now();
    std::cout << "N: " << size << ", time (closest_point_brute): " << duration<double>(t2 - t1).count() << std::endl;
}

int main(int argc, char **argv) {
    int max_pow = argc > 1 ? std::stoi(argv[1]) : 6;
    
	for (int i = 1; i <= max_pow; i++) {
		measure(pow(10, i));
	}

	return 0;
}