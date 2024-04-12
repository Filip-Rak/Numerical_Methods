#include <iostream>
#include <math.h>

double gaussian_quadrature(double (*f)(double), double a, double b)
{
	const int size = 4;
	double nodes[size] = { 
		-0.8611363115940525,
		-0.33998104358485626,
		0.33998104358485626,
		0.8611363115940525
	};
	double weights[size] = { 
		0.3478548451374538,
		0.6521451548625461,
		0.6521451548625461,
		0.3478548451374538
	};

	double result = 0;
	for (int i = 0; i < size; i++)
	{
		double t = (a + b) / 2 + ((b - a) / 2) * nodes[i];
		result += weights[i] * f(t);
	}

	return (b - a) / 2 * result;
}

double function(double x)
{
	return x * x;
}

int main()
{
	std::cout << gaussian_quadrature(function, -2, 2);
}