#include <intersection.hpp>
#include <iostream>
#include <vector>
#include <set>

int main()
{
	using namespace std;
	using namespace handmade;
	int num;
	cin >> num;
	std::vector<Convex<double>> triangles;
	triangles.reserve(num);
	std::array<Point<double>, 3> tmp_points;
	for (int i = 0; i < num; ++i)
	{
		double x, y, z;
		double x2, y2, z2;
		double x3, y3, z3;
		cin >> x >> y >> z;
		cin >> x2>> y2 >> z2;
		cin >> x3 >> y3 >> z3;
		tmp_points[0] = Point<double>{ x, y, z };
		tmp_points[1] = Point<double>{ x2, y2, z2 };
		tmp_points[2] = Point<double>{ x3, y3, z3 };
		triangles.push_back(tmp_points);
	}
	std::set<int> have_intersect;

	for (int it = 0, end = num; it < end; ++it)
	{
		for (int second = it + 1; second < end; ++second)
		{
			if (Convex<double>::isIntersect(triangles[it], triangles[second]))
			{
				have_intersect.insert(it);
				have_intersect.insert(second);
			}
		}
	}
	for (const auto& val : have_intersect)
		cout << val << " ";
	cout << "\n";
	return 0;
}
