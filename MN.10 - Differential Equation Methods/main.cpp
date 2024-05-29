#include <iostream>
#include <vector>
#include <chrono>

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
std::vector<double> solve_differential_equation(double x0, double y0, double h, double xn, Derivative derivative, Formula formula)
{
	// Result array initialization
	std::vector<double> results;

	// Adding the first known result to the vector
	results.push_back(y0);

	// Assigning x0 as a start point
	double y = y0;
	double x = x0;

	while (x < xn)
	{
		// Get new x and y
		y += formula(derivative, h, x, y);
		x += h;

		// Save the result
		results.push_back(y);
	}

	return results;
}

double measure_time(int trials, double x0, double y0, double h, double xn, Derivative derivative, Formula formula)
{
	double time_sum = 0;

	for (int i = 0; i < trials; i++)
	{
		// Start of the time measurement
		auto start = std::chrono::high_resolution_clock::now();

		// Measured function call
		solve_differential_equation(x0, y0, h, xn, derivative, formula);

		// Time measurement finalization
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

		time_sum += duration.count();
	}

	// Return average time
	return time_sum / (double) trials;
}

// Custom derivative
double function(double x, double y)
{
	return y * log(y);
}

int main()
{
	// Function arguments
	double x0 = 1;
	double y0 = exp(1);
	double h = 0.1;
	double xn = 9.0;

	Formula formulas[] = {
		euler_formula,
		heun_formula,
		rungy_kutty_formula
	};

	for(Formula formula : formulas)
	{
		// Warm up each method for better time measurment
		int trials = 10000;
		for (int i = 0; i < trials; i++)
			solve_differential_equation(x0, y0, h, xn, function, formula);

		// Time measurement
		double avg_time = measure_time(trials, x0, y0, h, xn, function, formula);

		// Get the solution
		std::vector<double> results = solve_differential_equation(x0, y0, h, xn, function, formula);

		// Result printing
		std::cout << "Results:\n";
		for (double result : results) std::cout << result << ", ";
		std::cout << "\nTime (nanoseconds): " << avg_time;
		std::cout << "\n--------------------\n";
	}
}