#include <iostream>
#include <fstream>
#include <string>

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

class LinearRegression 
{
	//Attributes
	double a0, a1, r;

public:
	//Constructor
	LinearRegression(int n, Point* p_arr)
	{
		//shared variables
		double xy = 0, xn = 0, yn = 0, x2 = 0, y2 = 0;
		for (int i = 0; i < n; i++)
		{
			xy += p_arr[i].getX() * p_arr[i].getY();
			x2 += pow(p_arr[i].getX(), 2);
			y2 += pow(p_arr[i].getY(), 2);
			xn += p_arr[i].getX();
			yn += p_arr[i].getY();
		}
		
		//shared by a0 and a1
		double denominator = (n * x2) - pow(xn, 2);

		//coefficients
		if (denominator == 0)
			throw std::invalid_argument("invalid point array");
		else
		{
			this->a1 = ((n * xy) - (xn * yn)) / denominator;
			this->a0 = ((x2 * yn) - (xn * xy)) / denominator;
		}

		denominator = sqrt((n * x2) - pow(xn, 2)) * sqrt((n * y2) - pow(yn, 2));
		if (denominator == 0)
			this->r = 0;
		else 
			this->r = ((n * xy) - (xn * yn)) / denominator;
	}

	//Getters
	double getIntercept() { return a0; }
	double getSlope() { return a1; }
	double getCorrelation() { return r; }

	//Methods
	double predict(double x) { return a0 + a1 * x; }
};


int main()
{
	Point p_arr[] = {
		Point(2, 2),
		Point(4, 11),
		Point(6, 35),
		Point(8, 39),
		Point(10, 63),
	};

	int n = sizeof(p_arr) / sizeof(Point);

	try 
	{
		LinearRegression result(n, p_arr);
		std::cout << "a1: " << result.getSlope() << "\na0: " << result.getIntercept() << "\nR: " << result.getCorrelation();
	}
	catch (std::invalid_argument exc)
	{
		std::cout << exc.what();
	}
	
}