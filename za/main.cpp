#include <iostream>
#include <vector>
#include <algorithm>

std::vector<std::vector<double>> swap_matrix(std::vector<std::vector<double>> matrix, int numRows, int numCols){
    int minIndex = 0, maxIndex = 0;
    double minElement = matrix[0][0], maxElement = matrix[0][0];
    std::vector<double> tempRow(numCols);

    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            if (matrix[i][j] < minElement) {
                minElement = matrix[i][j];
                minIndex = i;
            }
            if (matrix[i][j] > maxElement) {
                maxElement = matrix[i][j];
                maxIndex = i;
            }
        }
    }
    
    for (int col = 0; col < numCols; ++col) {
        tempRow[col] = matrix[minIndex][col];  
        matrix[minIndex][col] = matrix[maxIndex][col];  
        matrix[maxIndex][col] = tempRow[col];  
    }

    return matrix;
}


int main() {

    int numRows, numCols;
    std::wcout << "Enter the number of rows: ";
    std::cin >> numRows;
    std::wcout << "Enter the number of columns: ";
    std::cin >> numCols;

    std::vector<std::vector<double>> matrix(numRows, std::vector<double>(numCols));

    std::wcout << "Enter the elements of the matrix:" << std::endl;
    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            std::cin >> matrix[i][j];
        }
    }

    matrix = swap_matrix(matrix, numRows, numCols);

    std::cout << "After swapping:" << std::endl;
     for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }


    return 0;
}