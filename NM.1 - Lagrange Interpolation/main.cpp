#include <iostream>

struct Point {
	double x, y;

	Point(int x, int y): x(x), y(y){}
	Point(): x(0), y(0) {}
};

double lagrange_interpolation(int n, Point* p_arr, double tgt_x)
{
	double *l_arr = new double[n];

	for (int i = 0; i < n; i++)
	{
		l_arr[i] = 1;
		for (int j = 0; j < n; j++)
		{
			if (j != i)	
				l_arr[i] *= (tgt_x - p_arr[j].x) / (p_arr[i].x - p_arr[j].x);
		}
	}

	double L = 0;
	for (int i = 0; i < n; i++)
		L += l_arr[i] * p_arr[i].y;

	delete[] l_arr;

	return L;
}

int main()
{
	Point p[] = {
		Point(-2, 0.13),
		Point(2, 7.38),
		Point(3, 5)
	};

	int n = sizeof(p) / sizeof(Point);

	double tgt_x = 1;
	std::cout << lagrange_interpolation(n, p, tgt_x);
}