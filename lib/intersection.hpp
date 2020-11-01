#pragma once
#include <iostream>
#include <cmath>
#include <cassert>
#include <array>
namespace handmade
{
	template<typename T>
	bool almostEqual(T a, T b, T pol = 1e-3)
	{
		return std::abs(a - b) < pol;
	}
	template<typename T>
class Interval
{
	T left;
	T right;
public:
	Interval(T l = NAN, T r = NAN) :
		left{ l }, right{ r } {};
	static bool areNotIntersect(const Interval& lhs, const Interval& rhs)
	{
		return lhs.left > rhs.right || lhs.right < rhs.left;
	}
	static bool areIntersect(const Interval& lhs, const Interval& rhs)
	{
		return !areNotIntersect(lhs, rhs);
	}
};
template<typename T>
class Point
{
protected:
	T x;
	T y;
	T z;
public:
	Point(T _x = 0, T _y = 0, T _z = 0) :
		x(_x),
		y(_y),
		z(_z) {}
	void dump() const
	{
		std::cout << "(" << x << ", " << y << ", " << z << ")\n";
	}
	bool equal(const Point& other) const
	{
		constexpr T tolerance = 1e-3;
		return (almostEqual(x, other.x, tolerance)) &&
			(almostEqual(y, other.y, tolerance)) &&
			(almostEqual(z, other.z, tolerance));
	}
	Point operator-() const
	{
		return Point{ -x, -y, -z };
	}
	Point& operator+=(const Point& p)
	{
		x += p.x;
		y += p.y;
		z += p.z;
		return *this;
	}
	Point& operator-=(const Point& p)
	{
		x -= p.x;
		y -= p.y;
		z -= p.z;
		return *this;
	}

};
template<typename T>
Point<T> operator+(const Point<T>& p1, const Point<T>& p2)
{
	Point<T> ret{ p1 };
	return	ret += p2;
}
template<typename T>
Point<T> operator-(const Point<T>& p1, const Point<T>& p2)
{
	Point<T> ret{ p1 };
	return	ret -= p2;
}
template<typename T>
bool operator==(const Point<T>& p1, const Point<T>& p2)
{
	return	p1.equal(p2);
}
template<typename T>
bool operator!=(const Point<T>& p1, const Point<T>& p2)
{
	return	!p1.equal(p2);
}
//in fact I do not need it, but for look...
template<typename T>
class Vector final : public Point<T>
{
	using PointT = Point<T>;
public:
	Vector(const PointT& from, const PointT& to) :
		Point<T>(to - from) {};
	Vector(const PointT& p) :
		Point<T>(p) {};
	Vector(T x, T y, T z) :
		Point<T>(x, y, z) {};

	static T dot(const Vector& lhs, const Vector& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}
	static Vector cross(const Vector& p1, const Vector& p2)
	{
		return Vector{ p1.y * p2.z - p1.z * p2.y, p1.z* p2.x - p1.x*p2.z, p1.x * p2.y - p1.y * p2.x};
	}
	T getLen() const
	{
		return std::sqrt(dot(*this, *this));
	}
	// not need 
#if 0
	Vector& normilize()
	{
		T coeff = getLen();
		x /= coeff;
		y /= coeff;
		z /= coeff;
		return *this;
	}
#endif
	static int isParallel(const Vector& lhs, const Vector& rhs)
	{
		{
			// zero case in div
			bool x_aces = lhs.x == 0 && rhs.x != 0;
			bool y_aces = lhs.y == 0 && rhs.y != 0;
			bool z_aces = lhs.z == 0 && rhs.z != 0;
			if (x_aces || y_aces || z_aces)
				return 0;// not parrallel at all
		}
		T x_coeff = 0;
		T y_coeff = 0;
		T z_coeff = 0;
		if(rhs.x != 0)
			x_coeff = lhs.x / rhs.x;
		if(rhs.y != 0)
			y_coeff = lhs.y / rhs.y;
		if(rhs.y != 0)
			z_coeff = lhs.z / rhs.z;

		if (x_coeff == 0 && y_coeff == 0 && z_coeff == 0)
			return 1;// well, you give the same points... (0,0,0)
		T tmp_coeff = x_coeff != 0 ? x_coeff :
			y_coeff != 0 ? y_coeff : z_coeff; // well one of them not null
		assert(tmp_coeff != 0 && "isParrallel logic error");
		bool result = true;

		// may be case with x = 0...
		result &= almostEqual(tmp_coeff, x_coeff != 0 ? x_coeff : tmp_coeff);
		result &= almostEqual(tmp_coeff, y_coeff != 0 ? y_coeff : tmp_coeff);
		result &= almostEqual(tmp_coeff, z_coeff != 0 ? z_coeff : tmp_coeff);

		if (!result)
			return 0;
		if (tmp_coeff > 0)
			return 1;// they are one side
		else
			return -1;// they are other side
	}
	T project(const Vector& p) const
	{
		return dot(*this, p);
	}
};
template<typename T, size_t size = 3>
class Convex
{
	std::array<Point<T>, size> Points;
	//Vector N;
	using VectorT = Vector<T>;
	using IntervalT = Interval<T>;

