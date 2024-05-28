#include <iostream>
#include <vector>

// Custom types
using Derivative = double (*)(double, double);

// Main routine
std::vector<double> euler_method(double x0, double y0, double h, double xn, Derivative f)
{
	// Result array initialization
	std::vector<double> results;

	// Adding the first known result to array
	results.push_back(y0);

	// Assigning x0 as a start point
	double y = y0;
	double x = x0;

	while (x < xn)
	{
		// Get new x and y
		y += h * f(x, y);
		x += h;

		// Save the result
		results.push_back(y);
	}

	return results;
}

// Custom derivative
double function(double x, double y)
{
	return y;
}

int main()
{
	// Function arguments
	double x0 = 0;
	double y0 = 1;
	double h = 0.1;
	double xn = 0.4;

	// Function call
	std::vector<double> results  = euler_method(0, 1, 0.1, 0.4, function);

	// Argument print
	for (double result : results)
		std::cout << result << "\n";
}