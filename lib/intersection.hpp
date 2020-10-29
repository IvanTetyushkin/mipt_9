#pragma once
#include <iostream>
#include <cmath>
#include <cassert>
#include <array>
namespace handmade
{
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
		return (std::abs(x - other.x) < tolerance) &&
			(std::abs(y - other.y) < tolerance) &&
			(std::abs(z - other.z) < tolerance);
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
	static T dot(const Point& p1, const Point& p2)
	{
		return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
	}
	T length() const
	{
		return std::sqrt(dot(*this, *this));
	}
	T project(const Point& p) const
	{
		//assert(length() == 1 && "Error projection non-one vector");
		return dot(*this, p);
	}

	Point& normilize()
	{
		T coeff = length();
		x /= coeff;
		y /= coeff;
		z /= coeff;
		return *this;
	}
	static Point cross(const Point& p1, const Point& p2)
	{
		return Point{p1.y * p2.z - p1.z * p2.y, p1.z* p2.x - p1.x*p2.z, p1.x * p2.y - p1.y * p2.x};
	}
	static int isParallel(const Point& p1, const Point& p2)
	{
		// zero case
		bool x_aces = p1.x == 0 && p2.x != 0;
		bool y_aces = p1.y == 0 && p2.y != 0;
		bool z_aces = p1.z == 0 && p2.z != 0;
		if (x_aces || y_aces || z_aces)
			return 0;// not parrallel at all
		T x_coeff = 0;
		T y_coeff = 0;
		T z_coeff = 0;
		if(p2.x != 0)
			x_coeff = p1.x / p2.x;
		if(p2.y != 0)
			y_coeff = p1.y / p2.y;
		if(p2.y != 0)
			z_coeff = p1.z / p2.z;
		if (x_coeff == 0 && y_coeff == 0 && z_coeff == 0)
			return 1;// well, you give the same points... (0,0,0)
		T tmp_coeff = x_coeff != 0 ? x_coeff :
			y_coeff != 0 ? y_coeff : z_coeff; // well one of them not null
		assert(tmp_coeff != 0 && "isParrallel logic error");
		bool result = true;
		// may be case with x = 0...
		result &= (tmp_coeff == (x_coeff != 0 ? x_coeff : tmp_coeff));
		result &= (tmp_coeff == (y_coeff != 0 ? y_coeff : tmp_coeff));
		result &= (tmp_coeff == (z_coeff != 0 ? z_coeff : tmp_coeff));

		if (!result)
			return 0;
		if (tmp_coeff > 0)
			return 1;// they are one side
		else
			return -1;// they are other side
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
class Vector
{
	Point<T> val;
	using PointT = Point<T>;
public:
	Vector(const PointT& from, const PointT& to) :
		val(to - from) {};
	Vector(const PointT& p) :
		val(p) {};
	bool equal(const Vector& v) const
	{
		return (val == v.val);
	}
	static T dot(const Vector& f, const Vector& s)
	{
		return PointT::dot(f.val, s.val);
	}
	static Vector cross(const Vector& f, const Vector& s)
	{
		return PointT::cross(f.val, s.val);
	}
	T getLen() const
	{
		return val.length();
	}
	Vector& normilize()
	{
		val.normilize();
		return *this;
	}
	static int isParallel(const Vector& v1, const Vector& v2)
	{
		return PointT::isParallel(v1.val, v2.val);
	}
	T project(const PointT& p) const
	{
		return val.project(p);
	}
};
template<typename T>
class Convex
{
	static constexpr size_t size = 3;
	std::array<Point<T>, size> Points;
	//Vector N;
	using VectorT = Vector<T>;
	using IntervalT = Interval<T>;

	void validate() const
	{
		bool check = true;
		// trust me points different
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
public:
	Convex(const std::array<Point<T>, 3>& ps) :
		Points{ ps } 
	{
		validate();
	};


	// https://www.geometrictools.com/Documentation/MethodOfSeparatingAxes.pdf
	static bool isIntersect(const Convex& lhs, const Convex& rhs)
	{
		VectorT lhs_N = VectorT::cross(VectorT{ lhs.Points[1], lhs.Points[0] },
			VectorT{ lhs.Points[1], lhs.Points[2] }).normilize();
		VectorT rhs_N = VectorT::cross(VectorT{ rhs.Points[1], rhs.Points[0] },
			VectorT{ rhs.Points[1], rhs.Points[2] }).normilize();
		// first
		//assert(lhs_N.getLen() == 1 && "normale non-one");
		//assert(rhs_N.getLen() == 1 && "normale non-one");

		IntervalT lhs_proj, rhs_proj;
		// N
		lhs_proj = lhs.getMinMaxProjection(lhs_N);
		rhs_proj = rhs.getMinMaxProjection(lhs_N);
		if (IntervalT::areNotIntersect(lhs_proj, rhs_proj))
			return false;
		lhs_proj = lhs.getMinMaxProjection(rhs_N);
		rhs_proj = rhs.getMinMaxProjection(rhs_N);
		if (IntervalT::areNotIntersect(lhs_proj, rhs_proj))
			return false;
		// lhs.edge * rhs.edge
		for (int i = 0; i < (lhs.size + 1); ++i)
		{
			int id0 = i % lhs.size;
			int id1 = (i + 1) % lhs.size;
			VectorT lhs_edge{lhs.Points[id0], lhs.Points[id1]};
			for (int j = 0; j < (rhs.size + 1); ++j)
			{
				id0 = j % rhs.size;
				id1 = (j + 1) % rhs.size;
				VectorT rhs_edge{rhs.Points[id0], rhs.Points[id1]};

				VectorT current_ax = VectorT::cross(lhs_edge, rhs_edge).normilize();
				lhs_proj = lhs.getMinMaxProjection(current_ax);
				rhs_proj = rhs.getMinMaxProjection(current_ax);
				if (IntervalT::areNotIntersect(lhs_proj, rhs_proj))
					return false;
			}
		}
		// lhs_N * lhs_edge
		for (int i = 0; i < (lhs.size + 1); ++i)
		{
			int id0 = i % lhs.size;
			int id1 = (i + 1) % lhs.size;
			VectorT lhs_edge{lhs.Points[id0], lhs.Points[id1]};

			VectorT current_ax = VectorT::cross(lhs_edge, lhs_N).normilize();
			lhs_proj = lhs.getMinMaxProjection(current_ax);
			rhs_proj = rhs.getMinMaxProjection(current_ax);
			if (IntervalT::areNotIntersect(lhs_proj, rhs_proj))
				return false;
		}
		// lhs_N * rhs_edge
		for (int i = 0; i < (rhs.size + 1); ++i)
		{
			int id0 = i % rhs.size;
			int id1 = (i + 1) % rhs.size;
			VectorT rhs_edge{rhs.Points[id0], rhs.Points[id1]};

			VectorT current_ax = VectorT::cross(rhs_edge, lhs_N).normilize();
			lhs_proj = lhs.getMinMaxProjection(current_ax);
			rhs_proj = rhs.getMinMaxProjection(current_ax);
			if (IntervalT::areNotIntersect(lhs_proj, rhs_proj))
				return false;
		}
		// seems all, they have intersect... (
		return true;
	}
};
}