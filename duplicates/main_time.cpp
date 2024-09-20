#include "duplicates.h"
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <cmath>

using namespace std;
using namespace std::chrono;

vector<int> gen_random_array(int size) {
	vector<int> result(size);
	for (int i = 0; i < size; i++) {
		result[i] = rand();		
	}
	return result;
}

void measure(int size) {	
	const auto data = gen_random_array(size);
	
	auto t1 = steady_clock::now();
	vector<int> res = get_duplicates_fast(data);
	auto t2 = steady_clock::now();
    cout << "N: " << size << ", time (fast_get): " << duration<double>(t2 - t1).count() << endl;

    t1 = steady_clock::now();
	res = get_duplicates_slow(data);
	t2 = steady_clock::now();
    cout << "N: " << size << ", time (slow_get): " << duration<double>(t2 - t1).count() << endl;

    t1 = steady_clock::now();
	bool b = has_duplicates_fast(data);
	t2 = steady_clock::now();
    cout << "N: " << size << ", time (fast_has): " << duration<double>(t2 - t1).count() << endl;

    t1 = steady_clock::now();
	b = has_duplicates_slow(data);
	t2 = steady_clock::now();
    cout << "N: " << size << ", time (slow_has): " << duration<double>(t2 - t1).count() << endl;
}

int main(int argc, char **argv) {
    int max_pow = argc > 1 ? stoi(argv[1]) : 6;
    
	for (int i = 1; i <= max_pow; i++) {
		measure(pow(10, i));
	}

	return 0;
}
