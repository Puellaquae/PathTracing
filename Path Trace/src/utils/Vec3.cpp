#include <utils/Vec3.h>

#include <algorithm>
#include <cmath>

namespace RayTrace
{
	Real Vec3::length() const
	{
		return std::sqrt(sqrLength());
	}

	Real Vec3::sqrLength() const
	{
		return x * x + y * y + z * z;
	}

	bool Vec3::nearZero() const
	{
		constexpr Real s = 1e-8;
		return (std::abs(x) < s) && (std::abs(y) < s) && (std::abs(z) < s);
	}

	Vec3 operator+(const Vec3 a, const Vec3 b)
	{
		return Vec3{a.x + b.x, a.y + b.y, a.z + b.z};
	}

	Vec3 operator-(const Vec3 v)
	{
		return Vec3{-v.x, -v.y, -v.z};
	}

	Vec3 operator-(const Vec3 a, const Vec3 b)
	{
		return Vec3{a.x - b.x, a.y - b.y, a.z - b.z};
	}

	Vec3 operator*(const Vec3 a, const Vec3 b)
	{
		return Vec3{a.x * b.x, a.y * b.y, a.z * b.z};
	}

	Vec3 operator*(const Vec3 v, const Real s)
	{
		return Vec3{v.x * s, v.y * s, v.z * s};
	}

	Vec3 operator*(const Real s, const Vec3 v)
	{
		return Vec3{v.x * s, v.y * s, v.z * s};
	}

	Vec3 operator/(const Vec3 v, const Real s)
	{
		return Vec3{v.x / s, v.y / s, v.z / s};
	}

	Real dot(const Vec3 a, const Vec3 b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	Vec3 cross(const Vec3 a, const Vec3 b)
	{
		return Vec3{
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		};
	}

	Vec3 norm(const Vec3 v)
	{
		return v / v.length();
	}

	Vec3 min(const Vec3 a, const Vec3 b)
	{
		return Vec3{std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z)};
	}

	Vec3 max(const Vec3 a, const Vec3 b)
	{
		return Vec3{std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z)};
	}

	Real max(const Vec3 v)
	{
		return std::max(std::max(v.x,v.y),v.z);
	}

	Real min(const Vec3 v)
	{
		return std::min(std::min(v.x,v.y),v.z);
	}
}
