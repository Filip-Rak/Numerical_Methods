#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <chrono>

typedef double* (*EliminationFunction)(double**, int);

// ----------------------------
// Matrix Utility Functions
void print_matrix(double** matrix, int size, std::string delimiter = "\t", std::string name = "Matrix")
{
    std::cout << name << "\n";
    
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size + 1; j++)
            std::cout << matrix[i][j] << delimiter;

        std::cout << "\n";
    }
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

double** copy_matrix(double** source, int size) 
{
    double** copy = new double* [size];
    for (int i = 0; i < size; i++) 
    {
        copy[i] = new double[size + 1]; // Plus one for the augmented column
        for (int j = 0; j < size + 1; j++)
            copy[i][j] = source[i][j];
    }
    return copy;
}

void delete_matrix(double** matrix, int size)
{
    for (int i = 0; i < size; i++)
        delete[] matrix[i];

    delete[] matrix;
}

// ----------------------------
// Core Algorithm Functions
double* reverse_substitution(double** matrix, int size)
{
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

void elimination(double** matrix, int size, int k)
{
    for (int i = k + 1; i < size; i++)
    {
        double m = matrix[i][k] / matrix[k][k];

        for (int j = k; j < size + 1; j++)
            matrix[i][j] -= matrix[k][j] * m;
    }
}

// ----------------------------
// Pivoting and Swapping Functions
int max_in_column(double** matrix, int size, int column, int begin = 0)
{
    int max = begin;
    for (int i = begin + 1; i < size; i++)
    {
        if (abs(matrix[i][column]) > abs(matrix[max][column]))
            max = i;
    }

    return max;
}

int max_in_row(double* arr, int size, int begin = 0)
{
    int max = begin;
    for (int i = begin + 1; i < size; i++)
    {
        if (abs(arr[i]) > abs(arr[max]))
            max = i;
    }

    return max;
}

void switch_rows(double** matrix, int a, int b, int size)
{
    // Simply replace pointers
    double* buffer = matrix[a];
    matrix[a] = matrix[b];
    matrix[b] = buffer;
}

void switch_columns(double** matrix, int a, int b, int size)
{
    for (int i = 0; i < size; i++)
    {
        double buffer = matrix[i][a];
        matrix[i][a] = matrix[i][b];
        matrix[i][b] = buffer;
    }
}

// ----------------------------
// Main Gaussian Elimination Routines
double* gaussian_elimination(double** matrix, int size)
{
    for (int k = 0; k <= size; k++)
        elimination(matrix, size, k);

    return reverse_substitution(matrix, size);
}

double* gaussian_elimination_column_pivot(double** matrix, int size)
{
    for(int k = 0; k <= size; k++)
    {
        // Column switching
        int max_index = max_in_row(matrix[k], size, k);
        switch_columns(matrix, k, max_index, size);

        elimination(matrix, size, k);
    }

    return reverse_substitution(matrix, size);
}

double* gaussian_elimination_row_pivot(double** matrix, int size)
{
    for (int k = 0; k <= size; k++)
    {
        // Rows switching
        int max_index = max_in_column(matrix, size, k, k);
        switch_rows(matrix, k, max_index, size);

        elimination(matrix, size, k);
    }

    return reverse_substitution(matrix, size);
}

// ----------------------------
// Main Execution Functions
void method_execution(double* (*function)(double**, int), double** matrix, int size)
{
    // Time measurment for the algorithm
    auto start = std::chrono::high_resolution_clock::now();

    // Gaussian Elimination Routine Call
    double* results = function(matrix, size);

    // Time measurement finalization
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // Printing the matrix after function's work
    //print_matrix(matrix, size, "After function call matrix");

    // Printing the results
    std::cout << "\nResults: \n";
    for (int i = 0; i < size; i++)
        std::cout << results[i] << " ";

    // Printing the duration
    std::cout << "\nExecution time (microseconds): " << duration.count() << "\n";

    // Clear results 
    delete[] results;
}

int main()
{
    // Input handling
    int size;
    double** matrix = loadFromFile("input/6x6.txt", &size);

    if (!matrix)
    {
        std::cout << "Failed to open file\n";
        return -1;
    }

    // Set input precision
    //std::cout << std::setprecision(2);
    print_matrix(matrix, size, "\t", "Input Matrix");

    // Array of algorithms
    EliminationFunction functions[] = {
        gaussian_elimination,
        gaussian_elimination_column_pivot,
        gaussian_elimination_row_pivot
    };
    
    // Execute the algorithms
    for (int i = 0; i < sizeof(functions) / sizeof(EliminationFunction); i++)
    {
        double** copy = copy_matrix(matrix, size);
        method_execution(functions[i], copy, size);
        delete_matrix(copy, size);
    }

    // Final cleanup
    delete_matrix(matrix, size);
}