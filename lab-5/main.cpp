#include <iostream>

int main(){
    int n, m;
    std::cout << "Enter number of rows and columns in the matrix: ";
    std::cin >> n >> m;

    int matrix[n][m];

    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= m; j++){
            std::cin >> matrix[i][j];
        }
    }

    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= m; j++){
            std::cout << matrix[i][j] << " ";
        }

        std::cout  << '\n';
    }
    

    return 0;
}