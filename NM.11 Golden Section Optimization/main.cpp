#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>

const double PHI = 0.61803398875;

double goldenSectionSearch(double a, double b, double (*f)(double), double epsilon) 
{
    // Initilization for search range
    double x_l = b - PHI * (b - a);
    double x_p = a + PHI * (b - a);

    // Search loop
    while ((b - a) > epsilon) 
    {
        // Min on rigth
        if (f(x_l) > f(x_p)) 
        {
            a = x_l;
            x_l = x_p;
            x_p = a + PHI * (b - a);
        }
        
        // Min on left
        else 
        {
            b = x_p;
            x_p = x_l;
            x_l = b - PHI * (b - a);
        }
    }

    // Return the average of the two
    return (a + b) / 2;
}

double function(double x)
{
    return sin(x);
}

int main()
{
    double a = 0.0;
    double b = M_PI * 2;
    double epsilon = 0.0001;

    std::cout << goldenSectionSearch(a, b, function, epsilon) << "\n";
}