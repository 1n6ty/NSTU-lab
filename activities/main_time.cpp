#include "activities.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <cmath>

using namespace std::chrono;

std::vector<Activity> gen_random_array(int size) {
	std::vector<Activity> result(size);
	for (int i = 0; i < size; i++) {
		result[i].start = rand();
		result[i].finish = rand() + result[i].start;
	}
	return result;
}

void measure(int size) {	
	std::vector<Activity> data = gen_random_array(size);
	std::vector<Activity> tmp;

	auto t1 = steady_clock::now();
	tmp = get_max_activities(data);
	auto t2 = steady_clock::now();
    std::cout << "N: " << size << ", time (get_max_activities): " << duration<double>(t2 - t1).count() << std::endl;

    t1 = steady_clock::now();
	tmp = get_max_activities_brute(data);
	t2 = steady_clock::now();
    std::cout << "N: " << size << ", time (get_max_activities_brute): " << duration<double>(t2 - t1).count() << std::endl;
}

int main(int argc, char **argv) {
    int max_pow = argc > 1 ? std::stoi(argv[1]) : 6;
    
	for (int i = 1; i <= max_pow; i++) {
		measure(pow(2, i));
	}

	return 0;
}