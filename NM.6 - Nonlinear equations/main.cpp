#include <iostream>
#include <cmath>

double numericDerivative(double (*f)(double), double x, double h = 1e-5)
{
	return (f(x + h) - f(x - h)) / (2 * h);
}

double bisection(double (*f)(double), double a, double b, double threshold)
{
	// The interval gets smaller with each iteration,
	// as long as, we do not reach the limit
	while ((a - b) / 2 > threshold)
	{
		double mid = (a + b) / 2;

		// If the middle point is a root, return it
		if (f(mid) == 0)
			return mid;
	
		// Mid splits the interval
		// If the multiplication is negative, 
		// the points must have different signs.
		// For that to happen there must be an x
		// that will give the value of 0 between the two

		if (f(a) * f(mid) < 0)
			b = mid;
		else
			a = mid;
	}

	// Approximation - we couldn't find the exact number
	return (a + b) / 2;
}

double newtonRaphson(double (*f)(double), double x0, double threshold, double h = 1e-5) 
{
	double x1;
	while (true) 
	{
		double dfx0 = numericDerivative(f, x0, h);
		x1 = x0 - f(x0) / dfx0;
		if (fabs(x1 - x0) < threshold)
			return x1;

		x0 = x1;
	}
}

double function(double x)
{
	return (x * x * x) - (3 * x) + 2;
}

int main()
{
	double a = 0, b = 2, threshold = pow(10, -12);
	std::cout << "Bisection: " << bisection(function, a, b, threshold);
	std::cout << "\nNewton - Raphson: " << newtonRaphson(function, -3, threshold);
}