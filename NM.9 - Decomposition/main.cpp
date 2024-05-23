#include <iostream>
#include <fstream>
#include <string>

// ----------------------------
// Matrix / Array Utility Functions
void print_matrix(double** matrix, int size, std::string name = "Matrix", std::string delimiter = "\t")
{
    std::cout << name << "\n";

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
            std::cout << matrix[i][j] << delimiter;

        std::cout << "\n";
    }
}

void print_array(double* arr, int size, std::string name = "Array", std::string delimiter = "\t")
{
    std::cout << name << "\n";

    for (int i = 0; i < size; i++)
        std::cout << arr[i] << delimiter;

    std::cout << "\n";
}

bool load_matrix_and_extension(double**& A, double*& B, std::string filename, int* sizeOUT)
{
    std::fstream file(filename, std::ios::in);
    if (!file.is_open())
        return false;

    // Get the number of rows
    *sizeOUT = 0;
    std::string buffer;
    while (std::getline(file, buffer))
        (*sizeOUT)++;

    // Create the matrix and extension
    B = new double[*sizeOUT];

    A = new double* [*sizeOUT];
    for (int i = 0; i < *sizeOUT; i++)
        A[i] = new double[*sizeOUT];

    // Reset file pointer to the beginning
    file.clear(); // Clear eof flag
    file.seekg(0, std::ios::beg);

    for (int i = 0; i < *sizeOUT; i++)
    {
        for (int j = 0; j < *sizeOUT; j++)
            file >> A[i][j];

        file >> B[i];
    }

    file.close();

    return true;
}

double** copy_matrix(double** source, int size)
{
    double** copy = new double* [size];
    for (int i = 0; i < size; i++)
    {
        copy[i] = new double[size];
        for (int j = 0; j < size; j++)
            copy[i][j] = source[i][j];
    }
    return copy;
}

double** identity_matrix(int size)
{
    double** matrix = new double*[size];
    for (int i = 0; i < size; i++)
    {
        matrix[i] = new double[size];
        for (int j = 0; j < size; j++)
            matrix[i][j] = 0;

        matrix[i][i] = 1;
    }

    return matrix;
}

void delete_matrix(double**& matrix, int size)
{
    for (int i = 0; i < size; i++)
        delete[] matrix[i];

    delete[] matrix;
    matrix = nullptr;
}

void delete_array(double*& array)
{
    delete[] array;
    array = nullptr;
}

// ----------------------------
// Core Algorithm Functions
void elimination(double** U, double** L, int size, int k) 
{
    for (int i = k + 1; i < size; i++) 
    {
        // Calculate the multiplier for the current row and store it in L
        double m = U[i][k] / U[k][k];
        L[i][k] = m;

        // Update the elements of the current row in the U matrix
        for (int j = k; j < size; j++)
            U[i][j] -= m * U[k][j];
    }
}

double* reverse_substitution(double** matrix, double* vector, int size)
{
    double* results = new double[size];
    for (int i = size - 1; i >= 0; i--)
    {
        double sum = vector[i];
        for (int j = i + 1; j < size; j++)
            sum -= matrix[i][j] * results[j];

        results[i] = sum / matrix[i][i];
    }

    return results;
}

double* forward_substitution(double** matrix, double* vector, int size)
{
    double* results = new double[size];
    for (int i = 0; i < size; i++)
    {
        double sum = vector[i];
        for (int j = 0; j < i; j++)
            sum -= matrix[i][j] * results[j];

        results[i] = sum / matrix[i][i];
    }

    return results;
}

// ----------------------------
// Main Decomposition Routines
void LU_decomposition(double** A, double**& L, double**& U, int size) 
{
    // Initialize U as a copy of A
    U = copy_matrix(A, size);

    // Initialize L as an identity matrix
    L = identity_matrix(size);

    // Perform elimination process for each column
    for (int k = 0; k < size; k++)
        elimination(U, L, size, k);
}


int main()
{
    int size = 0;
    double** matrix = nullptr;
    double* vector = nullptr;
    if (!load_matrix_and_extension(matrix, vector, "input/3x3.txt", &size))
    {
        std::cout << "Loading Failed!";
        return -1;
    }

    double** L_matrix = nullptr;
    double** U_matrix = nullptr;
    LU_decomposition(matrix, L_matrix, U_matrix, size);

    print_matrix(matrix, size);
    print_matrix(L_matrix, size, "\nL");
    print_matrix(U_matrix, size, "\nU");

    double* y_arr = forward_substitution(L_matrix, vector, size);
    double* x_arr = reverse_substitution(U_matrix, y_arr, size);

    print_array(y_arr, size, "\nY");
    print_array(x_arr, size, "\nX");


    // Cleanup
    delete_matrix(matrix, size);
    delete_matrix(L_matrix, size);
    delete_matrix(U_matrix, size);
    delete_array(vector);
    delete_array(y_arr);
}