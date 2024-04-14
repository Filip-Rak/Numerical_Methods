#include <iostream>
#include <cmath>

double gaussian_quadrature_2D(double x[], double y[])
{
	// precomputed derivatives with respect to xi
	const double d_xi[2][4] = {
		{-0.394338, 0.394338, 0.105662, -0.105662},
		{-0.105662, 0.105662, 0.394338, -0.394338}
	};

	// precomputed derivatives with respect to eta
	const double d_eta[2][4] = {
		{-0.394338, -0.105662, 0.105662, 0.394338},
		{-0.105662, -0.394338, 0.394338, 0.105662}
	};

	double area = 0;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			// partial derivatives for jakobi matrix
			double dx_d_xi = 0;
			double dy_d_xi = 0;
			double dx_d_eta = 0;
			double dy_d_eta = 0;

			for (int k = 0; k < 4; k++)
			{
				dx_d_xi += d_xi[j][k] * x[k];
				dy_d_xi += d_xi[j][k] * y[k];
				dx_d_eta += d_eta[j][k] * x[k];
				dy_d_eta += d_eta[j][k] * y[k];
			}

			// area is the sum of all jakobians multiplied by the weights
			// weights are equal to 1, therefore we skip the multiplication
			area += fabs(dx_d_xi * dy_d_eta - dx_d_eta * dy_d_xi);
		}
	}

	return area;
}

// this function is completely unnecessary for program execution
// it is not called at any point in the code
// it was however used to compute eta and xi derivatives
// because of that I left it as a visualization
void precompute(double x[], double y[])
{
	const double weight[2] = { 1, 1 };
	const double point[2] = { -0.57735026920, 0.57735026920 };

	//derivatives with respect to xi and eta
	double d_xi[2][4];
	double d_eta[2][4];

	for (int i = 0; i < 2; i++)
	{
		d_xi[i][0] = -0.25 * (1.0 - point[i]);
		d_xi[i][1] = 0.25 * (1.0 - point[i]);
		d_xi[i][2] = 0.25 * (1.0 + point[i]);
		d_xi[i][3] = -0.25 * (1.0 + point[i]);

		d_eta[i][0] = -0.25 * (1.0 - point[i]);
		d_eta[i][1] = -0.25 * (1.0 + point[i]);
		d_eta[i][2] = 0.25 * (1.0 + point[i]);
		d_eta[i][3] = 0.25 * (1.0 - point[i]);
	}

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 4; j++)
			std::cout << "d_xi[" << i << "][" << j << "] = " << d_xi[i][j] << "\n";

	std::cout << "\n\n";

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 4; j++)
			std::cout << "d_eta[" << i << "][" << j << "] = " << d_eta[i][j] << "\n";
}

int main()
{
	double x[] = { 0.01, 0.03, 0.02, 0 };
	double y[] = { 0, 0.005, 0.01, 0.01 };

    std::cout << gaussian_quadrature_2D(x, y);
}