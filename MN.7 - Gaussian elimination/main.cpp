#include <iostream>
#include <fstream>
#include <string>

void print_matrix(double** matrix, double* b_arr, int size, std::string delimiter = ", ")
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
            std::cout << matrix[i][j] << delimiter;

        std::cout << "| " << b_arr[i] << "\n";
    }
}


void gaussian_elimination(double** matrix, double* b_arr, int size)
{
    int offsetY = 1;
    int offsetX = 0;

    while (offsetX <= size)
    {
        for (int i = offsetY; i < size; i++)
        {
            double m = matrix[i][offsetX] / matrix[offsetY - 1][offsetX];

            for (int j = offsetX; j < size; j++)
            {
                std::cout << "TOP: " << matrix[offsetY - 1][j] << "\n";
                std::cout << "BOTTOM:" << matrix[i][j] << "\n";
                std::cout << "M: " << m << "\n";
                std::cout << "--------------\n";

                matrix[i][j] -= matrix[offsetY - 1][j] * m;
            }
            b_arr[offsetY] -= b_arr[offsetY - 1] * m;
        }

        offsetY += 1;
        offsetX += 1;
    }
}

double** loadFromFile(std::string filename, double** b_arr, int* sizeOUT) 
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

    (*sizeOUT)--;

    // Create the matrix
    double** matrix = new double* [*sizeOUT];
    for (int i = 0; i < *sizeOUT; i++)
        matrix[i] = new double[*sizeOUT];

    // Reset file pointer to the beginning
    file.clear(); // Clear eof flag
    file.seekg(0, std::ios::beg);

    for (int i = 0; i < *sizeOUT; i++) 
    {
        for (int j = 0; j < *sizeOUT; j++)
            file >> matrix[i][j];
    }

    *b_arr = new double[*sizeOUT];
    for (int i = 0; i < *sizeOUT; i++) 
        file >> (*b_arr)[i];

    file.close();
    return matrix;
}

int main()
{
	int size;
	double* b_arr = nullptr;
	double** matrix = loadFromFile("input/a.txt", &b_arr, &size);

    std::cout << "ORIGINAL:\n";
    print_matrix(matrix, b_arr, size);
    std::cout << "\n\n";

    gaussian_elimination(matrix, b_arr, size);

    std::cout << "\n\n";
    std::cout << "NEW:\n";
    print_matrix(matrix, b_arr, size);

	// Delete arrays
	for (int i = 0; i < size; i++)
		delete[] matrix[i];

	delete[] matrix;
	delete[] b_arr;
}