#include <iostream>
#include <fstream>
#include <string>
#include <List>

class Point
{
	//Attributes
	double x, y;

	//Constructors
public:

	Point(double x, double y) : x(x), y(y) {}
	Point() : x(0), y(0) {}

	//Getters
	double getX() { return this->x; }
	double getY() { return this->y; }

	//Setters
	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }

	//Methods
	Point* loadFromFile(std::string filename, int* outSize)
	{
		std::fstream file(filename, std::ios::in);

		//Get the size of the file
		int rows = 0;
		std::string buffer;
		while (std::getline(file, buffer))
			rows++;

		file.close();

		//Get points from the file
		Point* arr = new Point[rows];
		file.open(filename, std::ios::in);

		for (int i = 0; i < rows; i++)
		{
			double x, y;
			file >> x >> y;

			arr[i] = Point(x, y);
		}

		file.close();

		*outSize = rows;
		return arr;
	}
};

double** make_difference_table(int p_num, Point* p_arr)
{
	double** table = new double* [p_num];	//alokacja kolumn
	for (int i = 0; i < p_num; i++)	
	{
		table[i] = new double[p_num - i];	//alokacja wierszy
		table[i][0] = p_arr[i].getY();	//pierwszy wiersz to y-ki
	}

	for (int k = 1; k < p_num; k++) //obliczanie wynikow
	{
		for (int i = 0; i < p_num - k; i++)
			table[i][k] = (table[i + 1][k - 1] - table[i][k - 1]) / (p_arr[i + k].getX() - p_arr[i].getX());
	}

	return table;
}

double newton_interpolation(int n, Point* p_arr, double tgt_x)
{
	double** table = make_difference_table(n, p_arr);
	double W = p_arr[0].getY();

	for (int i = 1; i < n; i++)
	{
		double addition = table[0][i];
		for (int j = 0; j < i; j++)
			addition *= (tgt_x - p_arr[j].getX());

		W += addition;
	}

	for (int i = 0; i < n; i++)
		delete[] table[i];

	delete[] table;

	return W;
}

int main()
{
	Point p[] = {
		Point(-2, 0.13533528323),
		Point(2, 7.38905609893),
		Point(1, 2.718),
	};

	int n = sizeof(p) / sizeof(Point);
	double tgt_x = 1;
	std::cout << newton_interpolation(n, p, tgt_x);
}