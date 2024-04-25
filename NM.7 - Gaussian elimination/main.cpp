#include <iostream>
#include <fstream>
#include <string>

void print_matrix(double** matrix, int size, std::string delimiter = ", ")
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
            std::cout << "CALL:" << i << "\n";

            for (int j = k; j < size + 1; j++)
            {
                std::cout << "TOP: " << matrix[k][j] << "\n";
                std::cout << "BOTTOM:" << matrix[i][j] << "\n";
                std::cout << "M: " << m << "\n";
                std::cout << "--------------\n";

                matrix[i][j] -= matrix[k][j] * m;
            }
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
        std::cout << results[i] << "\n";
    }

    return results;
}

double** loadFromFile(std::string filename, int* sizeOUT) 
{
    std::fstream file(filename, std::ios::in);
    if (!file.is_open()) 
    {
        std::cerr << "Failed to open file." << std::endl;
        return nullptr;
    }

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
	int size;
	double* b_arr = nullptr;
	double** matrix = loadFromFile("input/a.txt", &size);

    std::cout << "ORIGINAL:\n";
    print_matrix(matrix, size);
    std::cout << "\n\n";

    double* results = gaussian_elimination(matrix, size);

    std::cout << "\n\n";
    std::cout << "NEW:\n";
    print_matrix(matrix, size);


    std::cout << "\n\nRESULTS: \n";
    for (int i = 0; i < size; i++)
        std::cout << results[i] << "\n";

	// Delete arrays
	for (int i = 0; i < size; i++)
		delete[] matrix[i];

	delete[] matrix;
	delete[] b_arr;
	delete[] results;
}