#include<iostream>
#include<math.h>
using namespace std;

int main() {
	int n = 0;
	cin >> n;
	bool flag = true;
	for (int i = 2; i < sqrt(n) + 1; i++) {
		if (n % i == 0) {
			flag = false;
			break;
		}
	}
	cout << flag;
	return 0;
}
