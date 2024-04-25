#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>

void print_matrix(double** matrix, int size, std::string delimiter = "\t")
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size + 1; j++)
            std::cout << matrix[i][j] << delimiter;

        std::cout << "\n";
    }
}

double* gaussian_elimination(double** matrix, int size)
{
    int k = 0;
    while (k <= size)
    {
        for (int i = k + 1; i < size; i++)
        {
            double m = matrix[i][k] / matrix[k][k];

            for (int j = k; j < size + 1; j++)
                matrix[i][j] -= matrix[k][j] * m;
        }

        k++;
    }

    double* results = new double[size];
    for (int i = size - 1; i >= 0; i--)
    {
        double sum = matrix[i][size];
        for (int j = i + 1; j < size; j++)
            sum -= matrix[i][j] * results[j];

        results[i] = sum / matrix[i][i];
    }

    return results;
}

double** loadFromFile(std::string filename, int* sizeOUT) 
{
    std::fstream file(filename, std::ios::in);
    if (!file.is_open()) 
        return nullptr;

    // Get the number of rows
    *sizeOUT = 0;
    std::string buffer;
    while (std::getline(file, buffer))
        (*sizeOUT)++;

    // Create the matrix
    double** matrix = new double* [*sizeOUT];
    for (int i = 0; i < *sizeOUT; i++)
        matrix[i] = new double[*sizeOUT + 1];

    // Reset file pointer to the beginning
    file.clear(); // Clear eof flag
    file.seekg(0, std::ios::beg);

    for (int i = 0; i < *sizeOUT; i++) 
    {
        for (int j = 0; j < *sizeOUT + 1; j++)
            file >> matrix[i][j];
    }

    file.close();
    return matrix;
}

int main()
{
    // Input handling
	int size;
	double** matrix = loadFromFile("input/b.txt", &size);

    if (!matrix)
    {
        std::cout << "Failed to open file\n";
        return 0;
    }

    // Setting precision and displaying input

    std::cout << std::setprecision(2);
    std::cout << "Input matrix:\n";
    print_matrix(matrix, size);

    // Actual algorithm
    double* results = gaussian_elimination(matrix, size);

    // Printing the matrix after function's work
    std::cout << "\nMatrix after function call:\n";
    print_matrix(matrix, size);

    // Printing the results
    std::cout << "\nResults: \n";
    for (int i = 0; i < size; i++)
        std::cout << results[i] << "\n";

	// Delete arrays
	for (int i = 0; i < size; i++)
		delete[] matrix[i];

	delete[] matrix;
	delete[] results;
}