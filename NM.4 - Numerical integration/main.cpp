#include <iostream>

double rectangleRule(double (*f)(double), double a, double b, int n)
{
	double dx = (b - a) / n;    //width of a rectangle

    double result = 0;
    for (int i = 1; i <= n; i++) 
    { 
        double x = a + i * dx;
        result += f(x) * dx;
    }

    return result;
}

double trapezoidalRule(double (*f)(double), double a, double b, int n) 
{
    double dx = (b - a) / n;

    double result = (f(a) + f(b)) / 2.0;
    for (int i = 1; i < n; i++) 
    {
        double x = a + i * dx;
        result += f(x);
    }
;
    return result * dx;
}

double simpsonRule(double (*f)(double), double a, double b, int n)
{
    if (n % 2 != 0)
        throw std::invalid_argument("EXCEPTION: n has to be divisible by 2");

    double h = (b - a) / n; 

    double sum = f(a) + f(b);
    for (int i = 1; i < n; i++) 
    {
        double x = a + i * h;

        //multiplier 2 for even indecies and 4 for odd indecies
        if (i % 2 == 0)
            sum += 2 * f(x);
        else
            sum += 4 * f(x); 
    }

    return sum * h / 3;
}

double monteCarloMethod(double (*f)(double), double a, double b, int n)
{
    srand(time(NULL));

    double avg = 0;
    for (int i = 0; i < n; i++)
    {
        double x = a + (double)rand() / RAND_MAX * (b - a);
        avg += f(x) / n;
    }

    double result = avg * abs(b - a);
    return result;
}

double function(double x)
{
    return pow(x, 3) + 2;
}

int main()
{
    double a = 1, b = 5;
    int n = 4;

	std::cout << "Rectangle method: " << rectangleRule(function, a, b, n);
	std::cout << "\nTrapezoidal method:" << trapezoidalRule(function, a, b, n);
    std::cout << "\nSimpson method: ";
    try { std::cout << simpsonRule(function, a, b, n); }
    catch (std::invalid_argument i){ std::cout << i.what(); }
    std::cout << "\nMonte Carlo method: " << monteCarloMethod(function, a, b, n);
}