#include <algorithm>
#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
using namespace std;
struct Point
{
	double x, y;
};
struct Circle
{
	Point o;
	double R;
};
double dist(const Point& a, const Point& b)
{
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}
bool is_inside(const Circle& c, const Point& p)
{
	return dist(c.o, p) <= c.R;
}
Point get_circle_center(double bx, double by, double cx, double cy)
{
	double B = bx * bx + by * by;
	double C = cx * cx + cy * cy;
	double D = bx * cy - by * cx;
	return { (cy * B - by * C) / (2 * D), (bx * C - cx * B) / (2 * D) };
}
Circle circle_from(const Point& A, const Point& B, const Point& C)
{
	Point I = get_circle_center(B.x - A.x, B.y - A.y, C.x - A.x, C.y - A.y);
	I.x += A.x;
	I.y += A.y;
	return { I, dist(I, A) };
}
Circle circle_from(const Point& A, const Point& B)
{
	Point C = { (A.x + B.x) / 2.0, (A.y + B.y) / 2.0 };
	return { C, dist(A, B) / 2.0 };
}
bool is_in_circle(const Circle& c, const vector<Point>& P)
{
	for (size_t i = 0; i < P.size(); i++)
	{
		if (!is_inside(c, P[i]))
			return false;
	}
	return true;
}
Circle min_circle(vector<Point>& P)
{
	if (P.empty()) 
	{
		return { { 0, 0 }, 0 };
	}
	else if (P.size() == 1)
	{
		return { P[0], 0 };
	}
	else if (P.size() == 2)
	{
		return circle_from(P[0], P[1]);
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = i + 1; j < 3; j++)
		{
			Circle c = circle_from(P[i], P[j]);
			if (is_in_circle(c, P))
				return c;
		}
	}
	return circle_from(P[0], P[1], P[2]);
}
Circle welzl_algorithm(vector<Point>& P, vector<Point> R, int n)
{
	if (n == 0 || R.size() == 3)
	{
		return min_circle(R);
	}
	int idx = rand() % n;
	Point p = P[idx];
	swap(P[idx], P[n - 1]);
	Circle d = welzl_algorithm(P, R, n - 1);
	if (is_inside(d, p)) 
	{
		return d;
	}
	R.push_back(p);
	return welzl_algorithm(P, R, n - 1);
}
Circle welzl(const vector<Point>& P)
{
	vector<Point> P_copy = P;
	random_shuffle(P_copy.begin(), P_copy.end());
	return welzl_algorithm(P_copy, {}, P_copy.size());
}
int autotest1()
{
	Circle disc = welzl({ {0,0},{1, 0},{1,1},{0.5,0.5},{2,0} });
	int ch = 0;
	if (fabs(disc.o.x - 1) < numeric_limits<double>::epsilon())
	{
		if (fabs(disc.o.y) < numeric_limits<double>::epsilon())
		{
			if (fabs(disc.R - 1) < numeric_limits<double>::epsilon())
			{
				ch = 1;
			}
		}
	}
	if (ch == 1)
	{
		cout << "Autotest1 passed!" << endl;
		return 1;
	}
	else
	{
		cout << "Autotest1 failed!" << endl;
		return 0;
	}
}
int autotest2()
{
	Circle disc = welzl({ {0,0},{1, 0},{1,1},{1,-1} });
	int ch = 0;
	if (fabs(disc.o.x - 1) < numeric_limits<double>::epsilon())
	{
		if (fabs(disc.o.y - 0) < numeric_limits<double>::epsilon())
		{
			if (fabs(disc.R - 1) < numeric_limits<double>::epsilon())
			{
				ch = 1;
			}
		}
	}
	if (ch == 1)
	{
		cout << "Autotest2 passed!" << endl;
		return 1;
	}
	else
	{
		cout << "Autotest2 failed!" << endl;
		return 0;
	}
}
int autotest3()
{
	Circle disc = welzl({ {0,0},{0, 0},{0,0},{0,0} });
	int ch = 0;
	if (fabs(disc.o.x - 0) < numeric_limits<double>::epsilon())
	{
		if (fabs(disc.o.y - 0) < numeric_limits<double>::epsilon())
		{
			if (fabs(disc.R) < numeric_limits<double>::epsilon())
			{
				ch = 1;
			}
		}
	}
	if (ch == 1)
	{
		cout << "Autotest3 passed!" << endl;
		return 1;
	}
	else
	{
		cout << "Autotest3 failed!" << endl;
		return 0;
	}
}
int autotest()
{
	if (autotest1() && autotest2() && autotest3() == 1)
	{
		cout << "Autotest passed!" << endl;
		return 0;
	}
	else
	{
		cout << "Autotest failed!" << endl;
		return 1;
	}
}
int main()
{
	if (autotest() == 0)
	{
		string filename;
		cout << "Enter filename:";
		cin >> filename;
		ifstream fin(filename);
		if (!fin.is_open())
		{
			cout << "Can't open the file!";
			fin.close();
			return 1;
		}
		else
		{
			int n;
			fin >> n;
			vector<Point> ar(n);
			for (int i = 0; i < n; i++)
			{
				fin >> ar[i].x;
				fin >> ar[i].y;
			}
			Circle disc = welzl(ar);
			for (int i = 0; i < n; i++)
			{
				cout << "dist" <<i+1 << " = " << dist(ar[i],disc.o) << endl;
			}
			cout << "Center = { " << disc.o.x << ", " << disc.o.y << " } Radius = " << disc.R << endl;
			fin.close();
			return 0;
		}
	}
	else
	{
		return 1;
	}
}