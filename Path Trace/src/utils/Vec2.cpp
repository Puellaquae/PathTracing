#include <utils/Vec2.h>

#include <algorithm>
#include <cmath>

namespace RayTrace
{
	Real Vec2::length() const
	{
		return std::sqrt(sqrLength());
	}

	Real Vec2::sqrLength() const
	{
		return u * u + v * v;
	}

	bool Vec2::nearZero() const
	{
		constexpr Real s = 1e-8;
		return (std::abs(u) < s) && (std::abs(v) < s);
	}

	Vec2 operator+(const Vec2 a, const Vec2 b)
	{
		return Vec2{ a.u + b.u, a.v + b.v };
	}

	Vec2 operator-(const Vec2 v)
	{
		return Vec2{-v.u, -v.v};
	}

	Vec2 operator-(const Vec2 a, const Vec2 b)
	{
		return Vec2{a.u - b.u, a.v - b.v};
	}

	Vec2 operator*(const Vec2 a, const Vec2 b)
	{
		return Vec2{a.u * b.u, a.v * b.v};
	}

	Vec2 operator*(const Vec2 v, const Real s)
	{
		return Vec2{v.u * s, v.v * s};
	}

	Vec2 operator*(const Real s, const Vec2 v)
	{
		return Vec2{v.u * s, v.v * s};
	}

	Vec2 operator/(const Vec2 v, const Real s)
	{
		return Vec2{v.u / s, v.v / s};
	}

	Real dot(const Vec2 a, const Vec2 b)
	{
		return a.u * b.u + a.v * b.v;
	}

	Real cross(const Vec2 a, const Vec2 b)
	{
		return a.u * b.v - b.u * a.v;
	}

	Vec2 norm(const Vec2 v)
	{
		return v / v.length();
	}

	Vec2 min(Vec2 a, Vec2 b)
	{
		return Vec2{std::min(a.u, b.u), std::min(a.v, b.v)};
	}

	Vec2 max(Vec2 a, Vec2 b)
	{
		return Vec2{std::max(a.u, b.u), std::max(a.v, b.v)};
	}
}

