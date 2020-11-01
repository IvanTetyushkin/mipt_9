#include <intersection.hpp>
#include <gtest/gtest.h>

using namespace handmade;

TEST(hello, hello)
{
	ASSERT_EQ(10, 10);
}
TEST(interval, basic)
{
	Interval<double> a(10, 11);
	Interval<double> b(9, 10);
	ASSERT_EQ(Interval<double>::areIntersect(a, b), true);
	ASSERT_EQ(Interval<double>::areNotIntersect(a, b), false);
	Interval<double> c(10, 23);
	ASSERT_EQ(Interval<double>::areIntersect(b, c), true);
	ASSERT_EQ(Interval<double>::areIntersect(c, b), true);
	ASSERT_EQ(Interval<double>::areIntersect(c, a), true);
}
TEST(Vector, dotProduct)
{
	Point<float> a{ 0, 0, 1 };
	Point<float> b{ 1, 0, 0 };
	ASSERT_EQ(Vector<float>::dot(a, b), 0);
	Point<float> c{ 1.1, 2.2, 3.3 };
	Point<float> d{ 2.1, 2.2, 2.3 };
	ASSERT_FLOAT_EQ(Vector<float>::dot(c, d), 14.74);
}
TEST(Vector, cross)
{
	Point<double> a{ 0, 0, 1 };
	Point<double> b{ 1, 0, 0 };
	ASSERT_EQ(Vector<double>::cross(a, b), Point<double>(0, 1, 0));
	Point<double> c{ 1.1, 2.2, 3.3 };
	Point<double> d{ 2.1, 2.2, 2.3 };
	ASSERT_EQ(Vector<double>::cross(c, d).equal(Point<double>(-2.2, 4.4, -2.2)) , true);
}
TEST(Vector, project)
{
	using pf = Vector<float>;
	pf a{ 0, 0, 1 };
	pf b{ 1, 0, 0 };
	ASSERT_EQ(a.project(b), 0);
	pf c{ 17, 0, 1 };
	pf d{ 1, 0, 0 };
	ASSERT_EQ(d.project(c), 17);
}
TEST(Vector, isParallel)
{
	using dp = Point<double>;
	dp a{ 0, 0, 1 };
	dp b{ 1, 0, 0 };
	ASSERT_EQ(Vector<double>::isParallel(a, b), 0);
	dp c{ 17, 0, 17 };
	dp d{ 1, 0, 1 };
	ASSERT_EQ(Vector<double>::isParallel(c, d), 1);
	ASSERT_EQ(Vector<double>::isParallel(c, -d), -1);
}
TEST(trianle, intersecton)
{
	std::array<Point<float>, 3> points1 = { Point<float>(1, 0, 0), Point<float>(0, 1, 0), Point<float>(0, 0, 1) };
	Convex<float, 3> tr1(points1);
	std::array<Point<float>, 3> points2 = { Point<float>(0, 0, 6), Point<float>(0, 0.5, 0), Point<float>(0, 0, 0) };
	Convex<float, 3> tr2(points2);
	ASSERT_EQ(Convex<float>::isIntersect(tr1, tr2), true);
	ASSERT_EQ(Convex<float>::isIntersect(tr2, tr1), true);
	std::array<Point<double>, 3> points3 = { Point<double>(1, 0, 0), Point<double>(0, 1, 0), Point<double>(0, 0, 1) };
	Convex<double, 3> tr3(points3);
	std::array<Point<double>, 3> points4 = { Point<double>(2, 0, 0), Point<double>(0, 2, 0), Point<double>(0, 0, 2) };
	Convex<double> tr4(points4);
	ASSERT_EQ(Convex<double>::isIntersect(tr3, tr4), false);
	ASSERT_EQ(Convex<double>::isIntersect(tr4, tr3), false);
	// well, i really do not know how to test this...
	// as basically I need test to all directions, this is 3 + 9 + 6 
}