	void validate() const
	{
		// for triangle not interesting
		if constexpr (size == 3)
			return;
		bool check = true;
		// trust me points different!!
		for (int i = 0, to = size / 4; i < to; ++i)
		{
			int ind0 = (size + 0) % size;
			int ind1 = (size + 1) % size;
			int ind2 = (size + 2) % size;
			int ind3 = (size + 3) % size;
			check &= (VectorT::isParallel(
				VectorT::cross(VectorT{ Points[ind1], Points[ind0] },
					VectorT{ Points[ind1], Points[ind2] }),
				VectorT::cross(VectorT{ Points[ind2], Points[ind1] },
					VectorT{ Points[ind2], Points[ind3] })) == 1);
		}
		if (!check)
			assert("not convex given");
	}
	IntervalT getMinMaxProjection(const VectorT& v) const
	{
			T right = v.project(Points[0]);
			T left = v.project(Points[0]);
			for (int i = 1; i < size; ++i)
			{
				T tmp_res = v.project(Points[i]);
				left = (tmp_res < left ? tmp_res : left);
				right = (tmp_res > right ? tmp_res : right);
			}
			return IntervalT(left, right);
	}
	VectorT getEdge(int i) const
	{
		assert(i < (size + 1) && "error i for edge too big");
		int id0 = i % size;
		int id1 = (i + 1) % size;
		assert(Points[id0] != Points[id1] && "Same point error");
		return VectorT{ Points[id0], Points[id1] };
	}
	// true -> not intersect, end 
	// false -> intersect
	template<size_t size1, size_t size2>
	static bool checkAx(const Convex<T, size1>& lhs, const Convex<T, size2>& rhs, const VectorT& ax)
	{
		IntervalT lhs_proj = lhs.getMinMaxProjection(ax);
		IntervalT rhs_proj = rhs.getMinMaxProjection(ax);
		if (IntervalT::areNotIntersect(lhs_proj, rhs_proj))
			return true;
		return false;
	}
public:
	Convex(const std::array<Point<T>, 3>& ps) :
		Points{ ps } 
	{
		validate();
	};
	// https://www.geometrictools.com/Documentation/MethodOfSeparatingAxes.pdf
	template<size_t size1, size_t size2>
	static bool isIntersect(const Convex<T, size1>& lhs, const Convex<T, size2>& rhs)
	{
		VectorT lhs_N = VectorT::cross(VectorT{ lhs.Points[1], lhs.Points[0] },
			VectorT{ lhs.Points[1], lhs.Points[2] });
		VectorT rhs_N = VectorT::cross(VectorT{ rhs.Points[1], rhs.Points[0] },
			VectorT{ rhs.Points[1], rhs.Points[2] });

		// N
		if (checkAx(lhs, rhs, lhs_N))
			return false;
		if (checkAx(lhs, rhs, rhs_N))
			return false;
		// lhs.edge * rhs.edge
		for (int i = 0; i < (size1 + 1); ++i)
		{
			VectorT lhs_edge = lhs.getEdge(i);
			for (int j = 0; j < (size2 + 1); ++j)
			{
				VectorT rhs_edge = rhs.getEdge(j);
				VectorT current_ax = VectorT::cross(lhs_edge, rhs_edge);
				if (checkAx(lhs, rhs, current_ax))
					return false;
			}
		}
		// lhs_N * lhs_edge
		for (int i = 0; i < (size1 + 1); ++i)
		{
			VectorT lhs_edge = lhs.getEdge(i);
			VectorT current_ax = VectorT::cross(lhs_edge, lhs_N);
			if (checkAx(lhs, rhs, current_ax))
				return false;
		}
		// lhs_N * rhs_edge
		for (int i = 0; i < (size2 + 1); ++i)
		{
			VectorT rhs_edge = rhs.getEdge(i);
			VectorT current_ax = VectorT::cross(rhs_edge, lhs_N);
			if (checkAx(lhs, rhs, current_ax))
				return false;
		}
		// seems all, they have intersect... (
		return true;
	}
};
}
