#include <iostream>
#include <vector>

// Custom types
using Derivative = double (*)(double, double);
using Formula = double (*)(Derivative f, double h, double x, double y);

// Method Formulas
double euler_formula(Derivative f, double h, double x, double y)
{
	return h * f(x, y);
}

double heun_formula(Derivative f, double h, double x, double y)
{

	return (h / 2) * (f(x, y) + f(x + h, y + h * f(x, y)));
}

double rungy_kutty_formula(Derivative f, double h, double x, double y)
{
	double k1 = h * f(x, y);
	double k2 = h * f(x + 0.5 * h, y + 0.5 * k1);
	double k3 = h * f(x + 0.5 * h, y + 0.5 * k2);
	double k4 = h * f(x + h, y + k3);

	return (k1 + 2 * k2 + 2 * k3 + k4) / 6;
}

// Main routine
std::vector<double> solve_differential_equation(double x0, double y0, double h, double xn, Derivative derivative, Formula method)
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
		y += method(derivative, h, x, y);
		x += h;

		// Save the result
		results.push_back(y);
	}

	return results;
}

// Custom derivative
double function(double x, double y)
{
	return x + y;
}

int main()
{
	// Function arguments
	double x0 = 0;
	double y0 = 1;
	double h = 0.1;
	double xn = 0.2;

	Formula formulas[] = {
		euler_formula,
		heun_formula,
		rungy_kutty_formula
	};

	for(Formula formula : formulas)
	{
		// Function call
		std::vector<double> results = solve_differential_equation(x0, y0, h, xn, function, formula);

		// Result print
		for (double result : results) std::cout << result << "\n";
		std::cout << "--------------------\n";
	}
}