#pragma once
namespace RayTrace
{
	struct Vec2
	{
		double u, v;

		double length() const
		{
			return sqrt(sqrLength());
		}

		double sqrLength() const
		{
			return u * u + v * v;
		}

		bool nearZero() const
		{
			constexpr auto s = 1e-8;
			return (fabs(u) < s) && (fabs(v) < s);
		}
	};

	inline Vec2 operator+(const Vec2 a, const Vec2 b)
	{
		return Vec2{ a.u + b.u,a.v + b.v };
	}

	inline Vec2 operator-(const Vec2 v)
	{
		return Vec2{ -v.u,-v.v };
	}

	inline Vec2 operator-(const Vec2 a, const Vec2 b)
	{
		return Vec2{ a.u - b.u,a.v - b.v };
	}

	inline Vec2 operator*(const Vec2 a, const Vec2 b)
	{
		return Vec2{ a.u * b.u,a.v * b.v };
	}

	inline Vec2 operator*(const Vec2 v, const double s)
	{
		return Vec2{ v.u * s,v.v * s };
	}

	inline Vec2 operator*(const double s, const Vec2 v)
	{
		return Vec2{ v.u * s,v.v * s };
	}

	inline Vec2 operator/(const Vec2 v, const double s) {
		return Vec2{ v.u / s,v.v / s };
	}

	inline double dot(const Vec2 a, const Vec2 b)
	{
		return a.u * b.u + a.v * b.v;
	}

	inline double cross(const Vec2 a, const Vec2 b)
	{
		return a.u * b.v - b.u * a.v;
	}

	inline Vec2 norm(const Vec2 v)
	{
		return v / v.length();
	}

	inline Vec2 min(Vec2 a, Vec2 b)
	{
		return Vec2{ fmin(a.u,b.u),fmin(a.v,b.v) };
	}

	inline Vec2 max(Vec2 a, Vec2 b)
	{
		return Vec2{ fmax(a.u,b.u),fmax(a.v,b.v) };
	}

	using Coord = Vec2;
}